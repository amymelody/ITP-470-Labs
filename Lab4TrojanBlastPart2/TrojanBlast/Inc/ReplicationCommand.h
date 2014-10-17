enum EReplicatedObjectAction
{
	EROA_UpdateState,
	EROA_Create,
	EROA_Destroy
};

class ReplicationManagerTransmissionData;

struct ReplicationCommand
{
public:

	ReplicationCommand() {}
	ReplicationCommand( uint32_t inInitialDirtyState ) : mAction( EROA_Create ), mDirtyState( inInitialDirtyState ) {}

	//if the create is ack'd, we can demote to just an update...
	void HandleCreateAckd()							{ if( mAction == EROA_Create ) { mAction = EROA_UpdateState; } }
	void AddDirtyState( uint32_t inState )			{ mDirtyState |= inState; }
	void SetDestroy()								{ mAction = EROA_Destroy; }

	bool					HasDirtyState() const	{ return ( mAction == EROA_Destroy ) || ( mDirtyState != 0 ); }

	EReplicatedObjectAction	GetAction()	const							{ return mAction; }
	uint32_t				GetDirtyState() const						{ return mDirtyState; }
	inline void				ClearDirtyState( uint32_t inStateToClear );

	//write is not const because we actually clear the dirty state after writing it....
	void Write( OutgoingPacketBuffer& inPacket, int inNetworkId, ReplicationManagerTransmissionData* ioTransactionData );
	void Read( IncomingPacketBuffer& inPacket, int inNetworkId );

private:

	uint32_t				mDirtyState;
	EReplicatedObjectAction	mAction;
};

//don't clear a create- just keep sending it for now
//this is to make sure we never send an update before teh initial create comes across with all data
//alternative is to be able to skip packet data
inline void	 ReplicationCommand::ClearDirtyState( uint32_t inStateToClear )
{ 
	//if( mAction != EROA_Create )
	{
		mDirtyState &= ~inStateToClear; 
	}
	
	if( mAction == EROA_Destroy ) 
	{
		mAction = EROA_UpdateState; 
	} 
}
