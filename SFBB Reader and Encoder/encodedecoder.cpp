#include <Windows.h>
#include <gdiplus.h>
#include <iostream>
#include "image.h"
#include "view.h"


int encodeimage(const TCHAR* filepath) {
    
    printf("\n -- Converting File -- \n");

    
    Gdiplus::Bitmap* m = Gdiplus::Bitmap::FromFile(filepath); // heap allocation

    if (m == nullptr) {
        printf("sorry, file is not a bitmap\n");
        return -1;
    }

    int width = m->GetWidth();
    int height = m->GetHeight();

    int imgByteSize = (width * height * 4) + 2;

    void* data = malloc(imgByteSize);

    if (data == nullptr) {
        printf("sorry, no image data\n");
        return -1;
    }

    if (width > 65536 || height > 65536) {
        printf("sorry, image width or height is too big\n");
        return -1;
    }


    wchar_t* ptr_ = (wchar_t*)data;

    *ptr_ = width;

    byte* ptr = (byte*)data;
    ptr += 2;

    
    //ptr += 4;

    //*ptr = height;

    //ptr += 4;
    
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Gdiplus::Color c;
            m->GetPixel(x, y, &c);
            INT8 pix = c.GetR();
            *ptr = pix;
            ptr++;
        }
    }


    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Gdiplus::Color c;
            m->GetPixel(x, y, &c);
            INT8 pix = c.GetG();
            *ptr = pix;
            ptr++;
        }
    }


    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Gdiplus::Color c;
            m->GetPixel(x, y, &c);
            INT8 pix = c.GetB();
            *ptr = pix;
            ptr++;
        }
    }


    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Gdiplus::Color c;
            m->GetPixel(x, y, &c);
            INT8 pix = c.GetA();
            *ptr = pix;
            ptr++;
        }
    }
    
    std::wstring str_path = std::wstring(filepath);

    std::wstring o_dir = str_path.substr(0, str_path.find_last_of(L"."));
    o_dir += L".sfbb";

    std::wcout << o_dir;

    // write to a file
    HANDLE hFile = CreateFileW(
        o_dir.c_str(),     // Filename
        GENERIC_WRITE,          // Desired access
        FILE_SHARE_READ,        // Share mode
        NULL,                   // Security attributes
        CREATE_ALWAYS,             // Creates a new file, only if it doesn't already exist
        FILE_ATTRIBUTE_NORMAL,  // Flags and attributes
        NULL);                  // Template file handle



    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "sorry, no file handling\n";
        return -1;
    }


    // Write data to the file
    std::string strText = "Hello World!"; // For C use LPSTR (char*) or LPWSTR (wchar_t*)
    DWORD bytesWritten;
    WriteFile(
        hFile,            // Handle to the file
        data,  // Buffer to write
        imgByteSize,   // Buffer size
        &bytesWritten,    // Bytes written
        nullptr);         // Overlapped

     // Close the handle once we don't need it.
    CloseHandle(hFile);

    delete m;
    free(data);
    
    
return 0;
    
}

int decodesfbb(const TCHAR* filepath) {
    printf("\n -- Reading File -- \n");
    HANDLE hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("sorry, the handle value was invalid");
        return -1;
    }

    DWORD fsize = GetFileSize(hFile, 0);
    printf("File Size: %i\n", fsize);

    int sizeOfAllocation = fsize;
    void* data = malloc(sizeOfAllocation);
    DWORD dwBytesRead = 0;
    DWORD dwBytesWritten = 0;

    if (!ReadFile(hFile, data, sizeOfAllocation, &dwBytesRead, NULL)) {
        return -1;
    }

    uint16_t* widthptr = (uint16_t*)data;
    if (widthptr == NULL) {
        printf("sorry, reading width failed\n");
        return -1;
    }

    
    int numOfPixels = (fsize - 2) / 4;

    printf("Number of pixels: %i\n", numOfPixels);

    int width = *(widthptr);

    printf("Width: %i\n", width);

    int height = numOfPixels / width;

    printf("Height: %i\n", height);

    int bitmapDataSize = numOfPixels * 4;
    void* bitmapData = malloc(bitmapDataSize);

    int* bmp_ptr = (int*)bitmapData;

    byte* dataptr = (byte*)data;
    
    for (int i = 0; i < (long long int)numOfPixels; i++) {

        // data
        byte* r_ptr = (dataptr + 2 + i);
        byte* g_ptr = (dataptr + 2 + i) + numOfPixels;
        byte* b_ptr = (dataptr + 2 + i) + numOfPixels * 2;
        byte* a_ptr = (dataptr + 2 + i) + numOfPixels * 3;

        //Gdiplus::Color c(*a_ptr, *r_ptr, *g_ptr, *b_ptr);

        int r = *r_ptr;
        int g = *g_ptr;
        int b = *b_ptr;
        int a = *a_ptr;

        int c = (a * 16777216) + (r * 65536) + (g * 256) + b;
        *bmp_ptr++ = c;
    }

    viewimage(width, height, bitmapData);

    printf("\nCleaning up memory..\n");
    free(bitmapData);
    free(data);
    printf("Cleaned\n");

    return 0;
}