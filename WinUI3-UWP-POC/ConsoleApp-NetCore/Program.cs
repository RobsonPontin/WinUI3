using System;

namespace ConsoleApp_NetCore
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            var math = new SimpleMathComponent.SimpleMath();
            var result = math.add(10, 10);
        }
    }
}
