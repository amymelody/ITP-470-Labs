#include <TrojanBlastClientPCH.h>
#include<SpriteBatch.h>

SpriteComponent::SpriteComponent( GameObject *inGameObject ) :
	mGameObject( inGameObject )
{
	//hardcoded at the moment...
	float textureWidth = 128.f, textureHeight = 128.f;
	//origin should be half texture size, but we're not loading the actual size at the moment
	mOrigin = XMVectorSet( textureWidth * 0.5f, textureHeight * 0.5f, 0.f, 0.f );

	//and add yourself to the rendermanager...
	RenderManager::sInstance->AddComponent( this );
}

SpriteComponent::~SpriteComponent()
{
	//don't render me, I'm dead!
	RenderManager::sInstance->RemoveComponent( this );
}


void SpriteComponent::Draw()
{
	if( mTexture )
	{
		RenderManager::sInstance->GetSpriteBatch()->Draw( mTexture->GetShaderResourceView(), mGameObject->GetLocation(), nullptr, mGameObject->GetColor(), mGameObject->GetRotation(), mOrigin, mGameObject->GetScale() );
	}
}