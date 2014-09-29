#include <TrojanBlastPCH.h>
#include "InputState.h"
#include "StringUtils.h"

bool InputState::Write(PacketBuffer* inPacketBuffer) const {
	if (!inPacketBuffer->WriteFloat(mDesiredRightAmount)) {
		return false;
	}
	if (!inPacketBuffer->WriteFloat(mDesiredLeftAmount)) {
		return false;
	}
	if (!inPacketBuffer->WriteFloat(mDesiredForwardAmount)) {
		return false;
	}
	if (!inPacketBuffer->WriteFloat(mDesiredBackAmount)) {
		return false;
	}
	return true;
}

bool InputState::Read(PacketBuffer* inPacketBuffer) {
	if (!inPacketBuffer->ReadFloat(mDesiredRightAmount)) {
		return false;
	}
	if (!inPacketBuffer->ReadFloat(mDesiredLeftAmount)) {
		return false;
	}
	if (!inPacketBuffer->ReadFloat(mDesiredForwardAmount)) {
		return false;
	}
	if (!inPacketBuffer->ReadFloat(mDesiredBackAmount)) {
		return false;
	}
	return true;
}