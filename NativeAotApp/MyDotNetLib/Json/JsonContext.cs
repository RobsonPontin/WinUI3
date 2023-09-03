using System;
using System.Runtime.InteropServices;
using System.Text.Json.Serialization;

namespace MyDotNetLib
{
    [JsonSerializable(typeof(LibraryInfo), GenerationMode = JsonSourceGenerationMode.Metadata)]
    internal partial class JsonContext : JsonSerializerContext
    {
    }

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
