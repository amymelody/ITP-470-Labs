#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "PacketBuffer.h"

class InputState
{
public:

	InputState() :
	mDesiredRightAmount( 0 ),
	mDesiredLeftAmount( 0 ),
	mDesiredForwardAmount( 0 ),
	mDesiredBackAmount( 0 )
	{}

	float GetDesiredHorizontalDelta()	const { return mDesiredRightAmount - mDesiredLeftAmount; }
	float GetDesiredVerticalDelta()		const { return mDesiredForwardAmount - mDesiredBackAmount; }

	bool Write(PacketBuffer* inPacketBuffer) const;
	bool Read(PacketBuffer* inPacketBuffer);

private:
	friend class InputManager;

	float	mDesiredRightAmount, mDesiredLeftAmount;
	float	mDesiredForwardAmount, mDesiredBackAmount;

};

#endif