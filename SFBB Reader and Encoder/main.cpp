#include <windows.h>
#include "encoderdecoder.h"
#include <iostream>
#include <stdio.h>
#include <gdiplus.h>
#include "view.h"
#include <string>

ULONG_PTR gdiplusToken;

int wmain(int argc, wchar_t** argv){


    Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    const wchar_t* fPh = *(argv+1);

    if(fPh==nullptr){
        wprintf(L"sorry, no arguments\n");
        return 0;
    }
    
    std::wstring strPath = std::wstring(fPh);

    wprintf(L"File Path: ");

    wprintf(L"[START]");
    
    wprintf(fPh);

    wprintf(L"[END]");

    wprintf(L"\n");


    if (strPath.substr(strPath.find_last_of(L".") + 1) == L"sfbb") {
        decodesfbb(fPh);
    }
    else {
        encodeimage(fPh);
    }


    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}