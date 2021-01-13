#include <map>
#include "engine/game/Game.hpp"
#include "engine/game/states/HorizontalSpring.hpp"

// CRT's memory leak detection
#ifndef NDEBUG
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#endif

int main(int argc, char *argv[]) {
#ifndef NDEBUG
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif
	game::Game game;
	game::HorizontalSpring test;
	game.run(std::make_unique<game::HorizontalSpring>(test));
	return EXIT_SUCCESS;
}
