using System;
using System.Runtime.InteropServices;
using System.Text.Json;
using System.Text;
using System.Reflection;

namespace MyDotNetLib
{
    public class Exports
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

        [UnmanagedCallersOnly(EntryPoint = "getLibraryInfo")]
        public static nint GetLibraryInfo()
        {
            LibraryInfo libInfo = new()
            {
                DotNetVersion = "7",
                DotNetType = DotNetType.Core
            };

            // Allocate unmanaged memory block
            var bufferPtr = Marshal.AllocHGlobal(Marshal.SizeOf<LibraryInfo>());

            Marshal.StructureToPtr(libInfo, bufferPtr, false);

            return bufferPtr;
        }
    }
}