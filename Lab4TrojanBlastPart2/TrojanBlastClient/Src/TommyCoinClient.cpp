#include <TrojanBlastClientPCH.h>

TommyCoinClient::TommyCoinClient()
{
	mSpriteComponent.reset( new SpriteComponent( this ) );
	mSpriteComponent->SetTexture( TextureManager::sInstance->GetTexture( "Tommy" ) );
}