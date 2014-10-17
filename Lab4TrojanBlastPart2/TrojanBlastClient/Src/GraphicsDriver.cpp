#include <TrojanBlastClientPCH.h>
#include "CommonStates.h"

using namespace DirectX;

std::unique_ptr< GraphicsDriver >	GraphicsDriver::sInstance;


namespace
{
	D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;

	IDXGISwapChain*                     g_pSwapChain = nullptr;
	ID3D11Texture2D*                    g_pDepthStencil = nullptr;
}


HRESULT GraphicsDriver::StaticInit( HWND inHWnd )
{
	GraphicsDriver* newGraphicsDriver = new GraphicsDriver();
	HRESULT hr = newGraphicsDriver->Init( inHWnd );

	if( FAILED( hr ) )
	{
		delete newGraphicsDriver;
	}
	else
	{
		sInstance.reset( newGraphicsDriver );
	}

	return hr;
}

HRESULT GraphicsDriver::Init( HWND inHWnd )
{
	HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( inHWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = inHWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &mDevice, &g_featureLevel, &mDeviceContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = mDevice->CreateRenderTargetView( pBackBuffer, nullptr, &mRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = mDevice->CreateTexture2D( &descDepth, nullptr, &g_pDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = mDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &mDepthStencilView );
    if( FAILED( hr ) )
        return hr;

    mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mDeviceContext->RSSetViewports( 1, &vp );

	return hr;

	
}

GraphicsDriver::GraphicsDriver() :
	mDeviceContext( nullptr ),
	mDevice( nullptr ),
	mRenderTargetView( nullptr ),
	mDepthStencilView( nullptr )
{
}


GraphicsDriver::~GraphicsDriver()
{
	 if( mDeviceContext ) mDeviceContext->ClearState();


    if( mDepthStencilView ) mDepthStencilView->Release();
    if( g_pDepthStencil ) g_pDepthStencil->Release();
    if( mRenderTargetView ) mRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( mDeviceContext ) mDeviceContext->Release();
    if( mDevice ) mDevice->Release();
}

void GraphicsDriver::Present()
{
	g_pSwapChain->Present( 0, 0 );
}
