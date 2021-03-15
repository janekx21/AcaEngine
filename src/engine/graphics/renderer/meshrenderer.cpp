#include "meshrenderer.hpp"
#include "engine/graphics/core/device.hpp"
#include "engine/graphics/core/shader.hpp"

namespace graphics {
	MeshRenderer::MeshRenderer() : meshQueue(), quad(graphics::Mesh("models/quad.obj")), geometryBuffer() {
		createGeometryProgram();
		createLightingProgram();

		findUniformLocations();

		auto size = graphics::Device::getBufferSize();
		createGeometryBuffer(size);
	}
	void MeshRenderer::createGeometryProgram() {
		geometryProgram = Program();
		geometryProgram.attach(ShaderManager::get("shader/model.vert", ShaderType::VERTEX));
		geometryProgram.attach(ShaderManager::get("shader/model.frag", ShaderType::FRAGMENT));
		geometryProgram.link();
	}

	void MeshRenderer::createLightingProgram() {
		lightingProgram = Program();
		lightingProgram.attach(graphics::ShaderManager::get("shader/fullscreen.vert", ShaderType::VERTEX));
		lightingProgram.attach(graphics::ShaderManager::get("shader/phong.frag", ShaderType::FRAGMENT));
		lightingProgram.link();
	}

	void MeshRenderer::findUniformLocations() {
		viewMatrixLocation = geometryProgram.getUniformLoc("view_matrix");
		mvpMatrixLocation = geometryProgram.getUniformLoc("mvp_matrix");
		albedoTextureLocation = geometryProgram.getUniformLoc("albedo_texture");
	}

	void MeshRenderer::createGeometryBuffer(const glm::ivec2 &size) {
		depthTexture = Texture2D::create(size.x, size.y, TexFormat::D32F, Sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Border::CLAMP));
		albedoTexture = Texture2D::create(size.x, size.y, TexFormat::RGB8, Sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Border::CLAMP));
		normalTexture = Texture2D::create(size.x, size.y, TexFormat::RGBA16F, Sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Border::CLAMP));
		positionTexture = Texture2D::create(size.x, size.y, TexFormat::RGBA16F, Sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Border::CLAMP));
		geometryBuffer.attachDepth(*depthTexture, 0);
		geometryBuffer.attach(0, *positionTexture, 0);
		geometryBuffer.attach(1, *normalTexture, 0);
		geometryBuffer.attach(2, *albedoTexture, 0);
	}

	void MeshRenderer::draw(const Mesh &_mesh, const Texture2D &_texture, const glm::mat4 _transform) {
		struct MeshInstance mesh_instance = {_mesh, _texture, _transform};
		MeshRenderer::meshQueue.push_back(mesh_instance);
	}

	void MeshRenderer::present(const Camera &_camera) {
		if (meshQueue.empty()) return;

		geometryBuffer.bind();
		geometryBuffer.clear(_camera.backgroundColor);
		renderAllModels(_camera);
		geometryBuffer.unbind();

		lightingProgram.use();
		{
			const auto slot = 0;
			positionTexture->bind(slot);
			auto location = lightingProgram.getUniformLoc("position_texture");
			lightingProgram.setUniform(location, slot);
		}
		{
			const auto slot = 1;
			normalTexture->bind(slot);
			auto location = lightingProgram.getUniformLoc("normal_texture");
			lightingProgram.setUniform(location, slot);
		}
		{
			const auto slot = 2;
			albedoTexture->bind(slot);
			auto location = lightingProgram.getUniformLoc("albedo_texture");
			lightingProgram.setUniform(location, slot);
		}

		quad.draw();
	}

	void MeshRenderer::renderAllModels(const Camera &_camera) {
		geometryProgram.use();
		int textureBindLocation = 0;

		for (auto &instance : meshQueue) {
			instance.texture.bind(textureBindLocation);
			geometryProgram.setUniform(albedoTextureLocation, textureBindLocation);
			geometryProgram.setUniform(viewMatrixLocation, _camera.getView());
			geometryProgram.setUniform(geometryProgram.getUniformLoc("model_matrix"), instance.transform);
			geometryProgram.setUniform(mvpMatrixLocation, _camera.getViewProjection() * instance.transform);
			instance.mesh.draw();
		}
	}

	void MeshRenderer::clear() {
		meshQueue.clear();
	}
}// namespace graphics