#include "Registry.hpp"
#include "component.hpp"

template<typename T>
Entity Registry<T>::create()
{
	uint32_t id;
	for (id = 0; id < flags.size(); id++) {
		if (flags[id] == 0) {
			flags[id] = 1;
			generations[id] += 1;
			data[id] = (T) 0;
			return Entity{ id };
		}
	}
	flags.push_back(1);
	generations.push_back(1);
	data.push_back((T) 0);
	return Entity{ id };
}

template<typename T>
void Registry<T>::erase(Entity _ent)
{
	flags[_ent.id] = 0;
	data[_ent.id] = (T) 0;
}

template<typename T>
EntityRef Registry<T>::getRef(Entity _ent) const
{
	return {_ent, generations[_ent.id]};
}

template<typename T>
std::optional<Entity> Registry<T>::getEntity(EntityRef _ent) const
{
	if (_ent.generation == generations[_ent.ent.id] && flags[_ent.ent.id]) {
		return _ent.ent;
	}
}

