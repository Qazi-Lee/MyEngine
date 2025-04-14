using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ScriptEngine
{
    public class SceneManager
    {
        public uint index
        {
            get
            {
                InternalCalls.SceneManager_GetIndex(out uint index);
                return index;
            }
        }

        public uint size
        {
            get
            {
                InternalCalls.SceneManager_GetSize(out uint size);
                return size;
            }
        }

        public SceneManager() { }

        public void SetIndex(uint index)
        {
            InternalCalls.SceneManager_SetIndex(ref index);
        }
    }
}
