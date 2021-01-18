#include "Registry.hpp"

namespace game {
	template class Registry<int>;

	template<typename T>
	Entity Registry<T>::create() {
		uint32_t id;
		for (id = 0; id < idUsed.size(); id++) {
			if (!idUsed[id]) {
				idUsed[id] = true;
				generations[id] += 1;
				data[id] = (T) 0;
				return Entity{id};
			}
		}
		idUsed.push_back(true);
		generations.push_back(1);
		data.push_back((T) 0);
		return Entity{id};
	}

	template<typename T>
	void Registry<T>::erase(Entity _entity) {
		idUsed[_entity.id] = false;
		data[_entity.id] = (T) 0;
	}

	template<typename T>
	EntityRef Registry<T>::getRef(Entity _entity) const {
		return {_entity, generations[_entity.id]};
	}

	template<typename T>
	std::optional<Entity> Registry<T>::getEntity(EntityRef _entityRef) const {
		if (_entityRef.generation == generations[_entityRef.entity.id] && idUsed[_entityRef.entity.id]) {
			return _entityRef.entity;
		}
	}

	template<typename T>
	void Registry<T>::setData(Entity _entity, const T &_value) {
		data[_entity.id] = _value;
	}

	template<typename T>
	const T &Registry<T>::getData(Entity _entity) const {
		return data[_entity.id];
	}

	template<typename T>
	T &Registry<T>::getData(Entity _entity) {
		return data[_entity.id];
	}

	template<typename T>
	template<typename FN>
	inline void Registry<T>::execute(FN _function) {
		uint32_t id;
		for (id = 0; id < data.size(); id++) {
			if (idUsed[id]) {
				_function(data[id]);
			}
		}
	}
}
