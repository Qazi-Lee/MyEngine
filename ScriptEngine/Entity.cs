using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ScriptEngine
{
    public class Entity
    {
        public int ID = 0;
        public TagComponent tag;
        public TransformComponent transform;
        public SceneManager sceneManager;

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }
        public T GetComponent<T>() where T:Component, new ()
         {
            if (!HasComponent<T>())
                return null;

            T component = new T() { EntityID = this.ID };
            return component;
        }
    }
}
