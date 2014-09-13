#include <TrojanBlastClientPCH.h>
#include "TommyCoinClient.h"
#include <SpriteBatch.h>
#include "SpriteComponent.h"
#include "TextureManager.h"

TommyCoinClient::TommyCoinClient() : TommyCoin()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("Tommy"));
}
