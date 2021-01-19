#pragma once
#include "Archetype.hpp"
#include "Entity.hpp"
#include "EntityRef.hpp"
#include "component_type.hpp"
#include <algorithm>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

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


		/**
		 * Add a new component to an existing entity. No changes are done if Component
		 * if _entity already has a component of this type.
		 * @tparam Component
		 * @tparam Args
		 * @param _entity
		 * @param _args
		 * @return A reference to the new component or the already existing component.
		 */
		template<component_type Component, typename... Args>
		Component &addComponent(Entity _entity, Args &&..._args) {
			auto types = entityToArchetype[_entity]->types;

			// add component
			types.push_back(typeid(Component).hash_code());

			// removed duplicates
			std::sort(types.begin(), types.end());
			types.erase(std::unique(types.begin(), types.end()), types.end());

			auto hash = Archetype<0>::hashCode(types);

			auto& oldEntities = entityToArchetype[_entity]->entities;
			auto it = std::find(oldEntities.begin(), oldEntities.end(), _entity);
			if (it == oldEntities.end()) {
				throw std::exception("Entity not found");
			}
			int pos = it - oldEntities.begin();
			auto& foo = entityToArchetype[_entity]->components.at(pos);
			entityToArchetype[_entity]->components.erase(pos);
			oldEntities.erase(pos);

			for(auto& array : entityToArchetype[_entity]->components) {
			}
			// oldEntities.erase(std::remove(oldEntities.begin(), oldEntities.end(), _entity), oldEntities.end());

			if (!archetypes.contains(hash)) {
				Archetype archetype = {types, std::vector<ComponentArray<10>>(), std::vector<Entity>()};
				archetypes[hash] = archetype;
			}
			archetypes.at(hash).entities.push_back(_entity);

			auto& component = Component(_args...);
			archetypes.at(hash).components.push_back(/* TODO */);

			// TODO
			// Work in Progress

			/*
			if (std::find(types.begin(), types.end(), typeid(Component).hash_code()) != types.end()) {
				// component already added
				// or entity has the right archetype already
			} else {
				// needs new archetype

				archetypes[types.h]

								std::find_if(archetypes.begin(), archetypes.end(), [](Archetype it) {
									return std::find_if(it.types.begin(), it.types.end(), [](size_t hash) {
										return hash = typeid(Componentl).hash_code()
									});
								});
			};

			Archetype foo = {};
			archetypes.push_back(foo);
			 */
		}

	private:
		std::vector<bool> idUsed;
		std::vector<uint32_t> generations;
		std::vector<T> data;
		std::unordered_map<size_t, Archetype> archetypes;
		std::unordered_map<Entity, Archetype *> entityToArchetype;
	};
}// namespace game
