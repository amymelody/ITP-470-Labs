#include "GameObject.h"

class TommyCoin : public GameObject
{
public:

	virtual uint32_t GetFourCCName() const override { return 'COIN'; }

	virtual bool	HandleCollisionWithShip(Ship* inShip) { (void)inShip; return true; }

	bool Write(PacketBuffer* inPacketBuffer) const;
	bool Read(PacketBuffer* inPacketBuffer);

protected:
	TommyCoin();
};