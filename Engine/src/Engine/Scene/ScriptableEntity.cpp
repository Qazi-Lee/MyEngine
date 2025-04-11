#include"pch.h"

#include"ScriptableEntity.h"

#include<mono/jit/jit.h>
#include <mono/metadata/mono-config.h>



namespace ENGINE
{


	CLRLoader::CLRLoader(const char* assemblyPath, const char* assemblyName)
	{
		std::string command;
	//TODO:引用已有dll
		command = "mcs -lib:C:\\Windows\\Microsoft.NET\\assembly\\GAC_MSIL\\System.Runtime\\v4.0_4.0.0.0__b03f5f7f11d50a3a ";
		command+="-r:ScriptEngine.dll -r:System.Runtime.dll ";
	//编译成dll
		command += "-target:library -out:"; command += assemblyName; command += ".dll "; command += assemblyPath;
	//	std::cout << command << std::endl;
		system(command.c_str());


		if (!mono_get_root_domain())
		{
		//	std::cout << "domain初始化" << std::endl;
			mono_jit_init("C#");
		}
		domain = mono_domain_create();
		std::string dllname = assemblyName; dllname += ".dll";
	//	std::cout << "dllname:" << dllname << std::endl;
		MonoAssembly* assembly = mono_domain_assembly_open(domain,dllname.c_str() );
		dllname = "ScriptEngine.dll";
		MonoAssembly* depassembly = mono_domain_assembly_open(domain, dllname.c_str());
		depimage = mono_assembly_get_image(depassembly);
		image = mono_assembly_get_image(assembly);

		baseClassName = "MonoBehavior";
	}
	MonoImage* CLRLoader::GetDepImage() const
	{
		return depimage;
	}
	std::vector<MonoClass*>CLRLoader::FindDerivedClasses()
	{
		std::vector<MonoClass*> result;
		const MonoTableInfo* typeDefTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int rows = mono_table_info_get_rows(typeDefTable);

		for (int i = 0; i < rows; ++i) {

			uint32_t token = 0x02000000 | (i + 1);
			//(i + 1) << MONO_TYPEDEF_OR_REF_BITS
			MonoClass* klass = mono_class_get(image, token);
			if (IsSubclassOf(klass, baseClassName)) {
				result.push_back(klass);
			}
		}
		return result;
	}

	bool CLRLoader::IsSubclassOf(MonoClass* klass, const char* baseName)
	{
		MonoClass* parent = mono_class_get_parent(klass);
		while (parent != nullptr) {
			if (strcmp(mono_class_get_name(parent), baseName) == 0)
				return true;
			parent = mono_class_get_parent(parent);
		}
		return false;

	}
	ScriptableEntity::ScriptableEntity(const char* assemblyPath, const char* baseClass,int id, Scene* scene)
		:loader(assemblyPath, baseClass), engine(scene)
	{
		engine.Register();
		engine.RegisterComponents(loader.GetDepImage());
		auto classes= loader.FindDerivedClasses();
		for (auto klass : classes)
		{
			MonoObject* obj = mono_object_new(loader.domain, klass);
			mono_runtime_object_init(obj); // 调用构造函数
			//调用基类构造函数
			MonoClass* plass = mono_class_get_parent(klass);
			MonoMethod* pmethod = mono_class_get_method_from_name(plass, ".ctor", 1);
			if (pmethod)
			{
				void* args[1];
				args[0] = &id;
				mono_runtime_invoke(pmethod, obj, args, NULL);
			}
			objects.push_back(obj);
		}

	}

	void ScriptableEntity::OnCreate()
	{
		for (auto obj : objects) {
			MonoClass* klass = mono_object_get_class(obj);
			MonoMethod* method = mono_class_get_method_from_name(
				klass, "OnCreate", 0);

			if (method) {
				mono_runtime_invoke(method, obj, NULL, NULL);
			}
		}
	}
	void ScriptableEntity::OnDestroy()
	{

		for (auto obj : objects) {
			MonoClass* klass = mono_object_get_class(obj);
			MonoMethod* method = mono_class_get_method_from_name(
				klass, "OnDestroy", 0);
			if (method) {
				mono_runtime_invoke(method, obj, NULL, NULL);
			}
		}
	}
	void ScriptableEntity::OnUpdate(float t)
	{

		for (auto obj : objects) {
			MonoClass* klass = mono_object_get_class(obj);
			MonoMethod* method = mono_class_get_method_from_name(
				klass, "OnUpdate",1);

			if (method) {
				void* args[1];
				args[0] = &t;
				mono_runtime_invoke(method, obj,args, NULL);
			}
		}
	}
}