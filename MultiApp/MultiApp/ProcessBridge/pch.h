#pragma once

#include <windows.h>
#include <string>
#include <string_view>
#include <optional>
#include <functional>
#include <memory>
#include <atomic>
#include <thread>
#include <cstdint>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime
