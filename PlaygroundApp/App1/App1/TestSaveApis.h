#pragma once

#include <winrt/Windows.Storage.h>

namespace Playground
{
	struct TestSaveApis
	{
		TestSaveApis() = default;

		winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> OpenFilePickerWinRTAsync(HWND hWnd);
		void OpenSaveFileDialogWin32(HWND wHandle, std::string filePath);

	private:
		void SaveDialogWin32(HWND wHandle, LPSTR path);
	};
}
