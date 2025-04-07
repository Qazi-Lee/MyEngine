using System.Runtime;
using System.Runtime.CompilerServices;
namespace ScriptEngine
{
    public class MonoBehavior
    {
        protected IntPtr nativeWrapperPtr;

        public int ID = 0;

        public TagCompoennt tag;
        public TransformComponent transform;


        public MonoBehavior()
        {
            transform = new TransformComponent();
            tag = new TagCompoennt();
        }

        public MonoBehavior(int id)
        {
            this.ID = id;
            transform = new TransformComponent
            {
                EntityID = ID
            };
            tag = new TagCompoennt
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
