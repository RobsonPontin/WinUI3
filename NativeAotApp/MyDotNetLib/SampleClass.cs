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

        [UnmanagedCallersOnly(EntryPoint = "getNameFromList")]
        public static IntPtr GetNameFromList()
        {
            // Simple test to see how compiling this library to NativeAOT will handle Collection and Linq .NET dependencies
            List<string> myNames = new List<string>
            {
                "John",
                "Rick",
                "Ana",
                "Lisa"                
            };

            var nameFound = myNames.Where<string>(x => x == "Ana").First();

            // Assign pointer of the concatenated string to sumPointer
            IntPtr myName = Marshal.StringToCoTaskMemAnsi(nameFound);

            return myName;
        }
    }
}