class TommyCoin : public GameObject
{
public:

	enum ETommyCoinReplicationState
	{
		ETCRS_Pose		= 1 << 0,
		ETCRS_Color		= 1 << 1,

		ETCRS_AllState	= ETCRS_Pose | ETCRS_Color
	};

	static	GameObject*	StaticCreate() { return new TommyCoin(); }

	virtual uint32_t	GetAllStateMask()	const override	{ return ETCRS_AllState; }

	virtual uint32_t	GetFourCCName() const override { return 'COIN'; }

	virtual bool		HandleCollisionWithShip( Ship* inShip ) override;

	virtual uint32_t	Write( OutgoingPacketBuffer& inPacket, uint32_t inDirtyState ) const override;
	virtual void		Read( IncomingPacketBuffer& inPacket ) override;

protected:
	TommyCoin();

};