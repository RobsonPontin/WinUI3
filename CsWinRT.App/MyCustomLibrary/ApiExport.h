#pragma once

#ifdef MYCUSTOMLIBRARY_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif