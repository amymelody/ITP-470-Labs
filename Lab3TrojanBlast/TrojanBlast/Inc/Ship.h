#include "GameObject.h"

class Ship : public GameObject
{
public:
	
		virtual uint32_t GetFourCCName() const override { return 'SHIP'; }

		virtual	Ship*	GetAsShip()	{ return this; }

		virtual void Update() {};

		void SetIsLocallyControlled( bool inIsLocallyControlled )	{ mIsLocallyControlled = inIsLocallyControlled; }

		void SimulateMovement( float inDeltaTime );

		void ProcessCollisions();
		void ProcessCollisionsWithScreenWalls();

		void		SetPlayerID( uint32_t inPlayerID )			{ mPlayerID = inPlayerID; }
		uint32_t	GetPlayerID()						const 	{ return mPlayerID; }



protected:

	Ship();

	XMVECTOR			mVelocity;


	float				mMaxDrivenLinearSpeed;
	float				mMaxDrivenLinearAcceleration;
	float				mMaxDrivenRotationSpeed;


	//bounce fraction when hitting various things
	float				mWallRestitution;
	float				mShipRestitution;

	//whether this host is in control of this ship
	bool				mIsLocallyControlled;

	//whether input has told us we're thrusting
	bool				mIsThrusting;

	uint32_t			mPlayerID;

};

typedef shared_ptr< Ship >	ShipPtr;