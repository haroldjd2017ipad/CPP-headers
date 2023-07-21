#pragma once

#include <iostream>
#include <windows.h>
#include <gdiplus.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <DbgHelp.h>
#include <strsafe.h>
#include <iomanip>
#include <string>
#include <locale>

namespace Conversion {
    LPCWSTR DWORDToLPCWSTR(DWORD value) {
        wchar_t buffer[20]; // Allocate enough buffer to hold the DWORD as a wide string
        swprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%lu", value);
        size_t length = wcslen(buffer) + 1;
        wchar_t* result = new wchar_t[length];
        wcscpy_s(result, length, buffer);
        return result;
    }

    LPCWSTR ConcatLPCWSTR(LPCWSTR str1, LPCWSTR str2) {
        int totalLen = wcslen(str1) + wcslen(str2) + 1;
        wchar_t* concatenatedStr = new wchar_t[totalLen];

        wcscpy_s(concatenatedStr, totalLen, str1);
        wcscat_s(concatenatedStr, totalLen, str2);

        return concatenatedStr;
    }

    LPCWSTR ToLPCWSTR(const char* str) {
        int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
        if (requiredSize == 0) {
            // Failed to convert, handle error (e.g., call GetLastError() to get the error code).
            return nullptr;
        }

        wchar_t* buffer = new wchar_t[requiredSize];
        if (MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer, requiredSize) == 0) {
            // Failed to convert, handle error (e.g., call GetLastError() to get the error code).
            delete[] buffer;
            return nullptr;
        }

        return buffer;
    }
}