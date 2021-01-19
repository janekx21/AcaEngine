#pragma once

	template<typename T>
	concept component_type = std::movable<T>;
