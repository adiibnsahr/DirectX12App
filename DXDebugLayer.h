#pragma once

#include "WinInclude.h"
#include "ComPointer.h"

class DXDebugLayer
{
	public:
		bool Init();
		void Shutdown();

	private:
	#ifdef _DEBUG
		ComPointer<ID3D12Debug6> m_d3d12Debug;
		ComPointer<IDXGIDebug1> m_dxgiDebug;
	#endif // _DEBUG



	// Singleton
	public:
		DXDebugLayer(const DXDebugLayer&) = delete;
		DXDebugLayer& operator=(const DXDebugLayer&) = delete;

		inline static DXDebugLayer& Get()
		{
			static DXDebugLayer instance;
			return instance;
		}

	private:
		DXDebugLayer() = default;

};