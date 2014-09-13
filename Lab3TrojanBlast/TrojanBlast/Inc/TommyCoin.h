#include "GameObject.h"

class TommyCoin : public GameObject
{
public:

	virtual uint32_t GetFourCCName() const override { return 'COIN'; }

	virtual bool	HandleCollisionWithShip( Ship* inShip ) override;

protected:
	TommyCoin();
};