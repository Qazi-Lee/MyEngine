using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ScriptEngine
{
    public struct Vector3
    {
        public float x, y, z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3(float scalar)
        {
            x = scalar;
            y = scalar;
            z = scalar;
        }
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
        }
    }
}
