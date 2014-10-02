class ClientProxy
{
public:

	ClientProxy( const SocketAddress& inSocketAddress, const wstring& inName, int inPlayerId );

	const	SocketAddress&	GetSocketAddress()	const	{ return mSocketAddress; }
			int				GetPlayerId()		const	{ return mPlayerId; }
	const	wstring&		GetName()			const	{ return mName; }

			void			SetInputState( const InputState& inInputState )		{ mInputState = inInputState; }
	const	InputState&		GetInputState()		const	{ return mInputState; }

			void			UpdateLastPacketTime();
			float			GetLastPacketFromClientTime()	const	{ return mLastPacketFromClientTime; }

			DeliveryNotificationManager&	GetDeliveryNotificationManager()	{ return mDeliveryNotificationManager; }
private:

	DeliveryNotificationManager	mDeliveryNotificationManager;

	SocketAddress	mSocketAddress;
	wstring			mName;
	int				mPlayerId;

	InputState		mInputState;

	float			mLastPacketFromClientTime;
	float			mLastLogStatTime;
};

typedef shared_ptr< ClientProxy >	ClientProxyPtr;