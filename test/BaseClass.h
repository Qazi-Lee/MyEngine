#pragma once
#include"iostream"

#include"mono/jit/jit.h"
#include"mono/metadata/assembly.h"
#include <mono/metadata/debug-helpers.h>
#include"mono/metadata/class.h"
#include <vector>
#define MONO_TYPEDEF_OR_REF_BITS 0x03
class BaseClass {
public:
    virtual void Display() { 
        std::cout << "BaseClass Display" << std::endl;
    }
    virtual void Method()=0;
    virtual ~BaseClass() = default;
};
class ManagedWrapper : public BaseClass {
public:
    MonoObject* managedObject;
    MonoMethod* managedMethod;
    ManagedWrapper(MonoObject* obj)
        : managedObject(obj) {}
    ManagedWrapper(MonoObject* obj, MonoMethod* method)
        : managedObject(obj), managedMethod(method) {}
    virtual void Method() override {
        MonoObject* exception = nullptr;
        mono_runtime_invoke(managedMethod, managedObject, nullptr, &exception);
        if (exception) {
            // 处理异常
        }
    }
};
class CLRLoader {
    MonoDomain* domain;
    MonoImage* image;
    const char* baseClassName;

    // 递归检查类继承关系
    bool IsSubclassOf(MonoClass* klass, const char* baseName) {
        MonoClass* parent = mono_class_get_parent(klass);
        while (parent != nullptr) {
            if (strcmp(mono_class_get_name(parent), baseName) == 0)
                return true;
            parent = mono_class_get_parent(parent);
        }
        return false;
    }

public:
    CLRLoader(const char* assemblyPath, const char* baseClass) {
        mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
        domain = mono_jit_init("MyDomain");
        MonoAssembly* assembly = mono_domain_assembly_open(domain, assemblyPath);
        image = mono_assembly_get_image(assembly);
        baseClassName = baseClass;
    }

    // 发现所有派生类
    std::vector<MonoClass*> FindDerivedClasses() {
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

    // 执行所有派生类方法
    void ExecuteAll() {
        auto classes = FindDerivedClasses();
        for (auto klass : classes) {
            MonoObject* obj = mono_object_new(domain, klass);
            mono_runtime_object_init(obj); // 调用构造函数

            MonoMethod* method = mono_class_get_method_from_name(
                klass, "Method", 0);

            if (method) {
                mono_runtime_invoke(method, obj, NULL, NULL);
            }
        }
    }

    ~CLRLoader() {
        mono_jit_cleanup(domain);
    }
};
