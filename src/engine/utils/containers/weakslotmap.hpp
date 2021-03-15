#pragma once

#include "../../utils/assert.hpp"
#include <concepts>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

namespace utils {
	// std::integral Key, std::movable Value
	template<typename Key, bool TrivialDestruct = false>
	class WeakSlotMap {
	protected:
		using SizeType = Key;
		constexpr static Key INVALID_SLOT = std::numeric_limits<Key>::max();

	public:
		template<typename Value>
		WeakSlotMap(Value *dummy, SizeType _initialSize = 4)
				: m_elementSize(sizeof(Value)),
					m_destructor(destroyElement<Value>),
					m_move(moveElement<Value>),
					m_size(0),
					m_capacity(_initialSize),
					m_values(new char[m_capacity * m_elementSize]) {
			static_assert(std::is_trivially_destructible_v<Value> || !TrivialDestruct,
										"Managed elements require a destructor call.");
		}

		WeakSlotMap(WeakSlotMap &&_oth) noexcept
				: m_elementSize(_oth.m_elementSize),
					m_destructor(_oth.m_destructor),
					m_move(_oth.m_move),
					m_size(_oth.m_size),
					m_capacity(_oth.m_capacity),
					m_values(std::move(_oth.m_values)),
					m_slots(std::move(_oth.m_slots)),
					m_valuesToSlots(std::move(_oth.m_valuesToSlots)) {
			_oth.m_size = 0;
			_oth.m_capacity = 0;
		}

		~WeakSlotMap() {
			destroyValues();
		}

		template<typename Value, typename... Args>
		Value &emplace(Key _key, Args &&..._args) {
			// increase slots if necessary
			if (m_slots.size() <= _key)
				m_slots.resize(_key + 1, INVALID_SLOT);
			else if (m_slots[_key] != INVALID_SLOT)// already exists
				return at<Value>(_key);

			m_slots[_key] = m_size;

			m_valuesToSlots.emplace_back(_key);
			if (m_capacity == m_size) {
				m_capacity = static_cast<SizeType>(1.5 * m_capacity);
				char *newBuf = new char[m_capacity * m_elementSize];
				for (SizeType i = 0; i < m_size; ++i) {
					new (&newBuf[i * m_elementSize]) Value(std::move(get<Value>(i)));
					get<Value>(i).~Value();
				}

				m_values.reset(newBuf);
			}

			return *new (&get<Value>(m_size++)) Value(std::forward<Args>(_args)...);
		}

		void erase(Key _key) {
			ASSERT(contains(_key), "Trying to delete a not existing element.");

			const Key ind = m_slots[_key];
			m_slots[_key] = INVALID_SLOT;
			char *back = &m_values[(m_size - 1) * m_elementSize];

			if (ind + 1 < m_size) {
				m_move(&m_values[m_elementSize * ind], back);
				m_slots[m_valuesToSlots.back()] = ind;
				m_valuesToSlots[ind] = m_valuesToSlots.back();
			}

			--m_size;
			if constexpr (!TrivialDestruct) m_destructor(back);
			m_valuesToSlots.pop_back();
		}

		void clear() {
			m_slots.clear();
			m_valuesToSlots.clear();
			destroyValues();
			m_size = 0;
		}

		// iterators

		template<typename Value>
		class Range {
		public:
			Range(WeakSlotMap &_target) : m_target(_target) {}

			class Iterator {
			public:
				Iterator(WeakSlotMap &_target, SizeType _ind) : m_target(_target), m_index(_ind) {}

				Key key() const { return m_target.m_valuesToSlots[m_index]; }
				Value &value() { return m_target.get<Value>(m_index); }

				Value &operator*() { return m_target.get<Value>(m_index); }
				const Value &operator*() const { return m_target.get<Value>(m_index); }

				Iterator &operator++() {
					++m_index;
					return *this;
				}
				Iterator operator++(int) {
					Iterator tmp(*this);
					++m_index;
					return tmp;
				}
				bool operator==(const Iterator &_oth) const {
					ASSERT(&m_target == &_oth.m_target, "Comparing iterators of different containers.");
					return m_index == _oth.m_index;
				}
				bool operator!=(const Iterator &_oth) const {
					ASSERT(&m_target == &_oth.m_target, "Comparing iterators of different containers.");
					return m_index != _oth.m_index;
				}

