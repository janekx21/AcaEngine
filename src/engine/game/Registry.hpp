#pragma once
#include <optional>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <any>

template<typename T>
concept component_type = std::movable<T>;
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
struct ComponentArray {
	std::vector<T> elements;	// data of entities with this component
	int size;					//size of component
};

template<typename Component>
struct Archetype {
	int length;															// length of archetype
	std::unordered_map<uint32_t, uint32_t> entity_component_map;		// mapping entities to componentarray_index
	std::unordered_map< char*, ComponentArray<Component>> components;				// mapping name of component to component arrray
};

template<typename T>
class Registry 
{ 
public:
	Entity create(); 
	void erase(Entity _ent); 
	EntityRef getRef(Entity _ent) const; 
	std::optional<Entity> getEntity(EntityRef _ent) const; 
	

	// Add a new component to an existing entity. No changes are done if _ent already has a component of this type.
	// @return A reference to the new component or the already existing component.

	template<component_type Component, typename... Args>
	Component& addComponent(Entity _ent, Args&&... _args) {
																// 1.  CASE entity not in ea_map		
		if (!entity_archetype_map.contains(_ent.id)) {																				
			for (auto& it : archetypes) {																									// if not: checke if archetype exists already
				if (it.length == 1) {
					if (it.components.contains(typeid(Component).name())) {
						it.components[typeid(Component).name()].elements.push_back(Component(_args...));
						it.components[typeid(Component).name()].size++;
						it.entity_component_map.insert({ {_ent.id, it.components[typeid(Component).name()].size - 1} });
						entity_archetype_map.insert({ {_ent.id, it} });
						int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
						Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
						return component_reference;
					}
				}
			}
			int a_length = 1;																												// if it doesn't exist create new
			std::unordered_map<uint32_t, uint32_t> a_ecmap;
			a_ecmap.insert({ {_ent.id, 0} });

			std::vector<T> a_elements;
			a_elements.push_back(Components(_args...));

			int a_size = 1;

			ComponentArray a_Array = { a_elements, a_size };

			std::unordered_map< char*, ComponentArray> a_components;
			a_components.insert({ {typeid(Component).name(), a_Array} });

			Archetype a_archetype = { a_length, a_ecmap, a_components };
			archetypes.push_back(a_archetype);

			entity_archetype_map.insert({ {_ent.id, archetypes.size()-1} });

			int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
			Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
			return component_reference;
		}
													//2. CASE component already exists in existing archetype // query wether component already exists in archetype
		if (archetypes[entity_archetype_map.at(_ent.id)].components.contains(typeid(Component).name())) {							
			int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
			Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
			return component_reference;
		}					

		else {																														
													//3. CASE component exists in another existing archetype --> move entity there 
			bool exists = true;
			vector<char*> temp_components;
			for (auto& it : archetypes[entity_archetype_map.at(_ent.id)].components) {
				temp_components.push_back(it);
			}
			temp_components.push_back(typeid(Component).name());

			for (auto& it : archetypes) {
				exists = true;
				for (int i = 0; i < temp_components.size(); i++) {
					if (!it.components.contains(temp_components[i])) { exists = false; }
				}
				if(exists && temp_components.size() == it.length){
					// TODO: move entity to existing archetype and update maps remove old entities and archtypes
					//where is data
					bool change_entity_component_map_id = false;
					int data_id = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];					// id in old ecmap
					for (auto& it_comp_old : archetypes[entity_archetype_map.at(_ent.id)].components) {
						it.components[it_comp_old].elements.push_back(archetypes[entity_archetype_map.at(_ent.id)].components[it_comp_old].elements[data_id]);		// move data to new archetype
						it.components[it_comp_old].size++;
						if (entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] == entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back()) {  //remove data from old archetype
							entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
						}
						else {
							change_entity_component_map_id = true;
							entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] = entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back();
							entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
						}
						entity_archetype_map.at(_ent.id).components[it_comp_old].size--;
						
					}
					if (change_entity_component_map_id) {
						for (auto& it_data_old : archetypes[entity_archetype_map.at(_ent.id)].entity_component_map) {        // change old component map id 
							if (archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] == entity_archetype_map.at(_ent.id).components[it_comp_old].size - 1) {
								archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] = data_id;
							}
						}
					}
					archetypes[entity_archetype_map.at(_ent.id)].entity_component_map.erase(_ent.id);						// delete moved old component map id

					it.entity_component_map.insert({ {_ent.id, it.components[typeid(Component).name()].size - 1} });		// add new ecmap entry in new archetype
					
					it.components[typeid(Component).name()].elements.push_back(Component(_args...));						// add new component data
					it.components[typeid(Component).name()].size++;
					
					entity_archetype_map[_ent.id] = std::distance(archetypes.begin(), it);

					int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
					Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
					return component_reference;
				}

			}
																					
														// 4. CASE New Archetype --> create and move old data there

			int a_length = temp_components.size();
			std::unordered_map<uint32_t, uint32_t> a_ecmap;
			a_ecmap.insert({ {_ent.id, 0} });

			std::vector<T> a_elements;
			a_elements.push_back(Components(_args...));

			int a_size = 1;

			ComponentArray a_Array = { a_elements, a_size };

			std::unordered_map< char*, ComponentArray> a_components;
			a_components.insert({ {typeid(Component).name(), a_Array} });

			Archetype a_archetype = { a_length, a_ecmap, a_components };
			
			int data_id = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
			bool change_entity_component_map_id = false;
			for (auto& it_comp_old : archetypes[entity_archetype_map.at(_ent.id)].components) {									// move old data to new archetype
				std::vector<T> b_elements;
				b_elements.push_back(archetypes[entity_archetype_map.at(_ent.id)].components[it_comp_old].elements[data_id]);
				ComponentArray b_Array = { b_elements, 1 };
				a_archetype.components.insert({ {it_comp_old, b_Array} });
				if (entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] == entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back()) {  
					entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
				}
				else {
					change_entity_component_map_id = true;
					entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] = entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back();
					entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
				}
				entity_archetype_map.at(_ent.id).components[it_comp_old].size--;
			}
			if (change_entity_component_map_id) {
				for (auto& it_data_old : archetypes[entity_archetype_map.at(_ent.id)].entity_component_map) {						// change old component map id 
					if (archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] == entity_archetype_map.at(_ent.id).components[it_comp_old].size - 1) {
						archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] = data_id;
					}
				}
			}

			archetypes[entity_archetype_map.at(_ent.id)].entity_component_map.erase(_ent.id);										// delete moved old component map id

			archetypes.push_back(a_archetype);																					// push archetype and update map
			entity_archetype_map[_ent.id] = archetypes.size()-1; 

			int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
			Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
			return component_reference;
		}
	
	
	}
	// Remove a component from an existing entity.
	// Does not check whether it exists.
	template<component_type Component>
	void removeComponent(Entity _ent) {
												
		if (archetypes[entity_archetype_map.at(_ent.id)].components.contains(typeid(Components).name())) {
												//1.CASE entity is without component
			if (archetypes[entity_archetype_map.at(_ent.id)].length == 1) {
				int data_id = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
				bool change_entity_component_map_id = false;
				for (auto& it_comp_old : archetypes[entity_archetype_map.at(_ent.id)].components) {									// remove old data 
					if (entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] == entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back()) {
						entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
					}
					else {
						change_entity_component_map_id = true;
						entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] = entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back();
						entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
					}
					entity_archetype_map.at(_ent.id).components[it_comp_old].size--;
				}
				if (change_entity_component_map_id) {
					for (auto& it_data_old : archetypes[entity_archetype_map.at(_ent.id)].entity_component_map) {						// change old component map id 
						if (archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] == entity_archetype_map.at(_ent.id).components[it_comp_old].size - 1) {
							archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] = data_id;
						}
					}
				}

				archetypes[entity_archetype_map.at(_ent.id)].entity_component_map.erase(_ent.id);										// delete moved old component map id
				entity_archetype_map.erase(_ent.id);
				return;
			}
												//2.CASE new archetype exists already
			vector<char*> temp_components;
			for (auto& it : archetypes[entity_archetype_map.at(_ent.id)].components) {
				if (it == typeid(Component).name()) {
					continue;
				}
				temp_components.push_back(it);
			}
			for (auto& it : archetypes) {
				if (it.length == temp_components.size()) {
					bool the_one = true;
					for (int i = 0; int i < temp_components.size(); i++) {
						if (!it.components.contains(temp_components[i]) {
							the_one = false;
						}
					}
					if (the_one) {
						//TODO move and delete
						bool change_entity_component_map_id = false;
						int data_id = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];					// id in old ecmap
						for (auto& it_comp_old : archetypes[entity_archetype_map.at(_ent.id)].components) {
							if (it_comp_old != typeid(Component).name())) {
								it.components[it_comp_old].elements.push_back(archetypes[entity_archetype_map.at(_ent.id)].components[it_comp_old].elements[data_id]);		// move data to new archetype
								it.components[it_comp_old].size++;
							}
							if (entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] == entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back()) {  //remove data from old archetype
								entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
							}
							else {
								change_entity_component_map_id = true;
								entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] = entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back();
								entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
							}
							entity_archetype_map.at(_ent.id).components[it_comp_old].size--;

						}
						if (change_entity_component_map_id) {
							for (auto& it_data_old : archetypes[entity_archetype_map.at(_ent.id)].entity_component_map) {        // change old component map id 
								if (archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] == entity_archetype_map.at(_ent.id).components[it_comp_old].size - 1) {
									archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] = data_id;
								}
							}
						}
						archetypes[entity_archetype_map.at(_ent.id)].entity_component_map.erase(_ent.id);						// delete moved old component map id

						it.entity_component_map.insert({ {_ent.id, it.components[typeid(Component).name()].size - 1} });		// add new ecmap entry in new archetype
						entity_archetype_map[_ent.id] = std::distance(archetypes.begin(),it);
						return;
					}
				}
			}

												// 3.CASE new archetype must be created
			int a_length = temp_components.size();
			std::unordered_map<uint32_t, uint32_t> a_ecmap;
			a_ecmap.insert({ {_ent.id, 0} });

			ComponentArray a_Array;

			std::unordered_map< char*, ComponentArray> a_components;

			Archetype a_archetype = { a_length, a_ecmap, a_components };

			int data_id = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
			bool change_entity_component_map_id = false;
			for (auto& it_comp_old : archetypes[entity_archetype_map.at(_ent.id)].components) {									// move old data to new archetype
				if (it_comp_old != typeid(Component).name()) {
					std::vector<T> b_elements;
					b_elements.push_back(archetypes[entity_archetype_map.at(_ent.id)].components[it_comp_old].elements[data_id]);
					ComponentArray b_Array = { b_elements, 1 };
					a_archetype.components.insert({ {it_comp_old, b_Array} });
				}
				if (entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] == entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back()) {
					entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
				}
				else {
					change_entity_component_map_id = true;
					entity_archetype_map.at(_ent.id).components[it_comp_old].elements[data_id] = entity_archetype_map.at(_ent.id).components[it_comp_old].elements.back();
					entity_archetype_map.at(_ent.id).components[it_comp_old].elements.pop_back();
				}
				entity_archetype_map.at(_ent.id).components[it_comp_old].size--;
			}
			if (change_entity_component_map_id) {
				for (auto& it_data_old : archetypes[entity_archetype_map.at(_ent.id)].entity_component_map) {						// change old component map id 
					if (archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] == entity_archetype_map.at(_ent.id).components[it_comp_old].size - 1) {
						archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[it_data_old] = data_id;
					}
				}
			}

			archetypes[entity_archetype_map.at(_ent.id)].entity_component_map.erase(_ent.id);										// delete moved old component map id

			archetypes.push_back(a_archetype);																					// push archetype and update map
			entity_archetype_map[_ent.id] = archetypes.size() - 1;

			return;
																		
		}
		else {									//4.CASE entity remains the same (component didn't exist in entity)
			return;
		}
	}
	// Retrieve a component associated with an entity.
	// @return The component or nullptr if the entity has no such component.
	template<component_type Component>
	Component* getComponent(Entity _ent) {
		if (archetypes[entity_archetype_map.at(_ent.id)].components.contains(typeid(Components).name())) {
			int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
			Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
			return component_reference;
		}
		else {
			return nullptr;
		}
	}
	template<component_type Component>
	const Component* getComponent(Entity _ent) const {
		if (archetypes[entity_archetype_map.at(_ent.id)].components.contains(typeid(Components).name())) {
			int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
			Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
			return component_reference;
		}
		else {
			return nullptr;
		}
	}
	// Retrieve a component associated with an entity.
	// Does not check whether it exits.
	template<component_type Component>
	Component& getComponentUnsafe(Entity _ent) {
		int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
		Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
		return component_reference;
	}
	template<component_type Component>
	const Component& getComponentUnsafe(Entity _ent) const {
		int i = archetypes[entity_archetype_map.at(_ent.id)].entity_component_map[_ent.id];
		Component& component_reference = archetypes[entity_archetype_map.at(_ent.id)].components[typeid(Component).name().elements[i]];
		return component_reference;
	}
	// Execute an Action on all entities having the components
	// expected by Action::operator(component_type&...).
	// In addition, the entity itself is provided if
	// the first parameter is of type Entity.
	template<typename Action>
	void execute(const Action& _action) {}
	std::vector<Archetype<std::any>> archetypes;
	std::vector<bool> flags;				//ToDo change
	std::vector<uint32_t>generations;
	std::unordered_map<uint32_t, uint32_t> entity_archetype_map;  //mapping entity_id to pos in archeytype vector
	
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

