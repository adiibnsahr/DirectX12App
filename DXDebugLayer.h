#pragma once

#include "WinInclude.h"      // Header yang berisi deklarasi Windows API dan komponen terkait
#include "ComPointer.h"      // Header untuk mengelola COM pointers (smart pointers untuk COM objects)

// Kelas DXDebugLayer bertanggung jawab untuk mengelola lapisan debug di DirectX
// Kelas ini hanya akan mengaktifkan fitur debug di build debug
class DXDebugLayer
{
public:
    // Fungsi untuk menginisialisasi lapisan debug DirectX (hanya diaktifkan di mode debug)
    bool Init();

    // Fungsi untuk membersihkan/menghapus lapisan debug saat shutdown
    void Shutdown();

private:
#ifdef _DEBUG  // Kode ini hanya diaktifkan dalam mode debug
    // Smart pointer untuk objek debug D3D12
    ComPointer<ID3D12Debug6> m_d3d12Debug;

    // Smart pointer untuk objek debug DXGI (DirectX Graphics Infrastructure)
    ComPointer<IDXGIDebug1> m_dxgiDebug;
#endif // _DEBUG

    // Singleton pattern untuk memastikan hanya ada satu instance dari DXDebugLayer
public:
    // Menghapus copy constructor dan assignment operator untuk mencegah salinan instance
    DXDebugLayer(const DXDebugLayer&) = delete;
    DXDebugLayer& operator=(const DXDebugLayer&) = delete;

    // Mendapatkan instance tunggal DXDebugLayer (dengan pola Singleton)
    inline static DXDebugLayer& Get()
    {
        static DXDebugLayer instance;  // Instance tunggal DXDebugLayer yang dibuat hanya sekali
        return instance;
    }

private:
    // Konstruktor privat untuk mencegah pembuatan instance di luar kelas ini
    DXDebugLayer() = default;
};
