
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <WinSock2.h>

#include <memory>
#include <DirectXMath.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <list>
#include <queue>
#include <deque>

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::list;
using std::deque;
using std::unordered_map;
using std::string;
using std::wstring;

using namespace DirectX;

class Ship;
class GameObject;

#include <SocketAddress.h>
#include <PacketBuffer.h>
#include <UDPSocket.h>
#include <UDPSocketUtil.h>

#include <DeliveryNotificationManager.h>

#include <InputAction.h>
#include <InputState.h>
#include <GameObject.h>
#include <GameObjectRegistry.h>
#include <Ship.h>
#include <World.h>
#include <Timing.h>
#include <TommyCoin.h>
#include <MathUtils.h>
#include <StringUtils.h>
#include <ScoreBoardManager.h>

#include <WeightedTimedMovingAverage.h>
#include <NetworkManager.h>
#include <Engine.h>

#include <ReplicationCommand.h>