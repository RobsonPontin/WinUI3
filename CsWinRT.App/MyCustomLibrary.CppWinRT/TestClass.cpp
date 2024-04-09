#include "pch.h"

#include "TestClass.h"
#if __has_include("TestClass.g.cpp")
#include "TestClass.g.cpp"
#endif

#include <MyCustomLibrary/TestClass.h>

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::MyCustomLibrary::implementation
{
    int32_t TestClass::AddAndGetResult()
    {
        return ::MyCustomLibrary::TestClass::AddAndGetResult();
    }
}
