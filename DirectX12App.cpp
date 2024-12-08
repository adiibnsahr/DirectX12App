// DirectX12App.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

#include "WinInclude.h"       // Header untuk deklarasi Windows API terkait
#include "ComPointer.h"       // Header untuk smart pointers yang mengelola objek COM (seperti untuk Direct3D)

#include "DXDebugLayer.h"     // Header untuk lapisan debugging DirectX, untuk membantu proses debug
#include "DXContext.h"        // Header untuk mengelola context DirectX, seperti device dan command queue
#include "Window.h"

int main()
{
    // Inisialisasi layer debug untuk DirectX (membantu dalam debug dan verifikasi operasional DirectX)
    DXDebugLayer::Get().Init();

    // Menginisialisasi context DirectX (device, command queue, dll.)
    if (DXContext::Get().Init() && DXWindow::Get().Init())
    {
        // Masuk ke loop utama aplikasi, menjalankan perintah GPU terus-menerus
        while (!DXWindow::Get().ShouldClose())
        {
            DXWindow::Get().Update();
            // Menginisialisasi command list untuk menulis perintah GPU
            auto* commandList = DXContext::Get().InitCommandList();

            // Mengeksekusi command list yang sudah diinisialisasi
            DXContext::Get().ExecuteCommandList();
        }

        DXWindow::Get().Shutdown();

        // Shutdown context DirectX (melepaskan semua resource dan objek yang digunakan)
        DXContext::Get().Shutdown();
    }

    // Shutdown layer debug DirectX (melepaskan resource yang digunakan oleh debug layer)
    DXDebugLayer::Get().Shutdown();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
