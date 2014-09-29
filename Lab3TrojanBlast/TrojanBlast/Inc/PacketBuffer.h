#pragma once
class PacketBuffer
{
public:
	PacketBuffer();
	~PacketBuffer();

	char* mBuffer;
	uint32_t mBufferCapacity;
	uint32_t mBufferHead;

	bool WriteData(const void* inData, size_t inLength);
	bool ReadData(void* outData, size_t outLength);
	bool WriteString(const string& inString);
	bool ReadString(string& outString);
	bool WriteFloat(const float& inFloat);
	bool ReadFloat(float& outFloat);
	bool WriteBool(const bool& inBool);
	bool ReadBool(bool& outBool);
	bool WriteVector(const XMVECTOR& inVector);
	bool ReadVector(XMVECTOR& outVector);
	bool WriteVectorF32(const XMVECTORF32& inVector);
	bool ReadVectorF32(XMVECTORF32& outVector);
	bool WriteInt(uint32_t& inInt);
	bool ReadInt(uint32_t& outInt);
};

