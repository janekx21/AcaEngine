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
#include <engine/game/Registry.hpp>
#include <engine/game/Registry.cpp>
#include <iostream>

static int testsFailed = 0;

# define EXPECT(cond,description)										\
do {																	\
	if (!(cond)) {														\
	  std::cerr << "FAILED "											\
				<< description << std::endl <<"       " << #cond		\
				<< "\n"													\
				<< "       " << __FILE__ << ':' << __LINE__				\
				<< std::endl;											\
	  ++testsFailed;													\
	}																	\
} while (false)

struct Foo {
	int i;
};

struct Bar {
	float f;
};
int main(int argc, char *argv[]) {

#ifndef NDEBUG
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(6429);
#endif
#endif

	//game::Game game;
	//game::Start test;
	//game::MovingPlanets test2;
	//game::HorizontalSpring test3;
	//game.run(std::make_unique<game::Start>(test));					//"startmenue" then spring			flyer set to unmovable position
	//game.run(std::make_unique<game::MovingPlanets>(test2));				//run this for planets				flyer free
	//game.run(std::make_unique<game::HorizontalSpring>(test3));		//run this for spring				flyer set to unmovable position

	//game.~Game();
	
	Registry<int> registry;

	using namespace game;


	std::vector<Entity> entities;

	for (int i = 0; i < 5; ++i)
		entities.push_back(registry.create());

	auto refDel = registry.getRef(entities[2]);

	EXPECT(registry.getEntity(refDel), "Reference is valid after creation.");
	registry.erase(entities[2]);
	EXPECT(!registry.getEntity(refDel), "Reference is invalid after delete.");

	entities[2] = registry.create();
	for (int i = 0; i < 6; ++i)
		entities.push_back(registry.create());

	EXPECT(!registry.getEntity(refDel), "Reference remains invalid after reuse of the id.");

	for (int i = 0; i < static_cast<int>(entities.size()); ++i)
	{
		const Foo& foo = registry.addComponent<Foo>(entities[i], i);
		EXPECT(foo.i == i, "Add a component.");
		if (i % 3 == 0)
		{
			const Bar& bar = registry.addComponent<Bar>(entities[i], static_cast<float>(i));
			EXPECT(bar.f == static_cast<float>(i), "Add a component.");
		}
	}

	for (int i = 0; i < static_cast<int>(entities.size()); ++i)
	{
		EXPECT(registry.getComponent<Foo>(entities[i]), "Retrieve a component.");
		EXPECT(registry.getComponentUnsafe<Foo>(entities[i]).i == i, "Retrieve a component.");
	}

	registry.removeComponent<Foo>(entities[0]);
	registry.removeComponent<Foo>(entities[1]);

	EXPECT(!registry.getComponent<Foo>(entities[0]), "Remove a component.");
	EXPECT(!registry.getComponent<Foo>(entities[1]), "Remove a component.");

	registry.erase(entities[2]);

	for (int i = 3; i < static_cast<int>(entities.size()); ++i)
	{
		EXPECT(registry.getComponent<Foo>(entities[i]), "Other components are untouched.");
		EXPECT(registry.getComponentUnsafe<Foo>(entities[i]).i == i, "Other components are untouched.");
	}

	int sum = 0;
	registry.execute([&sum](const Foo& foo) { sum += foo.i; });
	// without auto deduction
	// registry.execute<Foo>([&sum](const Foo& foo) { sum += foo.i; });
	EXPECT(sum == 10 * 11 / 2 - 3, "Execute action on a single component type.");

	sum = 0;
	registry.execute([&sum](const Bar& bar, const Foo& foo) { sum += foo.i - 2 * static_cast<int>(bar.f); });
	//registry.execute<Bar,Foo>([&sum](const Bar& bar, const Foo& foo) { sum += foo.i - 2 * static_cast<int>(bar.f); });
	EXPECT(sum == -3 - 6 - 9, "Execute action on multiple component types.");

	// registry.execute<Entity, Bar>([&](Entity ent, Bar& bar)
	registry.execute([&](Entity ent, Bar& bar)
		{
			EXPECT(registry.getComponentUnsafe<Bar>(ent).f == bar.f, "Execute provides the correct entity.");
			bar.f = -1.f;
		});

	for (size_t i = 3; i < entities.size(); i += 3)
	{
		EXPECT(registry.getComponentUnsafe<Bar>(entities[i]).f == -1.f, "Action can change components.");
	}
	/*
	Entity a = registry.create();
	registry.setData(a, 12);
	Entity b = registry.create();
	registry.setData(b, 88);
	Entity c = registry.create();
	registry.setData(c, 227);

	registry.execute(Printer<int>());
	std::cout << "\n";
	Sum<int> sum;
	
	registry.execute(sum);
	
	std::cout << sum.sum;
	registry.execute(PlusTwo<int>());
	registry.execute(Printer<int>());
	std::cout << "\n";
	registry.execute(TimesThree<int>());
	registry.execute(Printer<int>());
	std::cout << "\n";
	*/
}
