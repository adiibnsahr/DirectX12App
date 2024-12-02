#include "DXDebugLayer.h"  // Header file yang mendeklarasikan kelas DXDebugLayer untuk debugging DirectX

// Fungsi untuk menginisialisasi lapisan debug DirectX (hanya diaktifkan dalam mode debug)
bool DXDebugLayer::Init()
{
#ifdef _DEBUG  // Hanya mengaktifkan debugging jika dalam mode debug (macro _DEBUG)

    // Menginisialisasi D3D12 Debug Layer, yang memungkinkan pelaporan error dan peringatan dari Direct3D
    // D3D12GetDebugInterface digunakan untuk mendapatkan antarmuka debug dari Direct3D 12
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_d3d12Debug))))
    {
        // Mengaktifkan lapisan debug Direct3D
        m_d3d12Debug->EnableDebugLayer();

        // Menginisialisasi DXGI Debug untuk pelaporan terkait objek perangkat yang hidup (leak detection)
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiDebug))))
        {
            // Mengaktifkan pelacakan kebocoran objek pada thread ini
            m_dxgiDebug->EnableLeakTrackingForThread();
        }
    }

#endif // _DEBUG  // End of debug-specific code

    return false;  // Return false jika gagal menginisialisasi debug layer (kondisi ini seharusnya jarang terjadi)
}

// Fungsi untuk menutup dan membersihkan debugging DirectX (hanya diaktifkan dalam mode debug)
void DXDebugLayer::Shutdown()
{
#ifdef _DEBUG  // Hanya mengeksekusi kode debug dalam mode debug

    // Jika objek debug DXGI ada, lakukan laporan tentang objek perangkat yang masih hidup
    if (m_dxgiDebug)
    {
        // Menampilkan pesan di jendela debug output
        OutputDebugString(L"DXGI Reports living device object:\n");

        // Laporan objek perangkat yang masih hidup, termasuk detail tentang objek yang tidak dilepaskan (leak)
        m_dxgiDebug->ReportLiveObjects(
            DXGI_DEBUG_ALL,  // Melaporkan untuk semua kategori debug DXGI
            DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)  // Laporan detail dan mengabaikan objek internal
        );
    }

    // Melepaskan referensi pada objek debug DXGI dan D3D12
    m_dxgiDebug.Release();  // Melepaskan objek debug DXGI
    m_d3d12Debug.Release();  // Melepaskan objek debug D3D12

#endif // _DEBUG  // End of debug-specific code
}
