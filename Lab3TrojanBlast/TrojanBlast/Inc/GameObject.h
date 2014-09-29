#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "PacketBuffer.h"

class Ship;

class GameObject
{
public:

	GameObject();
	virtual ~GameObject() {}

	void* operator new(size_t inSize);
	void operator delete(void * inP);

	virtual	Ship*	GetAsShip()	{ return nullptr; }

	virtual uint32_t GetFourCCName() const = 0;

	uint32_t mNetworkID;

	virtual bool Write(PacketBuffer* inPacketBuffer) const { return false; }
	virtual bool Read(PacketBuffer* inPacketBuffer) { return false; }

	//return whether to keep processing collision
	virtual bool	HandleCollisionWithShip( Ship* inShip ) { ( void ) inShip; return true; }

	virtual void	Update();

			void	SetIndexInWorld( int inIndex )						{ mIndexInWorld = inIndex; }
			int		GetIndexInWorld()				const				{ return mIndexInWorld; }

			void	SetRotation( float inRotation )						{ mRotation = inRotation; }
			float	GetRotation()					const				{ return mRotation; }

			void	SetScale( float inScale )							{ mScale = inScale; }
			float	GetScale()						const				{ return mScale; }


			XMVECTOR	GetLocation()				const				{ return mLocation; }
			void		SetLocation( const FXMVECTOR inLocation )		{ mLocation = inLocation; }

			float		GetCollisionRadius()		const				{ return mCollisionRadius; }
			void		SetCollisionRadius( float inRadius )			{ mCollisionRadius = inRadius; }

			XMVECTOR	GetForwardVector()			const;


			void		SetColor( FXMVECTOR inColor )					{ mColor = inColor; }
			XMVECTOR	GetColor()					const				{ return mColor; }

			bool		DoesWantToDie()				const				{ return mDoesWantToDie; }
			void		SetDoesWantToDie( bool inWants )				{ mDoesWantToDie = inWants; }

			bool updated;

protected:

	XMVECTOR										mLocation;

	XMVECTOR										mColor;
	float											mRotation;

private:

	float											mRealWidth;
	
	float											mCollisionRadius;


	float											mScale;
	int												mIndexInWorld;

	bool											mDoesWantToDie;

};

typedef shared_ptr< GameObject >	GameObjectPtr;

#endif;