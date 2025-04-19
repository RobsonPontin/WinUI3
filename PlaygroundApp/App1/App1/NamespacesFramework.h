#pragma once

// Forward declaration
namespace winrt {
    namespace Windows {
        namespace ApplicationModel {
            namespace Activation {}
            namespace Core {}
            namespace DataTransfer {}
            namespace Resources {}
        }
        namespace Data {
            namespace Json {}
        }
        namespace Devices {
            namespace Input {}
            namespace Geolocation {}
        }
        namespace Foundation {
            namespace Collections {}
            namespace Numerics {}
        }
        namespace Graphics {
            namespace Imaging {}
            namespace Display {}
            namespace Printing {}
        }

        namespace Media {
            namespace Core {}
            namespace MediaProperties {}
            namespace Playback {}
        }

        namespace Security {
            namespace Authentication {
                namespace Web {
                    namespace Core {}
                }
            }
            namespace Credentials {}
        }

        namespace Storage {
            namespace Streams {}
        }
        namespace System {}

        namespace Web {
            namespace Http {
                namespace Filters {}
            }
        }

        namespace Management {
            namespace Deployment {}
        }

        namespace Networking
        {
            namespace Connectivity {}
        }

        namespace UI {
            struct Color;

            namespace ApplicationSettings {}
            namespace Core {}
            namespace Input {}
            namespace ViewManagement {}
            namespace Xaml {
                namespace Interop {}
            }

        }
        namespace Globalization {}
    }
    namespace Microsoft {
        namespace Windows {
            namespace ApplicationModel {
                namespace Resources {}
            }
            namespace AppLifecycle {}
        }
        namespace Web {}
        namespace UI {
            namespace Input {}
            namespace Dispatching {}
            namespace Windowing {}

            namespace Xaml {
                namespace Controls {}
                namespace Data {}
                namespace Media {}
                namespace Interop {}
            }
            namespace Composition {}
        }
        namespace Graphics {
            namespace Display {}
        }
    }
}

namespace MUX = winrt::Microsoft::UI::Xaml;
namespace MUXC = winrt::Microsoft::UI::Xaml::Controls;
namespace WAM = winrt::Windows::ApplicationModel;
namespace WAMA = winrt::Windows::ApplicationModel::Activation;
namespace WAMC = winrt::Windows::ApplicationModel::Core;
namespace WAMDT = winrt::Windows::ApplicationModel::DataTransfer;
namespace WD = winrt::Windows::Data;
namespace WF = winrt::Windows::Foundation;
namespace WFC = winrt::Windows::Foundation::Collections;
namespace WFN = winrt::Windows::Foundation::Numerics;
namespace WG = winrt::Windows::Graphics;
namespace WGrP = winrt::Windows::Graphics::Printing;
namespace WGI = winrt::Windows::Graphics::Imaging;
namespace WM = winrt::Windows::Media;
namespace WMC = winrt::Windows::Media::Core;
namespace WMP = winrt::Windows::Media::Playback;
namespace WMMP = winrt::Windows::Media::MediaProperties;
namespace WS = winrt::Windows::Storage;
namespace WSS = winrt::Windows::Storage::Streams;
namespace WST = winrt::Windows::System;
namespace WU = winrt::Windows::UI;
namespace WUA = winrt::Windows::UI::ApplicationSettings;
namespace WUCR = winrt::Windows::UI::Core;
namespace WUVM = winrt::Windows::UI::ViewManagement;
namespace WWH = winrt::Windows::Web::Http;
namespace WWHF = winrt::Windows::Web::Http::Filters;
namespace WDG = winrt::Windows::Devices::Geolocation;
namespace WMD = winrt::Windows::Management::Deployment;
namespace WNC = winrt::Windows::Networking::Connectivity;
namespace WSC = winrt::Windows::Security::Credentials;
namespace WSAWC = winrt::Windows::Security::Authentication::Web::Core;
// This only applies for WUXIT.TypeName. Only this struct needs to use the Windows.UI.Xaml.Interop.TypeName definition even for WinAppSDK
// (the rest of needs to use Microsoft.UI.Xaml.Interop). Recommended by the WinAppSDK team.
namespace WUXIT = winrt::Windows::UI::Xaml::Interop;

// Keep namespace aliases the same to reduce code changes
namespace MU = winrt::Microsoft::UI;
namespace MUD = winrt::Microsoft::UI::Dispatching;
namespace MUW = winrt::Microsoft::UI::Windowing;
namespace MWAL = winrt::Microsoft::Windows::AppLifecycle;
namespace WAMR = winrt::Microsoft::Windows::ApplicationModel::Resources;
namespace WDI = winrt::Microsoft::UI::Input; // WDI and WUI are merged into Microsoft::UI::Input
namespace WGD = winrt::Microsoft::Graphics::Display;
namespace WUC = winrt::Microsoft::UI::Composition;
namespace WUI = winrt::Microsoft::UI::Input; // WDI and WUI are merged into Microsoft::UI::Input
namespace WUX = winrt::Microsoft::UI::Xaml;
namespace WUXC = winrt::Microsoft::UI::Xaml::Controls;
namespace WUXD = winrt::Microsoft::UI::Xaml::Data;
namespace WUXM = winrt::Microsoft::UI::Xaml::Media;
namespace WUXI = winrt::Microsoft::UI::Xaml::Interop;
namespace WGL = winrt::Windows::Globalization;
namespace WUIColor = winrt::Microsoft::UI; // WUIColor is just for ColorHelper. In UWP it's under namespace Windows.UI, in WinAppSDK it's under Microsoft.UI.
namespace MW = winrt::Microsoft::Web;