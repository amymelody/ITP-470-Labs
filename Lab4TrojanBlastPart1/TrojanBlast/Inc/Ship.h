class Ship : public GameObject
{
public:

		static	GameObject*	StaticCreate()		{ return new Ship(); }
	
		virtual uint32_t GetFourCCName() const override { return 'SHIP'; }

		virtual	Ship*	GetAsShip()	{ return this; }

		virtual void Update()	override;

		void ProcessInput( float inDeltaTime, const InputState& inInputState );
		void SimulateMovement( float inDeltaTime );

		void ProcessCollisions();
		void ProcessCollisionsWithScreenWalls();

		void		SetPlayerId( uint32_t inPlayerId )			{ mPlayerId = inPlayerId; }
		uint32_t	GetPlayerId()						const 	{ return mPlayerId; }

		virtual void		Write( MemoryOutputStream& inStream ) const override;
		virtual void		Read( MemoryInputStream& inStream ) override;

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