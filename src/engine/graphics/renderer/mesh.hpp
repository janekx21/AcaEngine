#pragma once

#include "engine/graphics/core/geometrybuffer.hpp"
#include <string>
#include <vector>

namespace graphics {
	class Mesh {
	public:
		Mesh(const std::string &path);
		void draw() const;

	private:
		static const std::vector<VertexAttribute> attributes;
		GeometryBuffer geometryBuffer;
	};
}// namespace graphics
