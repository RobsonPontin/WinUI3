#pragma once

#include "LibraryApiExport.h"

namespace ContainerForCs::AppLib
{
    struct AppFactory
    {
        ContainerForCs_API static winrt::Microsoft::UI::Xaml::Application CreateApp();
    };
}