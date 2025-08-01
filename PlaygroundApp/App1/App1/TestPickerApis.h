#pragma once

#include <winrt/Windows.Storage.h>

namespace Playground
{
	struct TestPickerApis
	{
		TestPickerApis() = default;

		winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> OpenFilePickerWinRTAsync(HWND hWnd);
		winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> OpenSavePickerWinRTAsync(HWND hWnd);
		void OpenSaveFileDialogWin32(HWND wHandle, std::string filePath);
		winrt::Windows::Foundation::IAsyncAction OpenSaveFileDialogComShellAsync();

	private:
		void SaveDialogWin32(HWND wHandle, LPSTR path);
	};
}
