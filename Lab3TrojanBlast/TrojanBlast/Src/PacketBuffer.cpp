#include "TrojanBlastPCH.h"
#include "PacketBuffer.h"


PacketBuffer::PacketBuffer()
{
	mBuffer = new char[1600];
	mBufferCapacity = 1600;
	mBufferHead = 0;
}


PacketBuffer::~PacketBuffer()
{
}

bool PacketBuffer::WriteData(const void* inData, size_t inLength) {
	if (inLength + mBufferHead <= mBufferCapacity) {
		memcpy(mBuffer + mBufferHead, inData, inLength);
		mBufferHead += inLength;
		return true;
	}
	else {
		return false;
	}
}

bool PacketBuffer::ReadData(void* outData, size_t outLength) {
	if (outLength + mBufferHead <= mBufferCapacity) {
		memcpy(outData, mBuffer + mBufferHead, outLength);
		return true;
	}
	else {
		return false;
	}
}