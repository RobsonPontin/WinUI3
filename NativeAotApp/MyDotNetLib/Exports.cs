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

        #region JSON Serialization

        private static readonly LibraryInfo LibraryInformation = new()
        {
            DotNetVersion = "7",
            DotNetType = DotNetType.Core
        };

        [UnmanagedCallersOnly(EntryPoint = "getSerializedLibraryInfo")]
        public static nint GetSerializedLibraryInfo()
        {
            var serializedObject = JsonSerializer.Serialize(LibraryInformation, JsonContext.Default.LibraryInfo);
            var length = Encoding.UTF8.GetByteCount(serializedObject);

            // Allocate unmanaged memory block
            var bufferPtr = Marshal.AllocHGlobal(length + 1); // + 1 is due to the null terminator.

            var buffer = StringToBuffer(bufferPtr, length);

            Encoding.UTF8.GetBytes(serializedObject, buffer);

            // Add null terminator
            Marshal.WriteByte(bufferPtr, length, 0);

            return bufferPtr;
        }

        /// <summary>
        /// Get a buffer span with unsafe code / pointers.
        /// </summary>
        private static unsafe Span<byte> StringToBuffer(nint bufferPtr, int length)
        {
            // Write serialized object directly to unmanaged memory
            return new Span<byte>(bufferPtr.ToPointer(), length);
        }

        #endregion
    }
}