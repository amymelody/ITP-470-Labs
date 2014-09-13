#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "GameObject.h"
#include "Texture.h"

class SpriteComponent
{
public:

	SpriteComponent( GameObject* inGameObject );
	~SpriteComponent();

	void*	operator new(size_t inSize)	{ return _aligned_malloc(inSize, 16); }
	void	operator delete(void * inP)	{ _aligned_free(inP); }

	virtual void									Draw();

			void		SetTexture( TexturePtr inTexture )				{ mTexture = inTexture; }

			XMVECTOR	GetOrigin()					const				{ return mOrigin; }
			void		SetOrigin( const FXMVECTOR inOrigin )			{ mOrigin = inOrigin; }


private:

	XMVECTOR										mOrigin;

	TexturePtr										mTexture;

	//don't want circular reference...
	GameObject*										mGameObject;
};

typedef shared_ptr< SpriteComponent >	SpriteComponentPtr;

#endif