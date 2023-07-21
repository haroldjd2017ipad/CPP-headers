#pragma once

#include "Conversion.h"

using namespace Conversion;

LONG WINAPI ErrorLogger(PEXCEPTION_POINTERS  exceptionInfo) {
    LPCWSTR msg = L"RUNTIME ERROR:\n\n";

    std::cout << "AN ERROR OCCURED";

    switch (exceptionInfo->ExceptionRecord->ExceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
        msg = ConcatLPCWSTR(msg, L"Access Violation\n");
        break;
    case EXCEPTION_STACK_OVERFLOW:
        msg = ConcatLPCWSTR(msg, L"Stack Overflow\n");
        break;
        // Add more cases for other exception types as needed
    default:
        msg = ConcatLPCWSTR(msg, L"Unknown Exception\n");
        break;
    }

    // Get the function name and line number from the exception address
    DWORD64 dwDisplacement = 0;
    DWORD64 dwAddress = reinterpret_cast<DWORD64>(exceptionInfo->ExceptionRecord->ExceptionAddress);
    CHAR buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO pSymbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;

    if (SymFromAddr(GetCurrentProcess(), dwAddress, &dwDisplacement, pSymbol)) {
        msg = ConcatLPCWSTR(msg, ConcatLPCWSTR(ConcatLPCWSTR(L"Function: ", ToLPCWSTR(pSymbol->Name)), L"\n"));

        // Get the source file line number
        IMAGEHLP_LINE64 lineInfo;
        lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        if (SymGetLineFromAddr64(GetCurrentProcess(), dwAddress, reinterpret_cast<PDWORD>(&dwDisplacement), &lineInfo)) {
            msg = ConcatLPCWSTR(msg, ConcatLPCWSTR(ConcatLPCWSTR(L"File: ", ToLPCWSTR(lineInfo.FileName)), L"\n"));
            msg = ConcatLPCWSTR(msg, ConcatLPCWSTR(ConcatLPCWSTR(L"Line: ", DWORDToLPCWSTR(lineInfo.LineNumber)), L"\n"));
        }
    }

    wchar_t filename[MAX_PATH];
    DWORD result = GetModuleFileName(nullptr, filename, MAX_PATH);
    wchar_t* pFilename = wcsrchr(filename, L'\\');
    pFilename++;
    wchar_t exceptionAddressStr[20]; // Sufficient buffer to hold the address as a wide string
    StringCchPrintfW(exceptionAddressStr, sizeof(exceptionAddressStr) / sizeof(exceptionAddressStr[0]), L"%p", exceptionInfo->ExceptionRecord->ExceptionAddress);
    msg = ConcatLPCWSTR(msg, ConcatLPCWSTR(L"Exception Address : ", exceptionAddressStr));
    MessageBox(NULL, msg, pFilename, MB_ICONERROR);

    return EXCEPTION_CONTINUE_SEARCH;
}