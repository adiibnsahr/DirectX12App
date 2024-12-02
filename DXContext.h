#pragma once

#include "WinInclude.h"      // Header yang berisi deklarasi Windows API dan komponen terkait
#include "ComPointer.h"      // Header untuk mengelola COM pointers (seperti smart pointers untuk COM objects)

// Kelas DXContext bertanggung jawab untuk mengelola context DirectX (DX), 
// seperti device dan command queue yang digunakan dalam aplikasi.
class DXContext
{
public:
    // Inisialisasi context DX (seperti membuat device dan command queue)
    bool Init();

    // Shutdown context DX, melepaskan semua sumber daya yang digunakan
    void Shutdown();

    // Mendapatkan referensi ke device DirectX yang digunakan
    inline ComPointer<ID3D12Device10>& GetDevice()
    {
        return m_device;
    }

    // Mendapatkan referensi ke command queue yang digunakan
    inline ComPointer<ID3D12CommandQueue>& GetCommandQueue()
    {
        return m_commandQueue;
    }

private:
    // Smart pointers untuk device dan command queue DirectX
    ComPointer<ID3D12Device10> m_device;
    ComPointer<ID3D12CommandQueue> m_commandQueue;

    // Smart pointer untuk fence (untuk sinkronisasi operasi GPU dengan CPU)
    ComPointer<ID3D12Fence1> m_fence;

    // Nilai fence untuk menjaga sinkronisasi antara CPU dan GPU
    UINT64 m_fenceValue = 0;

    // Singleton pattern untuk memastikan hanya ada satu instance DXContext di seluruh aplikasi
public:
    // Menghapus copy constructor dan assignment operator untuk mencegah penyalinan instance
    DXContext(const DXContext&) = delete;
    DXContext& operator=(const DXContext&) = delete;

    // Mendapatkan instance tunggal DXContext (dengan pola Singleton)
    inline static DXContext& Get()
    {
        static DXContext instance;  // Instance tunggal DXContext yang dibuat hanya sekali
        return instance;
    }

private:
    // Konstruktor privat untuk mencegah pembuatan instance di luar kelas ini
    DXContext() = default;

};
