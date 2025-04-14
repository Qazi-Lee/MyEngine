using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ScriptEngine
{
    public static class InternalCalls
    {
        #region SceneManager
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]

        public extern static void SceneManager_GetIndex(out uint index);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]

        public extern static void SceneManager_GetSize(out uint size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]

        public extern static void SceneManager_SetIndex(ref uint size);
        #endregion

        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
         [DllImport("__Internal")]
         public extern static bool Input_IsKeyDown(KeyCode keycode);
        #endregion

        #region Entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]

        public extern static bool Entity_HasComponent(int id, Type componentType);
        #endregion

        #region TransformComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static void TransformComponent_GetTranslation(int id,out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static void TransformComponent_SetTranslation(int id ,ref Vector3 translation);
        #endregion

        #region TagComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]

        public extern static void TagComponent_GetTag(int id, out string tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static void TagComponent_SetTag(int id ,ref string tag);
        #endregion

        #region Rigidbody2DComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static void Rigidbody2DComponent_ApplyLinearImpulse(int entityID, ref Vector2 impulse, ref Vector2 point, bool wake);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(int entityID, ref Vector2 impulse, bool wake);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static void Rigidbody2DComponent_GetLinearVelocity(int entityID, out Vector2 linearVelocity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static void Rigidbody2DComponent_SetLinearVelocity(int entityID, ref Vector2 linearVelocity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static Rigidbody2DComponent.BodyType Rigidbody2DComponent_GetType(int entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        [DllImport("__Internal")]
        public extern static void Rigidbody2DComponent_SetType(int entityID, Rigidbody2DComponent.BodyType type);

        #endregion

    }
}
