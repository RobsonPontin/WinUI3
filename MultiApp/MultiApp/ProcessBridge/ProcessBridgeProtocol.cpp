#include "pch.h"
#include "ProcessBridgeProtocol.h"

namespace ProcessBridge::Protocol
{
    std::wstring GetOwnDirectory()
    {
        wchar_t self[MAX_PATH]{};
        DWORD len = ::GetModuleFileNameW(nullptr, self, MAX_PATH);
        if (len == 0 || len == MAX_PATH)
        {
            return {};
        }
        std::wstring path{ self, len };
        size_t slash = path.find_last_of(L"\\/");
        if (slash == std::wstring::npos)
        {
            return {};
        }
        path.resize(slash + 1);
        return path;
    }

    bool TryGetSwitchValue(int argc, LPWSTR* argv,
                           std::wstring_view prefix,
                           std::wstring& out_value)
    {
        for (int i = 1; i < argc; ++i)
        {
            std::wstring_view arg{ argv[i] };
            if (arg.size() >= prefix.size() &&
                arg.compare(0, prefix.size(), prefix) == 0)
            {
                out_value.assign(arg.substr(prefix.size()));
                return true;
            }
        }
        return false;
    }
}
