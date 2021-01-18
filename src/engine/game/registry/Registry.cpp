#include "Registry.hpp"

namespace game {
	template class Registry<int>;

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

	template<typename T>
	void Registry<T>::setData(Entity _ent, const T& _value)
	{
		data[_ent.id] = _value;
	}

	template<typename T>
	const T& Registry<T>::getData(Entity _ent) const
	{
		// TODO: insert return statement here
		return data[_ent.id];
	}

	template<typename T>
	T& Registry<T>::getData(Entity _ent)
	{
		// TODO: insert return statement here
		return data[_ent.id];
	}


	template<typename T>
	template<typename FN>
	inline void Registry<T>::execute(FN _fn)
	{
		uint32_t id;
		for (id = 0; id < data.size(); id++) {
			if (flags[id]) {
				_fn(data[id]);
			}
		}
	}
}
