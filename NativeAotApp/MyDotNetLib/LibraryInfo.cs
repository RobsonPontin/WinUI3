using System.Runtime.InteropServices;

namespace MyDotNetLib
{
    /// <summary>
    /// The managed type which will be exported to native code.
    /// </summary>
    /// <remarks>
    /// Ensure to keep the <see cref="StructLayoutAttribute"/> which will guarantee the <see cref="Marshal.AllocHGlobal"/>
    /// will work properly.
    /// </remarks>
    [StructLayout(LayoutKind.Sequential)]
    public class LibraryInfo
    {
        public string DotNetVersion { get; set; } = string.Empty;

        public DotNetType DotNetType { get; set; }
    }

    public enum DotNetType
    {
        Framework,
        Standard,
        Core
    }
}
