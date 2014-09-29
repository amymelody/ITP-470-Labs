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

bool PacketBuffer::WriteInt(uint32_t& inInt) {
	return WriteData(&inInt, 4);
}

bool PacketBuffer::ReadInt(uint32_t& outInt) {
	return ReadData(&outInt, 4);
}

bool PacketBuffer::WriteVector(const XMVECTOR& inVector) {
	return WriteData(&inVector, sizeof(XMVECTOR));
}

bool PacketBuffer::ReadVector(XMVECTOR& outVector) {
	return ReadData(&outVector, sizeof(XMVECTOR));
}

bool PacketBuffer::WriteVectorF32(const XMVECTORF32& inVector) {
	return WriteData(&inVector, sizeof(XMVECTORF32));
}

bool PacketBuffer::ReadVectorF32(XMVECTORF32& outVector) {
	return ReadData(&outVector, sizeof(XMVECTORF32));
}

bool PacketBuffer::WriteBool(const bool& inBool) {
	return WriteData(&inBool, 1);
}

bool PacketBuffer::ReadBool(bool& outBool) {
	return ReadData(&outBool, 1);
}

bool PacketBuffer::WriteFloat(const float& inFloat) {
	return WriteData(&inFloat, 4);
}

bool PacketBuffer::ReadFloat(float& outFloat) {
	return ReadData(&outFloat, 4);
}

bool PacketBuffer::WriteString(const string& inString) {
	size_t length = inString.size();
	if (length < 65535)
	{
		uint16_t l = static_cast< uint16_t >(length);
		WriteData(&l, 2);
		return WriteData(&inString[0], length);
	}
	return false;
}

bool PacketBuffer::ReadString(string& outString) {
	uint16_t length;
	if (ReadData(&length, 2))
	{
		outString.resize(length);
		return ReadData(&outString[0], length);
	}
	return false;
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
		mBufferHead += outLength;
		return true;
	}
	else {
		return false;
	}
}