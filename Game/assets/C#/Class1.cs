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
    }
}

