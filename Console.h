#pragma once

#include "Conversion.h"

void EnableConsoleOutput()
{
    AllocConsole();
    HANDLE consoleHandle = CreateFile(L"CONOUT$", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    if (consoleHandle != INVALID_HANDLE_VALUE)
    {
        SetStdHandle(STD_OUTPUT_HANDLE, consoleHandle);
        FILE* file = nullptr;
        freopen_s(&file, "CONOUT$", "w", stdout);
        std::ios::sync_with_stdio();
    }
}