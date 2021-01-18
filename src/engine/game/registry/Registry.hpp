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
		void erase(Entity _entity);
		EntityRef getRef(Entity _entity) const;
		std::optional<Entity> getEntity(EntityRef _entityRef) const;

		void setData(Entity _entity, const T &_value);
		const T &getData(Entity _entity) const;
		T &getData(Entity _entity);

		template<typename FN>
		void execute(FN _function);

		std::vector<bool> idUsed;
		std::vector<uint32_t> generations;
		std::vector<T> data;
	};
}
