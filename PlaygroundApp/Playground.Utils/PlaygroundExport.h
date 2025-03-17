#pragma once

#ifdef PLAYGROUND_UTILS_EXPORTING
#define PLAYGROUND_UTILS_API __declspec(dllexport)
#else
#define PLAYGROUND_UTILS_API __declspec(dllimport)
#endif