#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include "Entity.hpp"
#include "EntityRef.hpp"
#include "Archetype.hpp"
#include <unordered_map>
#include <any>

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
			  uint32_t position;
			  for (int i = 0; i < archetypes[_ent.archetype].entities.size(); i++) {
				  if (archetypes[_ent.archetype].entities[i].id == _ent.id) {
					  position = i;
					  break;
				  }
			  }
			  //////////////////////////////////////////////////////////
			  //CASE1 Does entities archetype contain component already?
			  int count_types = 0;
			  for (auto& it_types : archetypes[_ent.archetype].types) {
				  if (it_types == typeid(Component).hash_code()) {
					  Component& component_reference = reinterpret_cast<Component&>(archetypes[_ent.archetype].components[count_types].data.data()+sizeof(Component)*position);
					  return component_reference;
				  }
				  count_types++;
			  }

			  ////////////////////////////////////////////////////////////////
			  //CASE2 Need to move entity to new archetype that exists already

			  generations[_ent.id] += 1;

			  std::vector<size_t> entity_types;
			  for (auto& it_types : archetypes[_ent.archetype].types) {
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
					  it_archetypes.count_entities++;
					  bool move_back = true;
					  bool archetype_empty = false;
					  for (int i = 0; i < entity_types.size() - 1; i++) {
						  for (int j = 0; j < it_archetypes.types.size(); j++) {
							  if (entity_types[i] == it_archetypes.types[j]) {
								  size_t temp_typeSize = it_archetypes.components[j].typeSize;
								  //resize
								  it_archetypes.components[j].data.resize(temp_typeSize * it_archetypes.count_entities + 1);
								  //set src/dst
								  char* copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * position;
								  char* copy_data_dst = it_archetypes.components[j].data.data() * temp_typeSize * it_archetypes.count_entities;
								  //copy
								  memcpy(copy_data_dst, copy_data_src, temp_typeSize);

								  if (position == archetypes[_ent.archetype].count_entities) { 
									  archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * archetypes[_ent.archetype].count_entities);
									  move_back = false;
								  }
								  else {
									  copy_data_dst = copy_data_src;
									  copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize* archetypes[_ent.archetype].count_entities;
									  memcpy(copy_data_dst, copy_data_src, temp_typeSize);
									  archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * archetypes[_ent.archetype].count_entities);
									  
								  }
							  }
						  }
					  }

					  if (move_back) {
						  archetypes[_ent.archetype].entities[position] = archetypes[_ent.archetype].entities.back();
						  archetypes[_ent.archetype].entities.pop_back();
					  }

					  for (int j = 0; j < it_archetypes.types.size(); j++) {
						  if (typeid(Component).hash_code() == it_archetypes.types[j]) {
							  size_t temp_typeSize = it_archetypes.components[j].typeSize;
							  count_types = j;

							  it_archetypes.components[j].data.resize(temp_typeSize * it_archetypes.count_entities + 1);

							  Component* copy_data_src = Component(_args...);
							  char* copy_data_dst = it_archetypes.components[j].data.data() * temp_typeSize * it_archetypes.count_entities;
							  memcpy(copy_data_dst, copy_data_src, temp_typeSize);
							  break;
						  }
					  }

					  archetypes[_ent.archetype].count_entities--; 
					  it_archetypes.entities.push_back(_ent);
					  for (int l = 0; l < archetypes.size(); l++) {
						  if (archetypes[l] == it_archetypes) {
							  _ent.archetype = l;
							  break;
						  }
					  }
					  Component& component_reference = reinterpret_cast<Component&>(archetypes[_ent.archetype].components[count_types].data.data() + sizeof(Component) * (archetypes[_ent.archetype].entities.size()-1));
					  return component_reference;
				  }
			  }
			  ////////////////////////////////////////////////////////////////////
			  //CASE3 Need to move entity to new archetype that doesn't exists yet

			  std::vector <ComponentType> new_archetype_components;
			  std::vector <Entity*> new_archetype_entities;
			  uint32_t new_count = 0;
			  Archetype new_archetype = { entity_types; new_archetype_components; new_archetype_entities, new_count };
			  
			  bool moved_back = true;
			  bool archetype_empty = false;

			  for (int i = 0; i < entity_types.size() - 1; i++) {
				  size_t temp_typeSize = archetypes[_ent.archetype].components[i].typeSize;
				  std::vector<char> new_data;
				  new_data.resize(temp_typeSize);
				 
				  char* copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * position;
				  char* copy_data_dst = new_data.data();

				  memcpy(copy_data_dst, copy_data_src, temp_typeSize);
				  
				  ComponentType new_type = { new_data, temp_typeSize };				  
				  new_archetype.components.push_back(new_type);
					
				  ///////////////////////////////////////////////////////////////////////////
				  //hier weiter
				  //////////////////////////////////////////////////////////////////////////
				  if (entityArchetypeMap.at(_ent).components[i][position] == entityArchetypeMap.at(_ent).components[i].back()) {
					  if (entityArchetypeMap.at(_ent).components[i][position] == entityArchetypeMap.at(_ent).components[i].begin()) {
						  archetype_empty = true;
					  }
					  entityArchetypeMap.at(_ent).components[i].pop_back();
					  moved_back = false;
				  }
				  else {
					  entityArchetypeMap.at(_ent).components[i][position] = entityArchetypeMap.at(_ent).components[i].back();
				  }
			  }
			  
			  if (moved_back) {
				  for (auto& it_ent : entityArchetypeMap) {
					  if (entityArchetypeMap.at[it_ent] == entityArchetypeMap.at(_ent) && position == entityArchetypeMap.at(_ent).components[0].size() - 1) {
						  itposition = position;
					  }
				  }
			  }
			  std::vector<T> new_type;
			  new_archetype.components.push_back(new_type);
			  new_archetype.components[new_archetype.components.size() - 1].push_back(std::any(Component(_args ...)));

			  if (archetype_empty) {
				  entityArchetypeMap.at(_ent).is_empty = true;
			  }

			  position = 0;
			  entityArchetypeMap.at(_ent) = new_archetype;

			  archetypes.push_back(new_archetype);
			  //did you push entity on entity*vector?
			  Component& component_reference = std::any_cast<Component&>(entityArchetypeMap.at(_ent).components[entity_types.size() - 1][position]);
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
	};

}