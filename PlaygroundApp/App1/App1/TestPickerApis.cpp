#include "pch.h"
#include "TestPickerApis.h"

#include <winrt/Windows.Storage.Pickers.h>
#include <microsoft.ui.xaml.window.h>
#include <winrt/microsoft.ui.interop.h>
#include <ShObjIdl_core.h>
#include <atlbase.h>

namespace Playground
{
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> TestPickerApis::OpenFilePickerWinRTAsync(HWND hWnd)
    {
        auto picker = winrt::Windows::Storage::Pickers::FileOpenPicker();
        picker.ViewMode(winrt::Windows::Storage::Pickers::PickerViewMode::Thumbnail);
        picker.SuggestedStartLocation(winrt::Windows::Storage::Pickers::PickerLocationId::PicturesLibrary);
        picker.FileTypeFilter().Append(L".jpg");
        picker.FileTypeFilter().Append(L".jpeg");
        picker.FileTypeFilter().Append(L".png");
        picker.FileTypeFilter().Append(L".heic");

        picker.as<IInitializeWithWindow>()->Initialize(hWnd);

        auto file = co_await picker.PickSingleFileAsync();
        co_return file;
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> TestPickerApis::OpenSavePickerWinRTAsync(HWND hWnd)
    {
        auto picker = winrt::Windows::Storage::Pickers::FileSavePicker();
        picker.SuggestedStartLocation(WS::Pickers::PickerLocationId::PicturesLibrary);
        picker.SuggestedFileName(L"myFile");
        picker.FileTypeChoices().Insert(L"JPG", winrt::single_threaded_vector<winrt::hstring>({L".jpg"}));

        picker.as<IInitializeWithWindow>()->Initialize(hWnd);
        auto file = co_await picker.PickSaveFileAsync();
        co_return file;
    }

	void TestPickerApis::OpenSaveFileDialogWin32(HWND wHandle, std::string filePath)
	{
        LPSTR lpstrPath = const_cast<LPSTR>(filePath.c_str());
        SaveDialogWin32(wHandle, lpstrPath);
	}

    void TestPickerApis::SaveDialogWin32(HWND wHandle, LPSTR path)
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

    std::unique_ptr<COMDLG_FILTERSPEC[]> CreateFILTERSPEC(winrt::array_view<const winrt::hstring> names, winrt::array_view<const winrt::hstring> specs)
    {
        auto filterSpecs = std::make_unique<COMDLG_FILTERSPEC[]>(names.size());
        COMDLG_FILTERSPEC* filterSpec = filterSpecs.get();
        if (names.size() == specs.size())
        {
            for (uint32_t i = 0; i < specs.size(); i++)
            {
                filterSpec->pszName = names.at(i).c_str();
                filterSpec->pszSpec = specs.at(i).c_str();
                ++filterSpec;
            }
        }

        return filterSpecs;
    }

    winrt::Windows::Foundation::IAsyncAction TestPickerApis::OpenSaveFileDialogComShellAsync()
    {
        // Resume to a worker thread to avoid Re-entrancy issues if calling COM from the Xaml UI thread.
        co_await winrt::resume_background();

        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(hr))
        {
            co_return;
        }

        winrt::hstring defaultFolder = L"C:\\Users\\robsonpontin\\Downloads";

        CComPtr<IShellItem> pDefaultFolder;
        hr = SHCreateItemFromParsingName(defaultFolder.c_str(), NULL, IID_PPV_ARGS(&pDefaultFolder));

        if (SUCCEEDED(hr))
        {
            CComPtr<IFileSaveDialog> pDialog;
            // Create the FileOpenDialog object.
            hr = pDialog.CoCreateInstance(CLSID_FileSaveDialog);
            if (SUCCEEDED(hr))
            {
                FILEOPENDIALOGOPTIONS dialogOptions;
                if (SUCCEEDED(pDialog->GetOptions(&dialogOptions)))
                {
                    dialogOptions |= FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT | FOS_FORCEFILESYSTEM;
                    hr = pDialog->SetOptions(dialogOptions);
                }

                auto filterResult = CreateFILTERSPEC({ L"JPG"}, {L"*.jpg"});

                HRESULT hr1 = pDialog->SetDefaultFolder(pDefaultFolder);
                HRESULT hr2 = pDialog->SetFileName(L"myFile");
                HRESULT hr3 = pDialog->SetFileTypes(1, filterResult.get());
                HRESULT hr4 = pDialog->SetDefaultExtension(L".jpg");

                if (SUCCEEDED(hr) && SUCCEEDED(hr1) && SUCCEEDED(hr2) && SUCCEEDED(hr3) && SUCCEEDED(hr4))
                {
                    // Show the Open dialog box.
                    hr = pDialog->Show(NULL);

                    // Get the file name from the dialog box.
                    if (SUCCEEDED(hr))
                    {
                        CComPtr<IShellItem> pResultItem;
                        hr = pDialog->GetResult(&pResultItem);
                        if (SUCCEEDED(hr))
                        {
                            CComHeapPtr<wchar_t> pPath;
                            hr = pResultItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);

                            // Display the file name to the user.
                            if (SUCCEEDED(hr))
                            {
                                auto retPath = std::wstring(pPath);
                            }
                        }
                    }
                }
            }
        }
    }
}
