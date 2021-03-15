#pragma once
#include "../camera.hpp"
#include "../core/shader.hpp"
#include "engine/graphics/core/texture.hpp"
#include "mesh.hpp"
#include <engine/graphics/core/framebuffer.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace graphics {
	class MeshRenderer {
	public:
		MeshRenderer();

		void draw(const Mesh &_mesh, const Texture2D &_texture, const glm::mat4 _transform);
		void present(const Camera &_camera, const glm::vec3& lightDirection = glm::vec3(-.2, -.5, -.2));
		void clear();
		void setLightingShader(const Program &shader);

	private:
		struct MeshInstance {
			const Mesh &mesh;
			const Texture2D &texture;
			const glm::mat4 transform;
		};

		Program geometryProgram;
		std::vector<MeshInstance> meshQueue;
		int viewMatrixLocation{};
		int mvpMatrixLocation{};
		int albedoTextureLocation{};

		// Deferred rendering
		FrameBuffer geometryBuffer;
		Texture2D *depthTexture;
		Texture2D *albedoTexture{};
		Texture2D *normalTexture{};
		Texture2D *positionTexture{};
		Program lightingProgram;
		Mesh quad;

		int positionTextureLocation;
		int normalTextureLocation;
		int lightingAlbedoTextureLocation;

		void createGeometryProgram();
		void findUniformLocations();
		void renderAllModels(const Camera &_camera);
		void createLightingProgram();
		void createGeometryBuffer(const glm::ivec2 &size);
	};
}// namespace graphics