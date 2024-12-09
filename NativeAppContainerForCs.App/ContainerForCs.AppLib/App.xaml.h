#pragma once

#include "..\ContainerForCs.AppLib\Generated Files\App.xaml.g.h"

#include "LibraryApiExport.h"

namespace winrt::ContainerForCs::AppLib::implementation
{
    struct App : AppT<App>
    {
        ContainerForCs_API App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
