#include<TrojanBlastPCH.h>
#include<DirectXColors.h>

using namespace DirectX;



GameObject::GameObject() :
	mIndexInWorld( -1 ),
	mCollisionRadius( 0.5f ),
	mDoesWantToDie( false ),
	mRotation( 0.f ),
	mNetworkId( 0 ),
	mColor( Colors::White )
{
	mLocation = XMVectorSet( 0.f, 0.f, 0.f, 0.f );

	mRealWidth = 1.f;

	//hardcoded because texture loaded doesn't tell size...
	float textureWidth = 128.f;

	//so, your texture size is something ( for the ship it's 128x128 )
	//and then we need to scale from that to meters- say the ship is 1 meter
	//so our scale is mRealWidth / textureWidth
	mScale = mRealWidth / textureWidth;

}

void GameObject::Update()
{
	//object don't do anything by default...	
}


XMVECTOR GameObject::GetForwardVector()	const
{
	//should we cache this when you turn?
	return XMVectorSet( sinf( mRotation ), -cosf( mRotation ), 0.f, 0.f );
}

void GameObject::SetNetworkId( int inNetworkId )
{ 
	//this doesn't put you in the map or remove you from it
	mNetworkId = inNetworkId; 

}

void* GameObject::operator new( size_t inSize )
{
	return _aligned_malloc( inSize, 16 );;
}

void GameObject::operator delete( void * inP )
{
	_aligned_free( inP );
}
