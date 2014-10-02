class GameObject
{
public:


	GameObject();
	virtual ~GameObject() {}

	void* operator new( size_t inSize );
	void operator delete( void * inP );

	virtual	Ship*	GetAsShip()	{ return nullptr; }

	virtual uint32_t GetFourCCName() const = 0;

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

			int			GetNetworkId()				const				{ return mNetworkId; }
			void		SetNetworkId( int inNetworkId );

	virtual void		Write( MemoryOutputStream& inStream ) const	{  ( void ) inStream; }
	virtual void		Read( MemoryInputStream& inStream )			{ ( void ) inStream; }

private:


	XMVECTOR										mLocation;
	XMVECTOR										mColor;

	float											mRealWidth;
	
	float											mCollisionRadius;


	float											mRotation;
	float											mScale;
	int												mIndexInWorld;

	bool											mDoesWantToDie;

	int												mNetworkId;

};

typedef shared_ptr< GameObject >	GameObjectPtr;