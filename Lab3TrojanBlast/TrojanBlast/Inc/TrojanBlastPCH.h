
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <d3d11.h>
#include <memory>
#include <DirectXMath.h>

#include <vector>
#include <unordered_map>
#include <string>


using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::string;
using std::wstring;

using namespace DirectX;

class Ship;
class GameObject;



#include <Texture.h>

#include <InputAction.h>
#include <GameObject.h>
#include <SpriteComponent.h>
#include <GameObjectRegistry.h>
#include <InputState.h>
#include <InputManager.h>
#include <Ship.h>
#include <World.h>
#include <RenderManager.h>
#include <Timing.h>
#include <HUD.h>
#include <TextureManager.h>
#include <GraphicsDriver.h>
#include <TommyCoin.h>
#include <MathUtils.h>
#include <StringUtils.h>
#include <ScoreBoardManager.h>
#include <WindowManager.h>
#include <Engine.h>