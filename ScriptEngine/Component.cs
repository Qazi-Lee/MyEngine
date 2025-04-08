using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ScriptEngine
{
    public class Component
    {
        public int EntityID=0;
    }

    public class TransformComponent:Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(EntityID, out Vector3 translation);
                return translation;
            }
            set 
            {
                InternalCalls.TransformComponent_SetTranslation(EntityID, ref value);
            }

        }

    }

    public class TagCompoennt:Component
    {
        public string tag
        {
            get
            {
                InternalCalls.TagComponent_GetTag(EntityID, out string tag);
                return tag;
            }
            set
            {
               InternalCalls.TagComponent_SetTag(EntityID, ref value);
            }

        }

    }
}
