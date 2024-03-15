#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "winrt/Windows.ApplicationModel.h"
#include <winrt/microsoft.ui.interop.h>
#include <microsoft.ui.xaml.window.h>

#include "shellapi.h"

#include "FullTrustAppLauncher.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Multiprocess::App::implementation
{
	// Forward declaration
	HWND GetWindowHandle(Microsoft::UI::Xaml::Window const& window);
	void LaunchFromShell(Microsoft::UI::Xaml::Window const& window);

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::btnLaunchService_Click(IInspectable const&, RoutedEventArgs const&)
    {
        FullTrustAppLauncher::LaunchService();
    }

	void MainWindow::btnLaunchServiceWithShell_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{
		auto window = this->try_as<Window>();
		LaunchFromShell(window);
	}

	// ## Utility calls ##

	HWND GetWindowHandle(Microsoft::UI::Xaml::Window const& window)
	{
		try
		{
			auto windowNative{ window.try_as<::IWindowNative>() };
			winrt::check_bool(windowNative);

			HWND hWnd{ 0 };
			windowNative->get_WindowHandle(&hWnd);

			return hWnd;
		}
		catch (winrt::hresult_error const& error)
		{
			return { 0 };
		}
	}

	void LaunchFromShell(Microsoft::UI::Xaml::Window const& window)
	{
		HWND windowHandle = GetWindowHandle(window);

		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		auto path = std::wstring(buffer).substr(0, pos);

		path = path + L"\\Multiprocess.Service\\Multiprocess.Service.exe";

		ShellExecute(
			windowHandle,
			L"open",
			path.c_str(),
			NULL,
			NULL,
			SW_SHOWNORMAL);
	}
}
