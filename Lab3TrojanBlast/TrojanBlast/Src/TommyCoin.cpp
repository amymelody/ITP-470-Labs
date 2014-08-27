#include <TrojanBlastPCH.h>
#include <SpriteBatch.h>

TommyCoin::TommyCoin()
{
	SetScale( GetScale() * 0.5f );
	SetCollisionRadius( 0.25f );

	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("Tommy"));
}


bool TommyCoin::HandleCollisionWithShip( Ship* inShip )
{
	//kill yourself!
	SetDoesWantToDie( true );

	ScoreBoardManager::sInstance->IncScore( inShip->GetPlayerID(), 1 );

	return false;
}
