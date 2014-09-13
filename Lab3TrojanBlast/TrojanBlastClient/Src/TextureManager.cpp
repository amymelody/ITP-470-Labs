#include <TrojanBlastClientPCH.h>
#include "TextureManager.h"
#include <DDSTextureLoader.h>
#include "GraphicsDriver.h"

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset( new TextureManager() );
}

TextureManager::TextureManager()
{
	CacheTexture( "spaceshipWithThrust", L"../Assets/spaceshipWithThrust.dds" );
	CacheTexture( "spaceship", L"../Assets/spaceship.dds" );
	CacheTexture( "Tommy", L"../Assets/Tommy.dds" );
	
}

TexturePtr	TextureManager::GetTexture( const string& inTextureName )
{
	return mNameToTextureMap[ inTextureName ];
}

HRESULT TextureManager::CacheTexture( string inTextureName, const wchar_t* inFileName )
{
	ID3D11ShaderResourceView* textureResource;
	//DDS_ALPHA_MODE alphaMode = DirectX::DDS_ALPHA_MODE_PREMULTIPLIED;
	HRESULT hr = CreateDDSTextureFromFile( GraphicsDriver::sInstance->GetDevice(), inFileName, nullptr, &textureResource, 0, nullptr );
    if( FAILED( hr ) )
        return hr;

	TexturePtr newTexture( new Texture( 0, 0, textureResource ) );

	mNameToTextureMap[ inTextureName ] = newTexture;

	return S_OK;

}