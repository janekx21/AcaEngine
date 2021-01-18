#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include "Entity.hpp"
#include "EntityRef.hpp"

namespace game {
	template<typename T>
	class Registry {
	public:
		Entity create();
		void erase(Entity _ent);
		EntityRef getRef(Entity _ent) const;
		std::optional<Entity> getEntity(EntityRef _ent) const;

		void setData(Entity _ent, const T &_value);
		const T &getData(Entity _ent) const;
		T &getData(Entity _ent);

		template<typename FN>
		void execute(FN _fn);

		std::vector<bool> flags;
		std::vector<uint32_t> generations;
		std::vector<T> data;
	};
}
