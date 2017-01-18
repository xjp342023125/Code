#pragma once
#include "VMProtectSDK.h"
#include "d3d9.h"
#include "../ComAll/XType.h"
#include "CFunc.hpp"
#include <iostream>
using namespace std;


const char *pD3DName[]={
"QueryInterface",
"AddRef",
"Release",
"TestCooperativeLevel",
"GetAvailableTextureMem",
"EvictManagedResources",
"GetDirect3D",
"GetDeviceCaps",
"GetDisplayMode",
"GetCreationParameters",
"SetCursorProperties",
"SetCursorPosition",
"ShowCursor",
"CreateAdditionalSwapChain",
"GetSwapChain",
"GetNumberOfSwapChains",
"Reset",
"Present",
"GetBackBuffer",
"GetRasterStatus",
"SetDialogBoxMode",
"SetGammaRamp",
"GetGammaRamp",
"CreateTexture",
"CreateVolumeTexture",
"CreateCubeTexture",
"CreateVertexBuffer",
"CreateIndexBuffer",
"CreateRenderTarget",
"CreateDepthStencilSurface",
"UpdateSurface",
"UpdateTexture",
"GetRenderTargetData",
"GetFrontBufferData",
"StretchRect",
"ColorFill",
"CreateOffscreenPlainSurface",
"SetRenderTarget",
"GetRenderTarget",
"SetDepthStencilSurface",
"GetDepthStencilSurface",
"BeginScene",
"EndScene",
"Clear",
"SetTransform",
"GetTransform",
"MultiplyTransform",
"SetViewport",
"GetViewport",
"SetMaterial",
"GetMaterial",
"SetLight",
"GetLight",
"LightEnable",
"GetLightEnable",
"SetClipPlane",
"GetClipPlane",
"SetRenderState",
"GetRenderState",
"CreateStateBlock",
"BeginStateBlock",
"EndStateBlock",
"SetClipStatus",
"GetClipStatus",
"GetTexture",
"SetTexture",
"GetTextureStageState",
"SetTextureStageState",
"GetSamplerState",
"SetSamplerState",
"ValidateDevice",
"SetPaletteEntries",
"GetPaletteEntries",
"SetCurrentTexturePalette",
"GetCurrentTexturePalette",
"SetScissorRect",
"GetScissorRect",
"SetSoftwareVertexProcessing",
"GetSoftwareVertexProcessing",
"SetNPatchMode",
"GetNPatchMode",
"DrawPrimitive",
"DrawIndexedPrimitive",
"DrawPrimitiveUP",
"DrawIndexedPrimitiveUP",
"ProcessVertices",
"CreateVertexDeclaration",
"SetVertexDeclaration",
"GetVertexDeclaration",
"SetFVF",
"GetFVF",
"CreateVertexShader",
"SetVertexShader",
"GetVertexShader",
"SetVertexShaderConstantF",
"GetVertexShaderConstantF",
"SetVertexShaderConstantI",
"GetVertexShaderConstantI",
"SetVertexShaderConstantB",
"GetVertexShaderConstantB",
"SetStreamSource",
"GetStreamSource",
"SetStreamSourceFreq",
"GetStreamSourceFreq",
"SetIndices",
"GetIndices",
"CreatePixelShader",
"SetPixelShader",
"GetPixelShader",
"SetPixelShaderConstantF",
"GetPixelShaderConstantF",
"SetPixelShaderConstantI",
"GetPixelShaderConstantI",
"SetPixelShaderConstantB",
"GetPixelShaderConstantB",
"DrawRectPatch",
"DrawTriPatch",
"DeletePatch",
"CreateQuery"};


static void PD3DAddr(addr_t device)
{
	VMProtectBegin(__FUNCTION__);
	int nSize = sizeof(pD3DName)/sizeof(pD3DName[0]);
	for (int i = 0;i < nSize;++i)
	{
		cout<<"name:"<<pD3DName[i]<<",addr:"<<hex<<GetVFuncAddrByIndex(device,i)<<endl;
	}
	VMProtectEnd();
}


static LRESULT WINAPI CCCDDD( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	VMProtectBegin(__FUNCTION__);
	return DefWindowProc( hWnd, msg, wParam, lParam );
	VMProtectEnd();
}

LPDIRECT3DDEVICE9 GetDevice(wchar_t *pName = L"sfg8723")
{
	VMProtectBegin(__FUNCTION__);
	static LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // Our rendering device
	if (NULL != g_pd3dDevice)
	{
		return g_pd3dDevice;
	}
	WNDCLASSEXW wc =
	{
		sizeof( WNDCLASSEX ), CS_CLASSDC, CCCDDD, 0L, 0L,
		GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
		pName, NULL
	};
	RegisterClassExW( &wc );

	// Create the application's window
	HWND hWnd = CreateWindowW(pName, pName,
		WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
		NULL, NULL, wc.hInstance, NULL );

	LPDIRECT3D9 g_pD3D = NULL;
	
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return NULL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		cout << "CreateDevice err" << endl;
		return NULL;
	}
	
	return g_pd3dDevice;
	VMProtectEnd();

}
static DWORD* GetD3DVFuncAddr(const char *p,LPDIRECT3DDEVICE9 d=NULL)
{
	VMProtectBegin(__FUNCTION__);
	int nSize = sizeof(pD3DName)/sizeof(pD3DName[0]);
	int nIndex = -1;
	for (int i = 0;i < nSize;++i)
	{
		if (0 == strcmp(p,pD3DName[i]))
		{
			nIndex = i;
			break;
		}
	}
	if (-1 == nIndex)
	{
		return 0;
	}
	if (NULL == d)
	{
		d = GetDevice();
	}
	
	return (DWORD*)GetVFuncAddrAddrByIndex((addr_t)d,nIndex);
	VMProtectEnd();
}
static DWORD GetD3dAddr(const char *p,LPDIRECT3DDEVICE9 d=NULL)
{
	VMProtectBegin(__FUNCTION__);
	DWORD *addr = GetD3DVFuncAddr(p, d);
	if (addr)
	{
		return *addr;
	}
	return NULL;
	VMProtectEnd();
}


typedef HRESULT (WINAPI* DrawIndexedPrimitive_t)(LPDIRECT3DDEVICE9 p, D3DPRIMITIVETYPE t, INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
typedef HRESULT(WINAPI *SetRenderState_t)(IDirect3DDevice9*, D3DRENDERSTATETYPE State, DWORD Value);
