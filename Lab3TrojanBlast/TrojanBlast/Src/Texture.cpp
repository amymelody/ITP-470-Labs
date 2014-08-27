#include <TrojanBlastPCH.h>

Texture::Texture( uint32_t inWidth, uint32_t inHeight, ID3D11ShaderResourceView* inShaderResourceView ) :
	mWidth( inWidth ),
	mHeight( inHeight ),
	mShaderResourceView( inShaderResourceView )
{
}

Texture::~Texture()
{
	if( mShaderResourceView )
	{
		mShaderResourceView->Release();
	}
}