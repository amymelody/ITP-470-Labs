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
};

