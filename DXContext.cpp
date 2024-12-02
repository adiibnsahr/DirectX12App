#include "DXContext.h"


bool DXContext::Init()
{
    // Membuat Direct3D 12 device, yang akan digunakan untuk rendering
    // D3D12CreateDevice mencoba membuat perangkat GPU untuk komunikasi dengan DirectX 12
    // Jika pembuatan device berhasil, maka akan menyimpan perangkat tersebut dalam m_device
    if (SUCCEEDED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
    {
        // Mengatur parameter untuk command queue (antrian perintah GPU)
        D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};  // Struktur untuk mendeskripsikan command queue
        commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;  // Jenis command queue, digunakan untuk perintah langsung
        commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;  // Prioritas tinggi untuk command queue ini
        commandQueueDesc.NodeMask = 0;  // Masking node (0 berarti menggunakan semua node)
        commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;  // Tidak ada flag khusus untuk command queue ini

        // Membuat command queue dengan parameter yang telah disiapkan
        // Jika berhasil, menyimpan queue tersebut di m_commandQueue
        if (SUCCEEDED(m_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_commandQueue))))
        {
            // Membuat fence untuk sinkronisasi antara CPU dan GPU
            // Fence digunakan untuk menunggu sampai operasi GPU selesai sebelum melanjutkan
            if (SUCCEEDED(m_device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
            {
                return true;  // Jika semua komponen (device, command queue, fence) berhasil dibuat, kembalikan true
            }
        }
    }

    // Jika ada kegagalan pada proses pembuatan device, command queue, atau fence, return false
    return false;
}

// Fungsi untuk melepaskan/menghapus semua objek yang telah dibuat selama inisialisasi
void DXContext::Shutdown()
{
    m_commandQueue.Release();  // Melepaskan resource command queue
    m_device.Release();        // Melepaskan resource device
}
