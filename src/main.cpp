#include <engine/game/registry/Entity.hpp>
#include "engine/game/registry/Registry.hpp"
#include "engine/game/registry/Registry.cpp"
#include <iostream>
#include <map>


template<typename T>
struct Printer {
	void operator()(T &_data) const {
		std::cout << _data << ", ";
	}
};

template<typename T>
struct PlusTwo {
	void operator()(T &_data) {
		_data += 2;
	}
};

template<typename T>
struct TimesThree {
	void operator()(T &_data) {
		_data *= 3;
	}
};

template<typename T>
struct Sum {
	Sum() {
		sum = 0;
	}
	int sum;
	void operator()(T &_data) {
		sum += _data;
		std::cout << sum << ", ";
	}
};

int main(int argc, char *argv[]) {
	/*
	game::Game game;
	game::Start test;
	game.run(std::make_unique<game::Start>(test));
	*/

	game::Registry<int> registry;
	Entity a = registry.create();
	registry.setData(a, 12);
	Entity b = registry.create();
	registry.setData(b, 88);
	Entity c = registry.create();
	registry.setData(c, 227);

	registry.execute(Printer<int>());
	std::cout << "\n";
	registry.execute(Sum<int>());
	std::cout << "\n";
	registry.execute(PlusTwo<int>());
	registry.execute(Printer<int>());
	std::cout << "\n";
	registry.execute(TimesThree<int>());
	registry.execute(Printer<int>());
	std::cout << "\n";

	return EXIT_SUCCESS;
}
