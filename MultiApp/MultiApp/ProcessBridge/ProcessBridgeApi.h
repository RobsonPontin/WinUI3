#pragma once

// DLL export/import macro for ProcessBridge.
// Projects that consume the DLL define nothing; the DLL project defines PROCESSBRIDGE_EXPORTS.

#ifdef PROCESSBRIDGE_EXPORTS
#define PROCESSBRIDGE_API __declspec(dllexport)
#else
#define PROCESSBRIDGE_API __declspec(dllimport)
#endif
