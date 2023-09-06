#include "pch.h"

#include "App2.xaml.h"

#include <wil/resource.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Windows.System.h>

// prototype functions
std::vector<std::wstring> split_string(const std::wstring& wstr, char delimiter);

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    /* The data will be sent like "/InvokerPRAID: App /FileActivation" so it
     * needs to breakdown into App and Param */
    std::wstring cmdLineStr(lpCmdline);
    auto result = split_string(cmdLineStr, '/');
    winrt::hstring cmdLineHstr = L"";
    if (result.size() >= 3)
    {
        cmdLineHstr = winrt::to_hstring(result.at(2).c_str());
    }

    winrt::Microsoft::UI::Xaml::Application::Start([cmdLineHstr](auto&&)
        {
            ::winrt::make<winrt::WinUi3App::implementation::App2>(cmdLineHstr);
        });

    return 0;
}

std::vector<std::wstring> split_string(const std::wstring& wstr, char delimiter)
{
    std::vector<std::wstring> result;
    size_t start = 0;
    size_t end = wstr.find(delimiter);

    while (end != std::string::npos)
    {
        result.push_back(wstr.substr(start, end - start));
        start = end + 1;
        end = wstr.find(delimiter, start);
    }
    result.push_back(wstr.substr(start, end));
    return result;
}