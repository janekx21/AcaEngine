#include "keyboardInterface.hpp"
#include "../utils/assert.hpp"
#include <nlohmann/json.hpp>

namespace input {
	using namespace std::string_literals;
	using namespace nlohmann;

	// Mouse and keyboard mappings are encoded as a single int.
	// To differentiate between them mouse buttons k are stored as
	// negative numbers -k-1 .
	void to_json(json &j, const MKBKey &_key) {
		if (std::holds_alternative<MouseButton>(_key))
			j = -static_cast<int>(std::get<MouseButton>(_key)) - 1;
		else
			j = static_cast<int>(std::get<Key>(_key));
	}

	void from_json(const json &j, MKBKey &_key) {
		// entry not found -> mark as such
		if (j.is_null()) _key = std::monostate{};
		else {
			const int i = j.get<int>();
			if (i >= 0) _key = static_cast<Key>(i);
			else
				_key = static_cast<MouseButton>(-(i + 1));
		}
	}

	KeyboardInterface::KeyboardInterface(nlohmann::json &_config,
																			 const std::vector<std::pair<std::string, MKBKey>> &_defaults,
																			 const std::vector<VirtualAxis> &_axis)
			: m_axis(_axis) {
		unsigned c = 0;
		for (const auto &[name, key] : _defaults) {
			MKBKey k = _config[name].get<MKBKey>();
			if (std::holds_alternative<std::monostate>(k)) {
				_config[name] = key;
				k = key;
			}
			m_inputMap.add(Action(c), k);
			++c;
		}
	}


	bool KeyboardInterface::isKeyPressed(Action _action) const {
		auto hndl = m_inputMap.find(_action);
		ASSERT(!!hndl, "Action is not mapped to a key.");
		return isKeyPressed(hndl.data());
	}

	ActionState KeyboardInterface::getKeyState(Action _action) const {
		auto hndl = m_inputMap.find(_action);
		ASSERT(!!hndl, "Action is not mapped to a key.");
		const MKBKey &key = hndl.data();

		if (std::holds_alternative<MouseButton>(key))
			return InputManager::getButtonState(std::get<MouseButton>(key));
		else
			return InputManager::getKeyState(std::get<Key>(key));
	}

	float KeyboardInterface::getAxis(Axis _axis) const {
		const VirtualAxis va = m_axis[_axis.id()];

		float axis = 0.f;
		if (isKeyPressed(va.low))
			axis -= 1.f;
		if (isKeyPressed(va.high))
			axis += 1.f;

		return axis;
	}

	glm::vec2 KeyboardInterface::getCursorPos() const {
		return InputManager::getCursorPos();
	}

	bool KeyboardInterface::isKeyPressed(const MKBKey &_key) {
		if (std::holds_alternative<MouseButton>(_key))
			return InputManager::isButtonPressed(std::get<MouseButton>(_key));
		else
			return InputManager::isKeyPressed(std::get<Key>(_key));
	}
}// namespace input