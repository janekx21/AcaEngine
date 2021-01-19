#include "Registry.hpp"

namespace game {
	
	Entity Registry::create()
	{
		uint32_t pos ;
		uint32_t id;
		for (id = 0; id < flags.size(); id++) {
			if (flags[id] == 0) {
				flags[id] = 1;
				generations[id] += 1;
				Entity new_entity = { id, pos };
				return new_entity;
			}
		}
		flags.push_back(1);
		generations.push_back(1);
		Entity new_entity = { id, pos };
		if (archetypes.empty()) {
			std::vector <size_t> zero_types;
			std::vector < std::vector<std::any>> zero_archetype_components;
			bool zero_archetype_is_empty = false;
			Archetype zero_archetype = { zero_types, zero_archetype_components, zero_archetype_is_empty };
			archetypes.push_back(zero_archetype);
			entityArchetypeMap[new_entity] = zero_archetype;
		}
		return new_entity;
	}
	
	EntityRef Registry::getRef(Entity _ent) const
	{
		return { _ent, generations[_ent.id] };
	}
	std::optional<Entity> Registry::getEntity(EntityRef _ent) const
	{
		if (_ent.generation == generations[_ent.ent.id] && flags[_ent.ent.id]) {
			return _ent.ent;
		}
	}
	void Registry::erase(Entity _ent)
	{
		flags[_ent.id] = 0;
	}
}
