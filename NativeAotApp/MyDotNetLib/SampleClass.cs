using System;
using System.Runtime.InteropServices;

namespace MyDotNetLib
{
    public class SampleClass
    {
        [UnmanagedCallersOnly(EntryPoint = "getValue")]
        public static int GetValue()
        {
            return 7;
        }

        [UnmanagedCallersOnly(EntryPoint = "getName")]
        public static IntPtr GetName()
        {
            // Assign pointer of the concatenated string to sumPointer
            IntPtr myName = Marshal.StringToCoTaskMemAnsi("MyName");

            return myName;
        }
    }
}