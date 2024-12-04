#pragma once

#include "WinInclude.h"      // Header yang berisi deklarasi Windows API dan komponen terkait
#include "ComPointer.h"      // Header untuk mengelola COM pointers (seperti smart pointers untuk COM objects)

#include <d3d12.h>           // Header untuk Direct3D 12, API grafis utama yang digunakan
#include <dxgi1_4.h>         // Header untuk DXGI, API yang mengelola tampilan dan swap chain di Direct3D


// Kelas DXContext bertanggung jawab untuk mengelola context DirectX (DX), 
// seperti device dan command queue yang digunakan dalam aplikasi.
class DXContext
{
public:
    // Inisialisasi context DX (seperti membuat device dan command queue)
    // Fungsi ini akan membuat objek perangkat (device), command queue, command allocator, fence, dan command list
    bool Init();

    // Shutdown context DX, melepaskan semua sumber daya yang digunakan
    // Fungsi ini akan melepaskan semua objek yang dialokasikan saat inisialisasi seperti command queue, device, fence, dll.
    void Shutdown();

    // Fungsi untuk memberi sinyal ke fence dan menunggu sampai GPU selesai dengan pekerjaan yang diberikan
    void SignalAndWait();

    // Fungsi untuk menginisialisasi command list yang digunakan untuk menjalankan perintah GPU
    ID3D12GraphicsCommandList7* InitCommandList();

    // Fungsi untuk mengeksekusi command list dan menunggu hingga selesai
    void ExecuteCommandList();

    // Mendapatkan referensi ke device DirectX yang digunakan
    // Ini adalah fungsi inline yang mengembalikan smart pointer ke device DirectX yang digunakan dalam context ini
    inline ComPointer<ID3D12Device10>& GetDevice()
    {
        return m_device;
    }

    // Mendapatkan referensi ke command queue yang digunakan
    // Ini adalah fungsi inline yang mengembalikan smart pointer ke command queue
    inline ComPointer<ID3D12CommandQueue>& GetCommandQueue()
    {
        return m_commandQueue;
    }

private:
    // Smart pointers untuk device dan command queue DirectX
    // m_device adalah objek perangkat yang digunakan untuk berkomunikasi dengan GPU
    // m_commandQueue adalah queue untuk mengeksekusi perintah-perintah GPU
    ComPointer<ID3D12Device10> m_device;
    ComPointer<ID3D12CommandQueue> m_commandQueue;

    // Command Allocator dan Command List digunakan untuk mengalokasikan memori dan mengeksekusi perintah GPU
    ComPointer<ID3D12CommandAllocator> m_commandAllocator;
    ComPointer<ID3D12GraphicsCommandList7> m_commandList;

    // Smart pointer untuk fence (untuk sinkronisasi operasi GPU dengan CPU)
    // Fence digunakan untuk menunggu hingga GPU selesai mengeksekusi perintah sebelumnya
    ComPointer<ID3D12Fence1> m_fence;

    // Nilai fence untuk menjaga sinkronisasi antara CPU dan GPU
    // Nilai ini digunakan untuk menandakan sampai batas mana perintah GPU telah dieksekusi
    UINT64 m_fenceValue = 0;

    // Handle untuk event yang digunakan dalam sinkronisasi dengan fence
    HANDLE m_fenceEvent = nullptr;

    // Singleton pattern untuk memastikan hanya ada satu instance DXContext di seluruh aplikasi
public:
    // Menghapus copy constructor dan assignment operator untuk mencegah penyalinan instance
    // Ini penting karena kita hanya ingin satu instance DXContext di aplikasi ini
    DXContext(const DXContext&) = delete;
    DXContext& operator=(const DXContext&) = delete;

    // Mendapatkan instance tunggal DXContext (dengan pola Singleton)
    // Mengembalikan instance yang sudah dibuat sebelumnya atau membuat instance baru jika belum ada
    inline static DXContext& Get()
    {
        static DXContext instance;  // Instance tunggal DXContext yang dibuat hanya sekali
        return instance;
    }

private:
    // Konstruktor privat untuk mencegah pembuatan instance di luar kelas ini
    // Ini memastikan bahwa hanya satu instance dari DXContext yang bisa ada dalam aplikasi
    DXContext() = default;
};
