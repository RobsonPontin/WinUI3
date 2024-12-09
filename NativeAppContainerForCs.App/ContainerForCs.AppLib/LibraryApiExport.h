#pragma once

#ifdef LIBRARY_API_EXPORTING
#define ContainerForCs_API __declspec(dllexport)
#else
#define ContainerForCs_API __declspec(dllimport)
#endif