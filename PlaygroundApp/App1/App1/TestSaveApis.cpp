#include "pch.h"
#include "TestSaveApis.h"

#include <winrt/Windows.Storage.Pickers.h>
#include <microsoft.ui.xaml.window.h>
#include <winrt/microsoft.ui.interop.h>
#include <ShObjIdl_core.h>

namespace Playground
{
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> TestSaveApis::OpenFilePickerWinRTAsync(HWND hWnd)
    {
        auto picker = winrt::Windows::Storage::Pickers::FileOpenPicker();
        picker.ViewMode(winrt::Windows::Storage::Pickers::PickerViewMode::Thumbnail);
        picker.SuggestedStartLocation(winrt::Windows::Storage::Pickers::PickerLocationId::PicturesLibrary);
        picker.FileTypeFilter().Append(L".jpg");
        picker.FileTypeFilter().Append(L".jpeg");
        picker.FileTypeFilter().Append(L".png");

        picker.as<IInitializeWithWindow>()->Initialize(hWnd);

        auto file = co_await picker.PickSingleFileAsync();
        co_return file;
    }

	void TestSaveApis::OpenSaveFileDialogWin32(HWND wHandle, std::string filePath)
	{
        LPSTR lpstrPath = const_cast<LPSTR>(filePath.c_str());
        SaveDialogWin32(wHandle, lpstrPath);
	}

    void TestSaveApis::SaveDialogWin32(HWND wHandle, LPSTR path)
    {
        OPENFILENAMEA ofn = { 0 };

        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.hwndOwner = wHandle;
        //  The last string in the buffer must be terminated by two NULL characters.
        ofn.lpstrFilter = ".jpg\0*.*\0\0";
        ofn.lpstrFile = path;
        ofn.lpstrDefExt = ".jpg";
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Rob save file test";
        ofn.Flags = OFN_OVERWRITEPROMPT; // OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST |

        if (GetSaveFileNameA(&ofn))
        {
            // User wants to save, now handle saving of the file
        }
        else
        {
            // User cancelled saving, handle this case.
        }

        return;
    }
}
