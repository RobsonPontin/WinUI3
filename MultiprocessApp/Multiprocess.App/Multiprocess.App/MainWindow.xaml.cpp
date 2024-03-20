#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "winrt/Windows.ApplicationModel.h"
#include <winrt/microsoft.ui.interop.h>
#include <microsoft.ui.xaml.window.h>

#include "shellapi.h"

#include "AppLauncher.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;


namespace winrt::Multiprocess::App::implementation
{
	const winrt::hstring MULTIPROCESS_APP_PACKAGE_NAME{ L"Rpontin.Winui.MultiprocessApp_9yd5akztwvwhp" };
	const winrt::hstring MULTIPROCESS_APP_LAUNCH_SPARE_PROTOCOL{ L"wp-launch:spare" };
	const winrt::hstring MULTIPROCESS_APP_LAUNCH_MAIN_PROTOCOL{ L"wp-launch:main" };

	// Forward declaration
	HWND GetWindowHandle(Microsoft::UI::Xaml::Window const& window);
	void LaunchFromShell(Microsoft::UI::Xaml::Window const& window);

	MainWindow::MainWindow()
	{
		m_processManager = std::make_shared<::Multiprocess::Core::ProcessManager>();
	}

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
		m_processManager->CreateBackgroundService();
    }

	void MainWindow::btnLaunchServiceWithShell_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto window = this->try_as<Window>();
		LaunchFromShell(window);
	}

	void MainWindow::btnGetProcessesInfo_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto runningProcesses = m_processManager->RunningProcesses();
		auto first_runningProcess = runningProcesses->begin();
		auto processInfo = first_runningProcess->get()->ProcessInfo();
		auto startInfo = first_runningProcess->get()->StartInfo();

		tbProcessesInfo().Text(L"test.......");
	}

	winrt::Windows::Foundation::IAsyncAction MainWindow::btnRedirectMain_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		co_await ::Multiprocess::AppLauncher::ProtocolLaunchURIAsync(
			MULTIPROCESS_APP_PACKAGE_NAME, 
			MULTIPROCESS_APP_LAUNCH_MAIN_PROTOCOL, 
			L"none");
	}

	winrt::Windows::Foundation::IAsyncAction MainWindow::btnRedirectSpare_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		co_await ::Multiprocess::AppLauncher::ProtocolLaunchURIAsync(
			MULTIPROCESS_APP_PACKAGE_NAME,
			MULTIPROCESS_APP_LAUNCH_SPARE_PROTOCOL,
			L"none");
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
		catch (winrt::hresult_error const&)
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
