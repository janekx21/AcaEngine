#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include "Entity.hpp"
#include "EntityRef.hpp"
#include "Archetype.hpp"
#include <unordered_map>
#include <any>

template<typename T>
concept component_type = std::movable<T>;
namespace game {
	template<typename T>
	concept component_type = std::movable<T>;
	class Registry {

	public:
		  Entity create();
		  void erase(Entity _ent);
		  EntityRef getRef(Entity _ent) const;
		  std::optional<Entity> getEntity(EntityRef _ent) const;
		  // Add a new component to an existing entity. No changes are done if Component
		  // if_ent already has a component of this type.
		  // @return A reference to the new component or the already existing component.
		  template<component_type Component, typename... Args>
		  Component& addComponent(Entity _ent, Args&&... _args) {
			  
			  //////////////////////////////////////////////////////////
			  //CASE1 Does entities archetype contain component already?
			  int count_types = 0;
			  for (auto& it_types : entityArchetypeMap.at(_ent).types) {
				  if (it_types == typeid(Component).hash_code()) {
					  Component& component_reference = std::any_cast<Component&>(entityArchetypeMap.at(_ent).components[count_types][_ent.pos]);
					  return component_reference;
				  }
				  count_types++;
			  }

			  ////////////////////////////////////////////////////////////////
			  //CASE2 Need to move entity to new archetype that exists already

			  generations[_ent.id] += 1;

			  std::vector<size_t> entity_types;
			  for (auto& it_types : entityArchetypeMap.at(_ent).types) {
				  entity_types.push_back(it_types);
			  }
			  entity_types.push_back(typeid(Component).hash_code());
			  //TODO: check wether archetype exists
			  for (auto& it_archetypes : archetypes) {
				  bool archetype_exists = true;
				  for (auto& it_types_entity : entity_types) {
					  bool type_exists = false;
					  for (auto& it_types_archetype : it_archetypes.types) {
						  if (it_types_archetype == it_types_entity) {
							  type_exists = true;
							  break;
						  }
					  }
					  if (!type_exists) {
						  archetype_exists = false;
						  break;
					  }
				  }
				  if (archetype_exists) {				// move old data to existing archetype
					  bool moved_back = true;
					  bool archetype_empty = false;
					  for (int i = 0; i < entity_types.size() - 1; i++) {
						  for (int j = 0; j < it_archetypes.types.size(); j++) {
							  if (entity_types[i] == it_archetypes.types[j]) {
								  it_archetypes.components[j].push_back(entityArchetypeMap.at(_ent).components[i][_ent.pos]);
								  if (entityArchetypeMap.at(_ent).components[i][_ent.pos] == entityArchetypeMap.at(_ent).components[i].back()) {
									  if (entityArchetypeMap.at(_ent).components[i][_ent.pos] == entityArchetypeMap.at(_ent).components[i].begin()) {
										  archetype_empty = true;
									  }
									  entityArchetypeMap.at(_ent).components[i].pop_back();
									  moved_back = false;
								  }
								  else {
									  entityArchetypeMap.at(_ent).components[i][_ent.pos] = entityArchetypeMap.at(_ent).components[i].back();
								  }

							  }
						  }
					  }
					  if (moved_back) {
						  for (auto& it_ent : entityArchetypeMap) {
							  if (entityArchetypeMap.at[it_ent] == entityArchetypeMap.at(_ent) && _ent.pos == entityArchetypeMap.at(_ent).components[0].size() - 1) {
								  it_ent.pos = _ent.pos;
							  }
						  }
					  }

					  for (int j = 0; j < it_archetypes.types.size(); j++) {
						  if (typeid(Component).hash_code() == it_archetypes.types[j]) {
							  count_types = j;
							  it_archetypes.components[j].push_back(std::any(Component(_args ...)));
							  break;
						  }
					  }

					  if (archetype_empty) {
						  entityArchetypeMap.at(_ent).is_empty = true;
					  }

					  _ent.pos = it_archetypes.components[0].size() - 1;
					  entityArchetypeMap.at(_ent) = it_archetypes;
					  Component& component_reference = std::any_cast<Component&>(entityArchetypeMap.at(_ent).components[count_types][_ent.pos]);
					  return component_reference;
				  }

			  }
			  ////////////////////////////////////////////////////////////////////
			  //CASE3 Need to move entity to new archetype that doesn't exists yet

			  std::vector < std::vector<T>> new_archetype_components;
			  bool new_archetype_is_empty = false;
			  Archetype new_archetype = { entity_types; new_archetype_components; new_archetype_is_empty };

			  bool moved_back = true;
			  bool archetype_empty = false;

			  for (int i = 0; i < entity_types.size() - 1; i++) {
				  std::vector<T> new_type;
				  new_archetype.components.push_back(new_type);
				  new_archetype.components[i].push_back(entityArchetypeMap.at(_ent).components[i][_ent.pos]);
				  if (entityArchetypeMap.at(_ent).components[i][_ent.pos] == entityArchetypeMap.at(_ent).components[i].back()) {
					  if (entityArchetypeMap.at(_ent).components[i][_ent.pos] == entityArchetypeMap.at(_ent).components[i].begin()) {
						  archetype_empty = true;
					  }
					  entityArchetypeMap.at(_ent).components[i].pop_back();
					  moved_back = false;
				  }
				  else {
					  entityArchetypeMap.at(_ent).components[i][_ent.pos] = entityArchetypeMap.at(_ent).components[i].back();
				  }
			  }

			  if (moved_back) {
				  for (auto& it_ent : entityArchetypeMap) {
					  if (entityArchetypeMap.at[it_ent] == entityArchetypeMap.at(_ent) && _ent.pos == entityArchetypeMap.at(_ent).components[0].size() - 1) {
						  it_ent.pos = _ent.pos;
					  }
				  }
			  }
			  std::vector<T> new_type;
			  new_archetype.components.push_back(new_type);
			  new_archetype.components[new_archetype.components.size() - 1].push_back(std::any(Component(_args ...)));

			  if (archetype_empty) {
				  entityArchetypeMap.at(_ent).is_empty = true;
			  }

			  _ent.pos = 0;
			  entityArchetypeMap.at(_ent) = new_archetype;

			  archetypes.push_back(new_archetype);

			  Component& component_reference = std::any_cast<Component&>(entityArchetypeMap.at(_ent).components[entity_types.size() - 1][_ent.pos]);
			  return component_reference;
		  }
		  // Remove a component from an existing entity.// Does not check whether it exists.
		  template<component_type Component>
		  void removeComponent(Entity _ent) {}
		  // Retrieve a component associated with an entity.
		  // @return The component or nullptr if the entity has no such component.
		  template<component_type Component>
		  Component* getComponent(Entity _ent) {}template<component_type Component>const Component* getComponent(Entity _ent) const {}
		  // Retrieve a component associated with an entity.
		  // Does not check whether it exits.
		  template<component_type Component>
		  Component& getComponentUnsafe(Entity _ent) {}
		  template<component_type Component>
		  const Component& getComponentUnsafe(Entity _ent) const {}
		  // Execute an Action on all entities having the components
		  // expected by Action::operator(component_type&...).
		  // In addition, the entity itself is provided if
		  // the first parameter is of type Entity.
		  template<typename Action>
		  void execute(const Action& _action) {}

		  std::vector<Archetype> archetypes;
		  std::vector<bool> flags;
		  std::vector<uint32_t> generations;
		  std::unordered_map<Entity, Archetype> entityArchetypeMap;
	};

}