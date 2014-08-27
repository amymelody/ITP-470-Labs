class Ship : public GameObject
{
public:

		static	GameObject*	StaticCreate()		{ return new Ship(); }
	
		virtual uint32_t GetFourCCName() const override { return 'SHIP'; }

		virtual	Ship*	GetAsShip()	{ return this; }

		virtual void Update()	override;

		void SetIsLocallyControlled( bool inIsLocallyControlled )	{ mIsLocallyControlled = inIsLocallyControlled; }

		void ProcessInput( float inDeltaTime, const InputState& inInputState );
		void SimulateMovement( float inDeltaTime );

		void ProcessCollisions();
		void ProcessCollisionsWithScreenWalls();

		void		SetPlayerID( uint32_t inPlayerID )			{ mPlayerID = inPlayerID; }
		uint32_t	GetPlayerID()						const 	{ return mPlayerID; }



protected:
	Ship();


private:

	XMVECTOR			mVelocity;


	float				mMaxDrivenLinearSpeed;
	float				mMaxDrivenLinearAcceleration;
	float				mMaxDrivenRotationSpeed;

	TexturePtr			mInactiveTexture;
	TexturePtr			mThrustingTexture;


	//bounce fraction when hitting various things
	float				mWallRestitution;
	float				mShipRestitution;

	//whether this host is in control of this ship
	bool				mIsLocallyControlled;

	//whether input has told us we're thrusting
	bool				mIsThrusting;

	uint32_t			mPlayerID;

	SpriteComponentPtr	mSpriteComponent;

};

typedef shared_ptr< Ship >	ShipPtr;