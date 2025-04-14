using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
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

    public class TagComponent:Component
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

    public class Rigidbody2DComponent : Component
    {
        public enum BodyType { Static = 0, Dynamic, Kinematic }

        //线速度
        public Vector2 LinearVelocity
        {
            get
            {
                InternalCalls.Rigidbody2DComponent_GetLinearVelocity(EntityID, out Vector2 velocity);
                return velocity;
            }
            set
            {
                InternalCalls.Rigidbody2DComponent_SetLinearVelocity(EntityID, ref value);
            }
        }
        //类型
        public BodyType Type
        {
            get => InternalCalls.Rigidbody2DComponent_GetType(EntityID);
            set => InternalCalls.Rigidbody2DComponent_SetType(EntityID, value);
        }
        //冲量
        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(EntityID, ref impulse, ref worldPosition, wake);
        }
        
        //冲量到中心
        public void ApplyLinearImpulse(Vector2 impulse, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(EntityID, ref impulse, wake);
        }

    }
}
