#include <TrojanBlastClientPCH.h>

ShipClient::ShipClient()
{
	mInactiveTexture = TextureManager::sInstance->GetTexture( "spaceship" );
	mThrustingTexture = TextureManager::sInstance->GetTexture( "spaceshipWithThrust" );

	mSpriteComponent.reset( new SpriteComponent( this ) );
}

void ShipClient::Update()
{
	Ship::Update();

	//do we need to replicate this? sure we do!
	if( mIsThrusting )
	{
		mSpriteComponent->SetTexture( mThrustingTexture );
	}
	else
	{
		mSpriteComponent->SetTexture( mInactiveTexture );
	}
}