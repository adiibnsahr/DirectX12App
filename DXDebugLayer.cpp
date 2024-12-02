#include "DXDebugLayer.h"

bool DXDebugLayer::Init()
{
#ifdef _DEBUG

    // Init D3D12 Debug Layer
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_d3d12Debug))))
    {
        m_d3d12Debug->EnableDebugLayer();

        // Init DXGI Debug
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiDebug))))
        {
            m_dxgiDebug->EnableLeakTrackingForThread();
        }
    }

#endif // _DEBUG

    return false;
}

void DXDebugLayer::Shutdown()
{
#ifdef _DEBUG
    if (m_dxgiDebug)
    {
        OutputDebugString(L"DXGI Reports living device object:\n");
        m_dxgiDebug->ReportLiveObjects(
            DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
        );
    }

    m_dxgiDebug.Release();
    m_d3d12Debug.Release();
#endif // _DEBUG
}
