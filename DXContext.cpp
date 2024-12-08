#include "DXContext.h"
#include <iostream>


bool DXContext::Init()
{
    // Inisialisasi perangkat grafis (GPU) menggunakan Direct3D 12
    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
    {
        return false;  // Jika gagal, kembalikan false
    }

    // Mengatur parameter untuk command queue (antrian perintah GPU)
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};  // Struktur untuk mendeskripsikan command queue
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;  // Jenis command queue, digunakan untuk perintah langsung
    commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;  // Prioritas tinggi untuk command queue ini
    commandQueueDesc.NodeMask = 0;  // Masking node (0 berarti menggunakan semua node)
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;  // Tidak ada flag khusus untuk command queue ini

    // Membuat command queue dengan parameter yang telah disiapkan
    // Jika berhasil, menyimpan queue tersebut di m_commandQueue
    if (FAILED(m_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_commandQueue))))
    {
        return false;  // Jika gagal, kembalikan false
    }

    // Membuat fence untuk sinkronisasi antara CPU dan GPU
    if (FAILED(m_device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
    {
        return false;  // Jika gagal, kembalikan false
    }

    // Membuat event untuk menunggu completion dari fence
    m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
    if (!m_fenceEvent)
    {
        return false;  // Jika gagal, kembalikan false
    }

    // Membuat command allocator yang digunakan untuk mengalokasikan memori untuk command list
    if (FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator))))
    {
        return false;  // Jika gagal, kembalikan false
    }

    // Membuat command list yang akan digunakan untuk menjalankan perintah GPU
    if (FAILED(m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&m_commandList))))
    {
        return false;  // Jika gagal, kembalikan false
    }

    // Jika semua objek berhasil dibuat, kembalikan true
    return true;
}

// Fungsi untuk melepaskan/menghapus semua objek yang telah dibuat selama inisialisasi
void DXContext::Shutdown()
{
    m_commandList.Release();  // Melepaskan command list
    m_commandAllocator.Release();  // Melepaskan command allocator

    if (m_fenceEvent)
    {
        CloseHandle(m_fenceEvent);  // Menutup handle event fence
    }

    m_commandQueue.Release();  // Melepaskan resource command queue
    m_device.Release();        // Melepaskan resource device
}

void DXContext::SignalAndWait()
{
    // Mengirim sinyal ke fence untuk menandai bahwa command queue telah selesai mengeksekusi perintah
    m_commandQueue->Signal(m_fence, ++m_fenceValue);

    // Menunggu hingga fence mencapai nilai sinyal tertentu
    if (SUCCEEDED(m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent)))
    {
        // Menunggu sampai event fence dipicu atau timeout setelah 20 detik
        if (WaitForSingleObject(m_fenceEvent, 20000) != WAIT_OBJECT_0)
        {
            std::exit(-1);  // Jika tidak selesai dalam waktu yang ditentukan, keluar dari program
        }
    }
    else
    {
        std::exit(-1);  // Jika gagal menunggu, keluar dari program
    }
}

ID3D12GraphicsCommandList7* DXContext::InitCommandList()
{
    // Mereset command allocator sebelum membuat perintah baru
    m_commandAllocator->Reset();

    // Mereset command list dan mempersiapkannya untuk menerima perintah baru
    m_commandList->Reset(m_commandAllocator, nullptr);
    return m_commandList;  // Mengembalikan command list yang telah diinisialisasi
}

void DXContext::ExecuteCommandList()
{
    // Menutup command list dan mengeksekusinya jika tidak ada error
    if (SUCCEEDED(m_commandList->Close()))
    {
        // Menentukan daftar perintah yang akan dieksekusi
        ID3D12CommandList* lists[] = { m_commandList };

        // Mengeksekusi command list melalui command queue
        m_commandQueue->ExecuteCommandLists(1, lists);

        // Menunggu eksekusi selesai
        SignalAndWait();
    }
}
