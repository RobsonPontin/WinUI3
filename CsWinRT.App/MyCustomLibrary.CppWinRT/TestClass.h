#pragma once

#include "TestClass.g.h"

namespace winrt::MyCustomLibrary::implementation
{
    struct TestClass : TestClassT<TestClass>
    {
        TestClass() = default;

        static int32_t AddAndGetResult();
    };
}

namespace winrt::MyCustomLibrary::factory_implementation
{
    struct TestClass : TestClassT<TestClass, implementation::TestClass>
    {
    };
}
