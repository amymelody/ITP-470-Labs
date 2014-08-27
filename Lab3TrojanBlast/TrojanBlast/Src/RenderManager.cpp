#include <TrojanBlastPCH.h>

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <DirectXColors.h>
#include <CommonStates.h>

using namespace DirectX;


std::unique_ptr< RenderManager >	RenderManager::sInstance;

RenderManager::RenderManager()
{
    mSpriteBatch.reset( new SpriteBatch( GraphicsDriver::sInstance->GetDeviceContext() ) );

	mViewPoint = XMVectorZero();

	 D3D11_VIEWPORT viewport;
    UINT viewportCount = 1;

    GraphicsDriver::sInstance->GetDeviceContext()->RSGetViewports( &viewportCount, &viewport );

	mHalfViewPortSize = XMVectorSet( viewport.Width * 0.5f, viewport.Height * 0.5f, 0.f, 0.f );

	//let's say 100 pixels is 1 meter...we should be going 50 pixels per second then, right?
	mZoomScale = XMVectorSet( 100.f, 100.f, 1.f, 1.f );

	mStateObjects.reset( new CommonStates( GraphicsDriver::sInstance->GetDevice() ) );
}


void RenderManager::StaticInit()
{
	sInstance.reset( new RenderManager() );
}


void RenderManager::AddComponent( SpriteComponent* inComponent )
{
	mComponents.push_back( inComponent );
}

void RenderManager::RemoveComponent( SpriteComponent* inComponent )
{
	int index = GetComponentIndex( inComponent );

	if( index != -1 )
	{
		int lastIndex = mComponents.size() - 1;
		if( index != lastIndex )
		{
			mComponents[ index ] = mComponents[ lastIndex ];
		}
		mComponents.pop_back();
	}
}

int RenderManager::GetComponentIndex( SpriteComponent* inComponent ) const
{
	for( int i = 0, c = mComponents.size(); i < c; ++i )
	{
		if( mComponents[ i ] == inComponent )
		{
			return i;
		}
	}

	return -1;
}


//this part that renders the world is really a camera-
//in a more detailed engine, we'd have a list of cameras, and then render manager would
//render the cameras in order
void RenderManager::RenderComponents()
{

	//let's allow the camera to move, and let's also adjust the scale so that
	//we can have things in meters and that changes ot pixels
	//XMMATRIX transformMatrix = XMMatrixIdentity();
	XMMATRIX transformMatrix = XMMatrixTransformation2D( 
		XMVectorZero(), 0.f, mZoomScale, 
		XMVectorZero(), 0.f, 
		mViewPoint + mHalfViewPortSize );

	//our sprites are not prmeultiplied here- we need common states

	mSpriteBatch->Begin( SpriteSortMode_Deferred, mStateObjects->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, transformMatrix );
	
	for( auto cIt = mComponents.begin(), end = mComponents.end(); cIt != end; ++cIt )
	{
		( *cIt )->Draw();
	}
	
	mSpriteBatch->End();


	
}

void RenderManager::Render()
{
	//
    // Clear the back buffer
    //
    GraphicsDriver::sInstance->GetDeviceContext()->ClearRenderTargetView( GraphicsDriver::sInstance->GetRenderTargetView(), Colors::MidnightBlue );

    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    GraphicsDriver::sInstance->GetDeviceContext()->ClearDepthStencilView( GraphicsDriver::sInstance->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );


	
	RenderManager::sInstance->RenderComponents();

	HUD::sInstance->Render();
	
	//
    // Present our back buffer to our front buffer
    //
	GraphicsDriver::sInstance->Present();

}