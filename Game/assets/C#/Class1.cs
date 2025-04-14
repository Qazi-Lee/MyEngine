using System;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using ScriptEngine;

public class Derived : MonoBehavior
{
    public override void OnCreate()
    {
       //  Console.WriteLine("OnCreate");
    }
    public override void OnDestroy()
    {
     //   Console.WriteLine("OnDestroy");
    }
    public override void OnUpdate(float t)
    {
        Vector3 translation = transform.Translation;
        //Console.WriteLine("Get");
        //Vector3 translation = Vector3.Zero;
        float speed = 0.1f;
        if (Input.IsKeyDown(KeyCode.A))
        {
            // Console.WriteLine("A");
            translation.x -= speed;

        }
        if (Input.IsKeyDown(KeyCode.D))
        {
            // Console.WriteLine("D");
            translation.x += speed;
        }
        if (Input.IsKeyDown(KeyCode.W))
        {
            // Console.WriteLine("W");
            translation.y += speed;
        }
        if (Input.IsKeyDown(KeyCode.S))
        {
            // Console.WriteLine("S");
            translation.y -= speed;
        }
        transform.Translation = translation;

        Vector2 v = Vector2.Zero;

        if (Input.IsKeyDown(KeyCode.Left))
        {
            v.x -= 100 * t;
        }
        if (Input.IsKeyDown(KeyCode.Right))
        {
            v.x = 100 * t;
        }
        if (GetComponent<Rigidbody2DComponent>() != null)
        {
            GetComponent<Rigidbody2DComponent>().LinearVelocity = v;
        }
        if (Input.IsKeyDown(KeyCode.Tab))
        {
           // Console.WriteLine("TAB");
           // Console.WriteLine($"index:{sceneManager.index},size:{sceneManager.size}");
            if (sceneManager.index < sceneManager.size - 1)
            {
                uint index = sceneManager.index;
                sceneManager.SetIndex(index + 1);
            }
        }

    }
}