			private:
				SizeType m_index;
				WeakSlotMap &m_target;
			};

			Iterator begin() { return Iterator(m_target, 0); }
			Iterator end() { return Iterator(m_target, m_target.m_size); }

		private:
			WeakSlotMap &m_target;
		};

		template<typename Value>
		Range<Value> iterate() { return Range<Value>(*this); }

		// access operations
		bool contains(Key _key) const { return _key < m_slots.size() && m_slots[_key] != INVALID_SLOT; }

		template<typename Value>
		Value &at(Key _key) { return reinterpret_cast<Value &>(m_values[m_slots[_key] * m_elementSize]); }
		template<typename Value>
		const Value &at(Key _key) const { return reinterpret_cast<const Value &>(m_values[m_slots[_key] * m_elementSize]); }

		SizeType size() const { return m_size; }
		bool empty() const { return m_size == 0; }

	protected:
		template<typename Value>
		Value &get(SizeType _ind) { return reinterpret_cast<Value &>(m_values[_ind * m_elementSize]); }

		void destroyValues() {
			if constexpr (TrivialDestruct) return;

			for (SizeType i = 0; i < m_size; ++i)
				m_destructor(&m_values[i * m_elementSize]);
		}

		template<typename Value>
		static void destroyElement(void *ptr) {
			static_cast<Value *>(ptr)->~Value();
		}
		using Destructor = void (*)(void *);

		template<typename Value>
		static void moveElement(void *dst, void *src) {
			*static_cast<Value *>(dst) = std::move(*static_cast<Value *>(src));
		}
		using Move = void (*)(void *, void *);

		Destructor m_destructor;
		Move m_move;
		int m_elementSize;
		SizeType m_size;
		SizeType m_capacity;

		std::unique_ptr<char[]> m_values;

		std::vector<Key> m_slots;
		std::vector<Key> m_valuesToSlots;
	};

	// Allows multiple values for the same Key to be stored.
	/*	template<typename Key, typename Value>
	class MultiSlotMap : public SlotMap<Key, Value>
	{
		using Base = SlotMap<Key, Value>;
	public:
		template<typename... Args>
		Value& emplace(Key _key, Args&&... _args)
		{
			if (Base::contains(_key))
			{
				const Key ind = Base::m_slots[_key];
				m_links.push_back({ind, Base::INVALID_SLOT });
				m_links[ind].next = static_cast<Key>(Base::m_values.size());
				// Base::emplace will set this slot
				Base::m_slots[_key] = Base::INVALID_SLOT; // m_links[ind].next;
			}
			else
				m_links.push_back({ Base::INVALID_SLOT, Base::INVALID_SLOT });
			
			return Base::emplace(_key, std::forward<Args>(_args)...);
		}

		// erases all components associated with this entity
		void erase(Key _key)
		{
			const Key ind = Base::m_slots[_key];
			Key cur = ind;
			do{
				Key temp = m_links[cur].prev;
				eraseSlot(cur);
				cur = temp;

			} while (cur != Base::INVALID_SLOT);

			Base::m_slots[_key] = Base::INVALID_SLOT;
		}

		void clear()
		{
			Base::clear();
			m_links.clear();
		}
	private:
		void eraseSlot(Key _slot)
		{
			if (m_links[_slot].prev != Base::INVALID_SLOT) m_links[m_links[_slot].prev].next = Base::INVALID_SLOT;
			if (m_links[_slot].next != Base::INVALID_SLOT) m_links[m_links[_slot].next].prev = Base::INVALID_SLOT;

			if (_slot+1u < Base::m_values.size())
			{
				Base::m_values[_slot] = std::move(Base::m_values.back());
				Base::m_valuesToSlots[_slot] = Base::m_valuesToSlots.back();

				const Link& link = m_links.back();
				if (link.prev != Base::INVALID_SLOT) m_links[link.prev].next = _slot;
				if (link.next != Base::INVALID_SLOT) m_links[link.next].prev = _slot;
				else Base::m_slots[Base::m_valuesToSlots.back()] = _slot;
				m_links[_slot] = m_links.back();
			}
			Base::m_values.pop_back();
			Base::m_valuesToSlots.pop_back();
			m_links.pop_back();
		}

		struct Link 
		{
			Key prev, next;
		};
		std::vector<Link> m_links;
	};*/
}// namespace utils
