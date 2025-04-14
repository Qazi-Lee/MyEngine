using System.Numerics;
using System.Runtime;
using System.Runtime.CompilerServices;
namespace ScriptEngine
{
    public class MonoBehavior:Entity
    {
        protected IntPtr nativeWrapperPtr;

        //public int ID = 0;

        //public TagCompoennt tag;
        //public TransformComponent transform;


        public MonoBehavior()
        {
            transform = new TransformComponent();
            tag = new TagComponent();
            sceneManager = new SceneManager();
        }

        public MonoBehavior(int id)
        {
            sceneManager = new SceneManager();
            this.ID = id;
            transform = new TransformComponent
            {
                EntityID = ID
            };
            tag = new TagComponent
            {
                EntityID = ID
            };
            
        }

        public virtual void OnCreate()
        {

        }
        public virtual void OnDestroy()
        {

        }

        public virtual void OnUpdate(float t)
        {


        }

    }


}
