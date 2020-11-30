#pragma once

#include <vector>
#include "engine/graphics/core/geometrybuffer.hpp"

namespace graphics {
	class Mesh {
	public:
		Mesh(const std::string &path);
		void draw() const;

	private:
		static const std::vector<VertexAttribute> attributes;
		GeometryBuffer geometryBuffer;
	};
}
