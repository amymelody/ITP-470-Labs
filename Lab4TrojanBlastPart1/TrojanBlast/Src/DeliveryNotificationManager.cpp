#include <TrojanBlastPCH.h>



namespace
{
	const float kDelayBeforeAckTimeout = 0.5f;
}

DeliveryNotificationManager::DeliveryNotificationManager( bool inShouldSendAcks, bool inShouldProcessAcks ) :
	mNextOutgoingSequenceNumber( 0 ),
	mNextExpectedSequenceNumber( 0 ),
	//everybody starts at 0...
	mShouldSendAcks( inShouldSendAcks ),
	mShouldProcessAcks( inShouldProcessAcks ),
	mDeliveredPacketCount( 0 ),
	mDroppedPacketCount( 0 ),
	mDispatchedPacketCount( 0 )
{
}

void DeliveryNotificationManager::LogStats( const wstring& inName )
{
	LOG(L"Client '%ls' Delivery rate %d%%, Drop rate %d%%",
		inName.c_str(),
		(100 * mDeliveredPacketCount) / mDispatchedPacketCount,
		(100 * mDroppedPacketCount) / mDispatchedPacketCount);
}



DeliveryNotificationManager::InFlightPacket::InFlightPacket( SequenceNumber inSequenceNumber ) : 
mSequenceNumber( inSequenceNumber ), 
mTimeDispatched( Timing::sInstance.GetTimef() )
{
	//null out other transmision data params...
}


/*
lab4
This should write the next outgoing sequence number to the packet and imcrement it.  
Additionally, if mShouldProcessAcks is true, it should create a new InFlightPacket, 
add it to the mInFlightPackets list and return it, so that in Part 2 you can store Transmission data in it
*/
DeliveryNotificationManager::InFlightPacket* DeliveryNotificationManager::WriteSequenceNumber( MemoryOutputStream& inMemoryStream )
{
	++mDispatchedPacketCount;

	return nullptr;
}

/*
lab4
This function writes ack data into the packet.
Efficiently write data into the packet to indicate whether you have any acks in mPendingAcks or not.
If you do, write the first AckRange in mPendingAcks and remove it from the list
*/
void DeliveryNotificationManager::WriteAckData( MemoryOutputStream& inMemoryStream )
{
	
}



/*
lab4
This function should read a sequence number from the packet and respond appropriately.
It should use and update mNextExpectedSequenceNumber.
If the sequence number is as expected, add an ack to the pending ack list by calling AddPendingAck.
If the sequence number is too low, silently drop it ( we don’t reuse sequence numbers, so we don’t have to ack it )
If the sequence number is too high, it means some packets were dropped.  Adjust your expected sequence number appropriately, and do whatever else is appropriate.
Return true if the packet should be processed, or false if it should be ignored
*/
bool DeliveryNotificationManager::ProcessSequenceNumber( MemoryInputStream& inMemoryStream )
{
	return true;
}


/*
lab4: check if the packet has acks in it.  if so, read the AckRange.  And respond appropriately.
For our current strategy, let’s assume that if any inflight packet has a sequence number LESS than any ACK that comes in, then we consider the packet dropped.
Call HandlePacketDeliveryFailure  to report a dropped packet.
Make sure to clear out the inflightpackets  list of any packets you can.
If a packet is successfully delivered, make sure to increment mDeliveredPacketCount.
*/
void DeliveryNotificationManager::ProcessAcks( MemoryInputStream& inMemoryStream )
{
	
}

//lab4: run through the inflight packets and report any as dropped that aren't ack'd within kDelayBeforeAckTimeout
void DeliveryNotificationManager::ProcessTimedOutPackets()
{
	
}

void DeliveryNotificationManager::AddPendingAck( SequenceNumber inSequenceNumber )
{
	//if you don't have a range yet, or you can't correctly extend the final range with the sequence number,
	//start a new range
	if( mPendingAcks.size() == 0 || !mPendingAcks.back().ExtendIfShould( inSequenceNumber ) )
	{
		mPendingAcks.emplace_back( inSequenceNumber );
	}
}


void DeliveryNotificationManager::HandlePacketDeliveryFailure( const DeliveryNotificationManager::InFlightPacket& inFlightPacket ) const
{
	( void ) inFlightPacket;
	//look at your data, tell people who care
	//LOG( L"Server Failed to deliver packet number %d", inFlightPacket.GetSequenceNumber() );
	++mDroppedPacketCount;
}

/*lab4: Write your ack range here. the count is usually going to be 1 ( we could do statistical analysis to be sure )
but for now assume it is.  Also, we don't want to use too much space sending acks, so only use 8 bits at most to send the ack count
*/
void DeliveryNotificationManager::AckRange::Write( MemoryOutputStream& inMemoryStream ) const
{

}

void DeliveryNotificationManager::AckRange::Read( MemoryInputStream& inMemoryStream )
{

}