class Ship : public GameObject
{
public:
		enum EShipReplicationState
		{
			ESRS_Location	= 1 << 0,
			ESRS_Rotation		= 1 << 1,
			ESRS_Color			= 1 << 2,
			ESRS_PlayerId		= 1 << 3,

			ESRS_AllState	= ESRS_Location | ESRS_Rotation | ESRS_Color | ESRS_PlayerId
		};


		static	GameObject*	StaticCreate()			{ return new Ship(); }

		virtual uint32_t GetAllStateMask()	const override	{ return ESRS_AllState; }


		virtual uint32_t GetFourCCName() const override { return 'SHIP'; }

		virtual	Ship*	GetAsShip()	{ return this; }

		virtual void Update()	override;

		void ProcessInput( float inDeltaTime, const InputState& inInputState );
		void SimulateMovement( float inDeltaTime );

		void ProcessCollisions();
		void ProcessCollisionsWithScreenWalls();

		void		SetPlayerId( uint32_t inPlayerId )			{ mPlayerId = inPlayerId; }
		uint32_t	GetPlayerId()						const 	{ return mPlayerId; }

		virtual uint32_t	Write( OutgoingPacketBuffer& inPacket, uint32_t inDirtyState ) const override;
		virtual void		Read( IncomingPacketBuffer& inPacket ) override;

protected:
	Ship();

private:

	XMVECTOR			mVelocity;


	float				mMaxDrivenLinearSpeed;
	float				mMaxDrivenLinearAcceleration;
	float				mMaxDrivenRotationSpeed;

	//bounce fraction when hitting various things
	float				mWallRestitution;
	float				mShipRestitution;


	uint32_t			mPlayerId;

protected:
	///move down here for padding reasons...
	

	//whether input has told us we're thrusting
	bool				mIsThrusting;



};

typedef shared_ptr< Ship >	ShipPtr;