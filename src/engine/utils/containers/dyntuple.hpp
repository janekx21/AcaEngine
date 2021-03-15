#pragma once

#include "hashmap.hpp"
#include <typeindex>
#include <typeinfo>

namespace utils {

	class DynamicTuple {
	public:
		template<typename T>
		T &get() {
			return *static_cast<T *>(m_elements[std::type_index(typeid(T))]);
		}

	private:
		HashMap<std::type_index, void *> m_elements;
	};
}// namespace utils