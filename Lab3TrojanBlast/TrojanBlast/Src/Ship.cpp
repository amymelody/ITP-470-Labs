#include <TrojanBlastPCH.h>
#include "Ship.h"
#include "Timing.h"
#include "World.h"

using namespace DirectX;


Ship::Ship() :
	GameObject(),
	mMaxDrivenRotationSpeed( 5.f ),
	mMaxDrivenLinearSpeed( 5.f ),
	mMaxDrivenLinearAcceleration( 2.f ),
	mVelocity( XMVectorZero() ),
	mWallRestitution( 0.3f ),
	mShipRestitution( 0.9f ),
	mIsThrusting( false ),
	mPlayerID( 0 )
{
	SetCollisionRadius( 0.4f );
}

bool Ship::Write(PacketBuffer* inPacketBuffer) const {
	if (!inPacketBuffer->WriteVector(mLocation)) {
		return false;
	}
	if (!inPacketBuffer->WriteVector(mColor)) {
		return false;
	}
	uint32_t id = mPlayerID;
	if (!inPacketBuffer->WriteInt(id)) {
		return false;
	}
	if (!inPacketBuffer->WriteFloat(mRotation)) {
		return false;
	}
	if (!inPacketBuffer->WriteBool(mIsThrusting)) {
		return false;
	}
	return true;
}

bool Ship::Read(PacketBuffer* inPacketBuffer) {
	if (!inPacketBuffer->ReadVector(mLocation)) {
		return false;
	}
	if (!inPacketBuffer->ReadVector(mColor)) {
		return false;
	}
	if (!inPacketBuffer->ReadInt(mPlayerID)) {
		return false;
	}
	if (!inPacketBuffer->ReadFloat(mRotation)) {
		return false;
	}
	if (!inPacketBuffer->ReadBool(mIsThrusting)) {
		return false;
	}
	return true;
}