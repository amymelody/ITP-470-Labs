#include "GameObject.h"
#include "InputState.h"

class Ship : public GameObject
{
public:
	
		virtual uint32_t GetFourCCName() const override { return 'SHIP'; }

		virtual	Ship*	GetAsShip()	{ return this; }

		virtual void Update() {};

		void		SetPlayerID( uint32_t inPlayerID )			{ mPlayerID = inPlayerID; }
		uint32_t	GetPlayerID()						const 	{ return mPlayerID; }

		bool Write(PacketBuffer* inPacketBuffer) const;
		bool Read(PacketBuffer* inPacketBuffer);

		XMVECTOR			mVelocity;

		float				mShipRestitution;

protected:

	Ship();


	float				mMaxDrivenLinearSpeed;
	float				mMaxDrivenLinearAcceleration;
	float				mMaxDrivenRotationSpeed;


	//bounce fraction when hitting various things
	float				mWallRestitution;

	//whether input has told us we're thrusting
	bool				mIsThrusting;

	uint32_t			mPlayerID;

};

typedef shared_ptr< Ship >	ShipPtr;