#include "engine/game/Game.hpp"
#include "engine/game/states/HorizontalSpring.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/core/device.hpp"
#include "engine/graphics/core/shader.hpp"
#include "engine/graphics/core/texture.hpp"
#include "engine/graphics/renderer/mesh.hpp"
#include "engine/graphics/renderer/meshrenderer.hpp"
#include "engine/input/inputmanager.hpp"
#include "engine/math/directions.h"
#include "engine/utils/meshloader.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <map>
#include <thread>

// CRT's memory leak detection
#ifndef NDEBUG
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <engine/game/Game.hpp>
#endif
#include <engine/game/states/HorizontalSpring.hpp>
#include <engine/game/states/MovingPlanets.hpp>
#include <engine/game/states/Start.hpp>

int main(int argc, char *argv[]) {

#ifndef NDEBUG
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	_CrtSetBreakAlloc(2760);
#endif
#endif

	game::Game game;
	game::Start test;
	game::MovingPlanets test2;
	game::HorizontalSpring test3;
	game.run(std::make_unique<game::Start>(test));					// "startmenue" then spring			flyer set to unmovable position
	//game.run(std::make_unique<game::MovingPlanets>(test2));		//run this for planets				flyer free
	//game.run(std::make_unique<game::HorizontalSpring>(test3));	//run this for spring				flyer set to unmovable position
}
