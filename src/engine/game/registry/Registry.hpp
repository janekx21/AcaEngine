#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include "Entity.hpp"
#include "EntityRef.hpp"
#include "Archetype.hpp"
#include <unordered_map>
#include <any>
#include <tuple>

namespace game {
	template<typename T>
	concept component_type = std::movable<T>;
	class Registry {

	public:

		Entity create() {
			uint32_t id;
			if (archetypes.empty()) {
				std::vector < size_t > zero_types;
				std::vector < ComponentType > zero_components;
				std::vector <uint32_t> zero_entities;
				Archetype zero_archetype = { zero_types, zero_components, zero_entities };
				archetypes.push_back(zero_archetype);
			}
			for (id = 0; id < flags.size(); id++) {
				if (flags[id] == 0) {
					flags[id] = 1;
					generations[id] += 1;
					Entity new_entity = { id, 0 };
					archetypes[0].entities.push_back(new_entity.id);
					return new_entity;
				}
			}
			flags.push_back(1);
			generations.push_back(1);
			Entity new_entity = { id, 0 };
			archetypes[0].entities.push_back(new_entity.id);
			return new_entity;

		};


		void erase(Entity _ent) {
			flags[_ent.id] = 0;
			//TODO:erase components
			uint32_t position;
			for (int i = 0; i < archetypes[_ent.archetype].entities.size(); i++) {
				if (archetypes[_ent.archetype].entities[i] == _ent.id) {
					position = i;
					break;
				}
			}
			bool move_back = true;
			for (auto& component_iterator : archetypes[_ent.archetype].components) {
				size_t temp_typeSize = component_iterator.typeSize;
				if (position == archetypes[_ent.archetype].entities.size() - 1) {
					component_iterator.data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));
					move_back = false;
				}
				else {
					char* copy_data_src = component_iterator.data.data() + temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1);
					char* copy_data_dst = component_iterator.data.data() + temp_typeSize * position;
					memcpy(copy_data_dst, copy_data_src, temp_typeSize);
					component_iterator.data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));
				}
			}

			if (move_back) {
				archetypes[_ent.archetype].entities[position] = archetypes[_ent.archetype].entities.back();
			}
			archetypes[_ent.archetype].entities.pop_back();

		}

		EntityRef getRef(Entity _ent) const {
			return { _ent, generations[_ent.id] };
		}

		std::optional<Entity> getEntity(EntityRef _ent) const {
			if (_ent.generation == generations[_ent.ent.id] && flags[_ent.ent.id]) {
				return _ent.ent;
			}
			else {
				return std::nullopt;
			}
		}



		// Add a new component to an existing entity. No changes are done if Component
		// if_ent already has a component of this type.
		// @return A reference to the new component or the already existing component.
		template<component_type Component, typename... Args>
		Component& addComponent(Entity& _ent, Args&&... _args) {
			uint32_t position;
			for (int i = 0; i < archetypes[_ent.archetype].entities.size(); i++) {
				if (archetypes[_ent.archetype].entities[i] == _ent.id) {
					position = i;
					break;
				}
			}
			//////////////////////////////////////////////////////////
			//CASE1 Does entities archetype contain component already?
			int count_types = 0;
			for (auto& it_types : archetypes[_ent.archetype].types) {
				if (it_types == typeid(Component).hash_code()) {

					Component& component_reference = *reinterpret_cast<Component*>(archetypes[_ent.archetype].components[count_types].data.data() + sizeof(Component) * position);
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
			//std::cout << "entity_types.size() = " << entity_types.size() << "\n";
			//TODO: check wether archetype exists
			for (int k = 0; k < archetypes.size(); k++) {
				auto& it_archetypes = archetypes[k];
				//std::cout << "it_archetypes.types.size() = " << it_archetypes.types.size() << "\n";
				if (entity_types.size() != it_archetypes.types.size()) {
					continue;
				}
				bool archetype_exists = true;
				for (auto& it_types_entity : entity_types) {	//hash sortieren und summieren (collision hashmap)
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
					bool move_back = true;
					bool archetype_empty = false;
					for (int i = 0; i < entity_types.size() - 1; i++) {
						for (int j = 0; j < it_archetypes.types.size(); j++) {
							if (entity_types[i] == it_archetypes.types[j]) {
								size_t temp_typeSize = it_archetypes.components[j].typeSize;
								//resize

								it_archetypes.components[j].data.resize(temp_typeSize * (it_archetypes.entities.size() + 1));
								//set src/dst
								char* copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * position;
								char* copy_data_dst = it_archetypes.components[j].data.data() + temp_typeSize * it_archetypes.entities.size();
								//copy
								memcpy(copy_data_dst, copy_data_src, temp_typeSize);

								if (position == archetypes[_ent.archetype].entities.size()) {
									archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * archetypes[_ent.archetype].entities.size());
									move_back = false;
								}
								else {
									copy_data_dst = copy_data_src;
									copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * archetypes[_ent.archetype].entities.size();
									memcpy(copy_data_dst, copy_data_src, temp_typeSize);
									archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * archetypes[_ent.archetype].entities.size());

								}
							}
						}
					}

					if (move_back) {
						archetypes[_ent.archetype].entities[position] = archetypes[_ent.archetype].entities.back();
					}

					archetypes[_ent.archetype].entities.pop_back();

					for (int j = 0; j < it_archetypes.types.size(); j++) {
						if (typeid(Component).hash_code() == it_archetypes.types[j]) {
							size_t temp_typeSize = it_archetypes.components[j].typeSize;
							count_types = j;
							it_archetypes.components[j].data.resize(temp_typeSize * (it_archetypes.entities.size() + 1));

							Component copy_data_src = Component(_args...);
							char* copy_data_dst = it_archetypes.components[j].data.data() + temp_typeSize * it_archetypes.entities.size();
							memcpy(copy_data_dst, &copy_data_src, temp_typeSize);
							break;
						}
					}

					it_archetypes.entities.push_back(_ent.id);

					_ent.archetype = k;
					Component& component_reference = *reinterpret_cast<Component*>(archetypes[_ent.archetype].components[count_types].data.data() + sizeof(Component) * (archetypes[_ent.archetype].entities.size() - 1));
					return component_reference;
				}
			}
			////////////////////////////////////////////////////////////////////
			//CASE3 Need to move entity to new archetype that doesn't exists yet

			std::vector <ComponentType> new_archetype_components;
			std::vector <uint32_t> new_archetype_entities;
			Archetype new_archetype = { entity_types, new_archetype_components, new_archetype_entities }; // copy?

			bool move_back = true;
			bool archetype_empty = false;

			for (int i = 0; i < entity_types.size() - 1/*gef�hrlich*/; i++) {
				size_t temp_typeSize = archetypes[_ent.archetype].components[i].typeSize;
				std::vector<char> new_data;
				new_data.resize(temp_typeSize);

				char* copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * position;
				char* copy_data_dst = new_data.data();

				memcpy(copy_data_dst, copy_data_src, temp_typeSize);

				ComponentType new_type = { new_data, temp_typeSize };
				new_archetype.components.push_back(new_type);

				if (position == archetypes[_ent.archetype].entities.size() - 1) {
					archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));
					move_back = false;
				}
				else {
					copy_data_dst = copy_data_src;
					copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1);
					memcpy(copy_data_dst, copy_data_src, temp_typeSize);
					archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));
				}
			}

			if (move_back) {
				archetypes[_ent.archetype].entities[position] = archetypes[_ent.archetype].entities.back();

			}
			archetypes[_ent.archetype].entities.pop_back();

			size_t temp_typeSize = sizeof(Component);
			std::vector<char> new_data;
			new_data.resize(temp_typeSize);

			Component copy_data_src = Component(_args...);
			char* copy_data_dst = new_data.data();
			memcpy(copy_data_dst, &copy_data_src, temp_typeSize);

			ComponentType new_type = { new_data, temp_typeSize };
			new_archetype.components.push_back(new_type);


			new_archetype.entities.push_back(_ent.id);

			archetypes.push_back(new_archetype); // copy again, push first the use refernce (emplace_back)

			_ent.archetype = archetypes.size() - 1;

			Component& component_reference = *reinterpret_cast<Component*>(archetypes[_ent.archetype].components[count_types].data.data());
			return component_reference;
		}
		// Remove a component from an existing entity.// Does not check whether it exists.
		template<component_type Component>
		void removeComponent(Entity& _ent) {
			uint32_t position;
			for (int i = 0; i < archetypes[_ent.archetype].entities.size(); i++) {
				if (archetypes[_ent.archetype].entities[i] == _ent.id) {
					position = i;
					break;
				}
			}
			int pos_of_del_comp = 0;
			std::vector<size_t> entity_types;
			for (int a = 0; a < archetypes[_ent.archetype].types.size(); a++) {
				auto& it_types = archetypes[_ent.archetype].types[a];
				if (it_types != typeid(Component).hash_code()) {
					entity_types.push_back(it_types);
				}
				else {
					pos_of_del_comp = a;
				}
			}
			//1.CASE new archetype exists already
			for (int k = 0; k < archetypes.size(); k++) {
				auto& it_archetypes = archetypes[k];
				if (entity_types.size() != it_archetypes.types.size()) {
					continue;
				}
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

					bool move_back = true;
					bool archetype_empty = false;
					for (int i = 0; i < entity_types.size(); i++) {
						for (int j = 0; j < it_archetypes.types.size(); j++) {
							if (entity_types[i] == it_archetypes.types[j]) {
								size_t temp_typeSize = it_archetypes.components[j].typeSize;

								it_archetypes.components[j].data.resize(temp_typeSize * (it_archetypes.entities.size() + 1));

								char* copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * position;
								char* copy_data_dst = it_archetypes.components[j].data.data() + temp_typeSize * it_archetypes.entities.size();

								memcpy(copy_data_dst, copy_data_src, temp_typeSize);

								if (position == (archetypes[_ent.archetype].entities.size() - 1)) {
									archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * archetypes[_ent.archetype].entities.size());
									move_back = false;
								}
								else {
									copy_data_dst = copy_data_src;
									copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1);
									memcpy(copy_data_dst, copy_data_src, temp_typeSize);
									archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));

								}
							}
						}
					}

					if (move_back) {
						archetypes[_ent.archetype].entities[position] = archetypes[_ent.archetype].entities.back();
					}

					archetypes[_ent.archetype].entities.pop_back(); //attention entitiesvector is now one entry smaller

					for (int j = 0; j < archetypes[_ent.archetype].types.size(); j++) {
						if (typeid(Component).hash_code() == archetypes[_ent.archetype].types[j]) {
							size_t temp_typeSize = sizeof(Component);
							if (position == archetypes[_ent.archetype].entities.size()) {
								archetypes[_ent.archetype].components[j].data.resize(temp_typeSize * archetypes[_ent.archetype].entities.size());
							}
							else {
								char* copy_data_src = archetypes[_ent.archetype].components[j].data.data() + temp_typeSize * archetypes[_ent.archetype].entities.size();
								char* copy_data_dst = archetypes[_ent.archetype].components[j].data.data() + temp_typeSize * position;
								memcpy(copy_data_dst, copy_data_src, temp_typeSize);
								archetypes[_ent.archetype].components[j].data.resize(temp_typeSize * archetypes[_ent.archetype].entities.size());
							}
							break;
						}
					}

					it_archetypes.entities.push_back(_ent.id);

					_ent.archetype = k;

					return;
				}
			}
			//2.CASE new archetype must be created //func_create archetypes()
			std::vector <ComponentType> new_archetype_components;
			std::vector <uint32_t> new_archetype_entities;
			Archetype new_archetype = { entity_types, new_archetype_components, new_archetype_entities };

			bool move_back = true;
			bool archetype_empty = false;
			for (int i = 0; i < entity_types.size() + 1; i++) {
				if (i == pos_of_del_comp) {
					continue;
				}
				size_t temp_typeSize = archetypes[_ent.archetype].components[i].typeSize;
				std::vector<char> new_data;
				new_data.resize(temp_typeSize);

				char* copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * position;
				char* copy_data_dst = new_data.data();

				memcpy(copy_data_dst, copy_data_src, temp_typeSize);

				ComponentType new_type = { new_data, temp_typeSize };
				new_archetype.components.push_back(new_type);
				if (position == archetypes[_ent.archetype].entities.size() - 1) {
					archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));
					move_back = false;
				}
				else {
					copy_data_dst = copy_data_src;
					copy_data_src = archetypes[_ent.archetype].components[i].data.data() + temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1);
					memcpy(copy_data_dst, copy_data_src, temp_typeSize);
					archetypes[_ent.archetype].components[i].data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));
				}
			}


			for (int j = 0; j < archetypes[_ent.archetype].types.size(); j++) {
				if (typeid(Component).hash_code() == archetypes[_ent.archetype].types[j]) {
					size_t temp_typeSize = sizeof(Component);
					if (position == archetypes[_ent.archetype].entities.size() - 1) {
						archetypes[_ent.archetype].components[j].data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));
					}
					else {
						char* copy_data_src = archetypes[_ent.archetype].components[j].data.data() + temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1);
						char* copy_data_dst = archetypes[_ent.archetype].components[j].data.data() + temp_typeSize * position;
						memcpy(copy_data_dst, copy_data_src, temp_typeSize);
						archetypes[_ent.archetype].components[j].data.resize(temp_typeSize * (archetypes[_ent.archetype].entities.size() - 1));
					}
					break;
				}
			}

			if (move_back) {
				archetypes[_ent.archetype].entities[position] = archetypes[_ent.archetype].entities.back();
			}
			archetypes[_ent.archetype].entities.pop_back();

			new_archetype.entities.push_back(_ent.id);

			archetypes.push_back(new_archetype);
			_ent.archetype = archetypes.size() - 1;
			return;

		}
		// Retrieve a component associated with an entity.
		// @return The component or nullptr if the entity has no such component.
		template<component_type Component>
		Component* getComponent(Entity _ent) {
			uint32_t position;
			for (int i = 0; i < archetypes[_ent.archetype].entities.size(); i++) {
				if (archetypes[_ent.archetype].entities[i] == _ent.id) {
					position = i;
					break;
				}
			}
			for (int i = 0; i < archetypes[_ent.archetype].types.size(); i++) {
				if (typeid(Component).hash_code() == archetypes[_ent.archetype].types[i]) {
					Component* component_reference = reinterpret_cast<Component*>(archetypes[_ent.archetype].components[i].data.data() + sizeof(Component) * position);
					return component_reference;
				}
			}
			return nullptr;
		}


		template<component_type Component>
		const Component* getComponent(Entity _ent) const {
			uint32_t position;
			for (int i = 0; i < archetypes[_ent.archetype].entities.size(); i++) {
				if (archetypes[_ent.archetype].entities[i] == _ent.id) {
					position = i;
					break;
				}
			}
			for (int i = 0; i < archetypes[_ent.archetype].types.size(); i++) {
				if (typeid(Component).hash_code() == archetypes[_ent.archetype].types[i]) {
					Component& component_reference = *reinterpret_cast<Component*>(archetypes[_ent.archetype].components[i].data.data() + sizeof(Component) * position);
					return component_reference;
				}
			}
			return nullptr;
		}
		// Retrieve a component associated with an entity.
		// Does not check whether it exits.
		template<component_type Component>
		Component& getComponentUnsafe(Entity _ent) {
			uint32_t position;
			for (int i = 0; i < archetypes[_ent.archetype].entities.size(); i++) {
				if (archetypes[_ent.archetype].entities[i] == _ent.id) {
					position = i;
					break;
				}
			}
			for (int i = 0; i < archetypes[_ent.archetype].types.size(); i++) {
				if (typeid(Component).hash_code() == archetypes[_ent.archetype].types[i]) {
					Component& component_reference = *reinterpret_cast<Component*>(archetypes[_ent.archetype].components[i].data.data() + sizeof(Component) * position);
					return component_reference;
				}
			}
		}
		template<component_type Component>
		const Component& getComponentUnsafe(Entity _ent) const {
			uint32_t position;
			for (int i = 0; i < archetypes[_ent.archetype].entities.size(); i++) {
				if (archetypes[_ent.archetype].entities[i] == _ent.id) {
					position = i;
					break;
				}
			}
			for (int i = 0; i < archetypes[_ent.archetype].types.size(); i++) {
				if (typeid(Component).hash_code() == archetypes[_ent.archetype].types[i]) {
					Component& component_reference = *reinterpret_cast<Component*>(archetypes[_ent.archetype].components[i].data.data() + sizeof(Component) * position);
					return component_reference;
				}
			}

		}



		// Execute an Action on all entities having the components
		// expected by Action::operator(component_type&...).
		// In addition, the entity itself is provided if
		// the first parameter is of type Entity.
		template<typename... Args, typename Action>
		void execute(const Action& _action) {
			//using allComponents = std::tuple<Args...>;

			bool hasAllComponents;
			bool first_is_entity = false;
			std::vector<std::size_t> action_types;
			std::vector<std::size_t> action_types_size;
			(action_types.push_back(typeid(Args).hash_code()), ...);
			(action_types_size.push_back(sizeof(Args)), ...);
			

			if (*action_types.begin() == typeid(Entity).hash_code()) {
				first_is_entity = true;
				action_types.erase(action_types.begin());
				action_types_size.erase(action_types_size.begin());
			}
			std::vector<uint32_t> archetype_ids;
			for (int u = 0; u < archetypes.size(); u++) {
				auto& archetype_iterator = archetypes[u];
				hasAllComponents = true;

				for (auto& type_iterator : action_types) {
					bool contains_type = false;
					for (auto& archetype_type_iterator : archetype_iterator.types) {
						if (archetype_type_iterator == type_iterator) {
							contains_type = true;
						}
					}
					if (!contains_type) {
						hasAllComponents = false;
					}
				}

				if (hasAllComponents) {

					std::vector<uint32_t> component_flags;
					for (int i = 0; i < archetype_iterator.types.size(); i++) {
						bool component_type_exists = false;
						for (int j = 0; j < action_types.size(); j++) {
							if (archetype_iterator.types[i] == action_types[j]) {
								component_type_exists = true;
							}
						}
						if (component_type_exists) {
							component_flags.push_back(1);
						}
						else {
							component_flags.push_back(0);
						}

					}
					//execute
					for (int i = 0; i < archetype_iterator.entities.size(); i++) {

						int s = 0;
						while (component_flags[s] == 0) {
							s++;
						}
						Entity entity_help = { archetype_iterator.entities[i], u };
						auto tuple = std::tie();
						if constexpr (std::is_same_v<std::tuple_element_t<0, std::tuple<Args...>>, Entity>) {
							executeHelper_Entity<Args...>(_action, tuple, archetype_iterator.components, component_flags, i, s, entity_help);
						}
						else {
							executeHelper<Args...>(_action, tuple, archetype_iterator.components, component_flags, i, s);
						}

					}

				}
			}



			//check all archetypes, for all components needed in the execute call
			//execute call on those

		}


		template<component_type Component, typename ...Args, typename Action, typename Tuple>
		void executeHelper(Action& _action, Tuple& _tuple, std::vector<ComponentType>& _component, std::vector<uint32_t>& _component_flags, int i, int& s) {
			/*for (int i = 0; i < _component_flags.size(); i++) {
				std::cout << _component_flags[i];
			}
			std::cout << "\n";*/
			auto tuple = std::tuple_cat(_tuple, std::tie(*reinterpret_cast<Component*>(_component[s].data.data() + _component[s].typeSize * i)));
			if constexpr (sizeof ...(Args) > 0) {
				while (_component_flags[s] == 0) {
					s++;
				}
				executeHelper<Args...>(_action, tuple, _component, _component_flags, i, s);
			}
			else {
				std::apply(_action, tuple);
			}
		}

		template<component_type Component, typename ...Args, typename Action, typename Tuple>
		void executeHelper_Entity(Action& _action, Tuple& _tuple, std::vector<ComponentType>& _component, std::vector<uint32_t> _component_flags, int i, int& s, Entity _entity) {
			auto tuple = std::make_tuple(_entity);
			executeHelper<Args...>(_action, tuple, _component, _component_flags, i, s);
		}




		void print_entities() {
			std::cout << "archetypes\t_ent.id\tentarr.size\n";
			for (int f = 0; f < archetypes.size(); f++) {
				for (int g = 0; g < archetypes[f].entities.size(); g++) {
					std::cout << f << "\t\t" << archetypes[f].entities[g] << "\t" << archetypes[f].entities.size() << "\n\n";
				}
			}
		}

		std::vector<Archetype> archetypes;
		std::vector<bool> flags;
		std::vector<uint32_t> generations;
	};

}