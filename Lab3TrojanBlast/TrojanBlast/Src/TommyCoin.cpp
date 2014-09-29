#include <TrojanBlastPCH.h>
#include "TommyCoin.h"
#include "Ship.h"
#include "ScoreBoardManager.h"

TommyCoin::TommyCoin()
{
	SetScale( GetScale() * 0.5f );
	SetCollisionRadius( 0.25f );
}

bool TommyCoin::Write(PacketBuffer* inPacketBuffer) const {
	return inPacketBuffer->WriteVector(mLocation);
}

bool TommyCoin::Read(PacketBuffer* inPacketBuffer) {
	return inPacketBuffer->ReadVector(mLocation);
}
