using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ScriptEngine
{
    public static class InternalCalls
    {
        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
         [DllImport("__Internal")]
         public extern static bool Input_IsKeyDown(KeyCode keycode);
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

    }
}
