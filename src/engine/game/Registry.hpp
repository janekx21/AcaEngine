#pragma once
#include <optional>
#include <vector>
#include <iostream>
struct Entity
{
	uint32_t id;
};
struct EntityRef
{
	Entity ent;
	uint32_t generation;
};
template<typename T>
class Registry 
{ 
public:
	Entity create(); 
	void erase(Entity _ent); 
	EntityRef getRef(Entity _ent) const; 
	std::optional<Entity> getEntity(EntityRef _ent) const; 

	void setData(Entity _ent, const T& _value); 
	const T& getData(Entity _ent) const; 
	T& getData(Entity _ent); 

	template<typename FN>
	void execute(FN _fn); 

	std::vector<bool> flags;
	std::vector<uint32_t>generations;
	std::vector<T> data;
	
};

template<typename T>
struct Printer {
	void operator()(T& _data) const {
		std::cout << _data << ", ";
	}
};

template<typename T>
struct PlusTwo {
	void operator()(T& _data) {
		_data += 2;
	}
};

template<typename T>
struct TimesThree {
	void operator()(T& _data) {
		_data *= 3;
	}
};

template<typename T>
struct Sum {
	Sum() {
		sum = 0;
	}
	int sum;
	void operator()(T& _data) {
		sum += _data;
		std::cout << sum << ", ";
	}

};
