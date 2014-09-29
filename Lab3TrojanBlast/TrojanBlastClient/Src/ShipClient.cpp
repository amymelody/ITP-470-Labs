#include <TrojanBlastClientPCH.h>
#include "ShipClient.h"
#include <SpriteBatch.h>
#include "TextureManager.h"
#include "Timing.h"
#include "InputManager.h"

ShipClient::ShipClient() : Ship()
{
	mInactiveTexture = TextureManager::sInstance->GetTexture("spaceship");
	mThrustingTexture = TextureManager::sInstance->GetTexture("spaceshipWithThrust");

	mSpriteComponent.reset(new SpriteComponent(this));
}

void ShipClient::Update()
{
	//Ship::Update(InputManager::sInstance->GetState());
	
	if (mIsThrusting)
	{
		mSpriteComponent->SetTexture(mThrustingTexture);
	}
	else
	{
		mSpriteComponent->SetTexture(mInactiveTexture);
	}

}