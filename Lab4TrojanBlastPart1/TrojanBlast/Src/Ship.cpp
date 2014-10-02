#include <TrojanBlastPCH.h>
#include<SpriteBatch.h>

using namespace DirectX;

//zoom hardcoded at 100...if we want to lock players on screen, this could be calculated from zoom
#define HALF_WORLD_HEIGHT	3.6f
#define HALF_WORLD_WIDTH	6.4f

Ship::Ship() :
	GameObject(),
	mMaxDrivenRotationSpeed( 5.f ),
	mMaxDrivenLinearSpeed( 5.f ),
	mMaxDrivenLinearAcceleration( 2.f ),
	mVelocity( XMVectorZero() ),
	mWallRestitution( 0.3f ),
	mShipRestitution( 0.9f ),
	mIsThrusting( false ),
	mPlayerId( 0 )
{
	SetCollisionRadius( 0.4f );
}

void Ship::ProcessInput( float inDeltaTime, const InputState& inInputState )
{
	//process our input....

	//turning...
	float newRotation = GetRotation() + inInputState.GetDesiredHorizontalDelta() * mMaxDrivenRotationSpeed * inDeltaTime;
	SetRotation( newRotation );

	//moving...
	float inputForwardDelta = inInputState.GetDesiredVerticalDelta();
	if( inputForwardDelta != 0.f )
	{
		XMVECTOR forwardVector = GetForwardVector();
		mVelocity += XMVectorScale( forwardVector, inInputState.GetDesiredVerticalDelta() * inDeltaTime * mMaxDrivenLinearAcceleration ); 
		mIsThrusting = true;
	}
	else
	{
		mIsThrusting = false;
	}
}

void Ship::SimulateMovement( float inDeltaTime )
{
	//simulate us...
	SetLocation( GetLocation() + XMVectorScale( mVelocity, inDeltaTime ) );

	ProcessCollisions();
}

void Ship::Update()
{
	
}

void Ship::ProcessCollisions()
{
	//right now just bounce off the sides..
	//this is horribly inefficient but it doens't matter...

	ProcessCollisionsWithScreenWalls();

	float sourceRadius = GetCollisionRadius();
	XMVECTOR sourceLocation = GetLocation();

	//now let's iterate through the world and see what we hit...
	for( auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end(); goIt != end; ++goIt )
	{
		GameObject* target = goIt->get();
		if( target != this )
		{
			//simple collision test for spheres- are the radii summed less than the distance?
			XMVECTOR targetLocation = target->GetLocation();
			float targetRadius = target->GetCollisionRadius();

			XMVECTOR delta = targetLocation - sourceLocation;
			XMVECTOR distSq = XMVector2LengthSq( delta );
			float collisionDist = ( sourceRadius + targetRadius );
			if( XMVectorGetX( distSq ) < ( collisionDist * collisionDist ) )
			{
				//first, tell the other guy there was a collision with a ship, so it can do something...

				if( target->HandleCollisionWithShip( this ) )
				{
					//okay, you hit something!
					//so, project your location far enough that you're not colliding
					XMVECTOR dirToTarget = XMVector2Normalize( delta );
					XMVECTOR acceptableDeltaFromSourceToTarget = XMVectorScale( dirToTarget, collisionDist );
					//important note- we only move this ship. the other ship can take care of moving itself
					SetLocation( targetLocation - acceptableDeltaFromSourceToTarget );

					
					XMVECTOR relVel = mVelocity;
				
					//if other object is a ship, it might have velocity, so there might be relative velocity...
					Ship* targetShip = target->GetAsShip();
					if( targetShip )
					{
						relVel -= targetShip->mVelocity;
					}

					//got vel with dir between objects to figure out if they're moving towards each other
					//and if so, the magnitude of the impulse ( since they're both just balls )
					XMVECTOR relVelDotDir = XMVector2Dot( relVel, dirToTarget );

					if( XMVectorGetX( relVelDotDir ) > 0.f )
					{
						XMVECTOR impulse = relVelDotDir * dirToTarget;
					
						if( targetShip )
						{
							mVelocity -= impulse;
							mVelocity *= mShipRestitution;

							//we do impart some velocity on the other ship....
							targetShip->mVelocity += impulse;
							targetShip->mVelocity *= targetShip->mShipRestitution;
						}
						else
						{
							mVelocity -= impulse * 2.f;
							mVelocity *= mWallRestitution;
						}

					}
				}
			}
		}
	}

}

void Ship::ProcessCollisionsWithScreenWalls()
{
	XMVECTOR location = GetLocation();
	float x = XMVectorGetX( location );
	float y = XMVectorGetY( location );

	float vx = XMVectorGetX( mVelocity );
	float vy = XMVectorGetY( mVelocity );

	float radius = GetCollisionRadius();

	if( ( y + radius ) >= HALF_WORLD_HEIGHT && vy > 0 )
	{
		mVelocity = XMVectorSetY( mVelocity, -vy * mWallRestitution );
		//should we pin your location? probably...
		SetLocation( XMVectorSetY( location, HALF_WORLD_HEIGHT - radius ) );
	}
	else if( ( y - radius ) <= -HALF_WORLD_HEIGHT && vy < 0 )
	{
		mVelocity = XMVectorSetY( mVelocity, -vy * mWallRestitution );
		SetLocation( XMVectorSetY( location, -HALF_WORLD_HEIGHT + radius ) );
	}

	if( ( x + radius ) >= HALF_WORLD_WIDTH && vx > 0 )
	{
		mVelocity = XMVectorSetX( mVelocity, -vx * mWallRestitution );
		SetLocation( XMVectorSetX( location, HALF_WORLD_WIDTH - radius ) );
	}
	else if( ( x - radius ) <= -HALF_WORLD_WIDTH && vx < 0 )
	{
		mVelocity = XMVectorSetX( mVelocity, -vx * mWallRestitution );
		SetLocation( XMVectorSetX( location, -HALF_WORLD_WIDTH + radius ) );
	}
}

void Ship::Write( MemoryOutputStream& inStream ) const
{
	if( inStream.GetRemainingBytes() >= 3 * sizeof( float ) + sizeof( float ) + sizeof( XMVECTOR ) + sizeof( uint32_t ) + sizeof( uint8_t ) )
	{
		XMVECTORF32 location; location.v = GetLocation();
		inStream.Write( location.f[ 0 ] );
		inStream.Write( location.f[ 1 ] );

		inStream.Write( mIsThrusting );

		inStream.Write( GetRotation() );
		inStream.Write( GetColor() );
		inStream.Write( GetPlayerId() );

	}
}

void Ship::Read( MemoryInputStream& inStream )
{
	if( inStream.GetRemainingBytes() >= 3 * sizeof( float ) + sizeof( XMVECTOR ) + sizeof( uint32_t ) + sizeof( uint8_t ) )
	{
		XMVECTORF32 location = { 0 };
		inStream.Read( location.f[ 0 ] );
		inStream.Read( location.f[ 1 ] );
		SetLocation( location );

		inStream.Read( mIsThrusting );

		float rotation;
		inStream.Read( rotation );
		SetRotation( rotation );

		XMVECTOR color;
		inStream.Read( color );
		SetColor( color );

		inStream.Read( mPlayerId );



	}
}
