#pragma once
#include "engine/game/registry/Components.hpp"
#include <engine/graphics/renderer/meshrenderer.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>
#include "engine/utils/meshloader.hpp"

namespace game {
	class Actions {
	public:
		static void Draw(graphics::MeshRenderer& _meshRenderer, Registry& _registry) {
			_registry.execute<Mesh, Texture, Transform, Visibility>([&](Mesh _mesh, Texture _texture, Transform& _transform, Visibility& _visibility) {
				if (_visibility.visible==true) {
					_meshRenderer.draw(*_mesh.mesh, *_texture.texture, glm::scale(glm::translate(glm::mat4(1), _transform.position) * glm::toMat4(_transform.rotation), _transform.scale));
				}

			}); 
		}

		static void UpdateRotation(Registry& _registry, float _deltaTime) {
			_registry.execute<Transform, AngularVelocity>([&] (Transform& _transform, AngularVelocity& _angularVelocity) {				
				_transform.rotation = glm::slerp(_transform.rotation, _transform.rotation * _angularVelocity.angularVelocity, _deltaTime/5);
				});
		}
		static void UpdateCratePosition(Registry& _registry, float _deltaTime) {
			_registry.execute<Velocity, Transform, ObjectType>([&](Velocity& _velocity, Transform& _transform, ObjectType& _objectType) {
				if (_objectType.type == 0) {
					if (_transform.position.x >= 35 || _transform.position.x <= -35) {
						_velocity.velocity.x = -_velocity.velocity.x;
					}
					if (_transform.position.y >= 25 || _transform.position.y <= -25) {
						_velocity.velocity.y = -_velocity.velocity.y;
					}

					if (_transform.position.z >= 25 || _transform.position.z <= -25) {
						_velocity.velocity.z = -_velocity.velocity.z;
					}

				}

				_transform.position.x += _velocity.velocity.x * _deltaTime;
				_transform.position.y += _velocity.velocity.y * _deltaTime;
				_transform.position.z += _velocity.velocity.z * _deltaTime;
				});
		}
		static void AddAABB(Registry& _registry, Entity& _ent, const std::string& _path, graphics::Camera& _camera) {
			Transform& transdata = _registry.getComponentUnsafe<Transform>(_ent);
			glm::mat4 transmat = glm::scale(glm::translate(glm::mat4(1), transdata.position) * glm::toMat4(transdata.rotation), transdata.scale);
			glm::vec3 min;
			glm::vec3 max;

			glm::vec3 min_trans;
			glm::vec3 max_trans;
			
			glm::vec3 transface;

			auto meshData = utils::MeshLoader::get(_path.c_str());

			min = meshData->positions[0];
			max = meshData->positions[0];

			min_trans = glm::vec3(_camera.getViewProjection() * transmat * glm::vec4(meshData->positions[0], 1));
			max_trans = glm::vec3(_camera.getViewProjection() * transmat * glm::vec4(meshData->positions[0], 1));
	
			for (auto& face : meshData->positions) {
				transface = glm::vec3(_camera.getViewProjection() * transmat * glm::vec4(face, 1));
				if (min.x > face.x) {
					min.x = face.x;
				}
				if (min.y > face.y) {
					min.y = face.y;
				}
				if (min.z > face.z) {
					min.z = face.z;
				}
				if (max.x < face.x) {
					max.x = face.x;
				}
				if (max.y < face.y) {
					max.y = face.y;
				}
				if (max.z < face.z) {
					max.z = face.z;
				}
				if (min_trans.x > transface.x) {
					min_trans.x = transface.x;
				}
				if (min_trans.y > transface.y) {
					min_trans.y = transface.y;
				}
				if (min_trans.z > transface.z) {
					min_trans.z = transface.z;
				}
				if (max_trans.x < transface.x) {
					max_trans.x = transface.x;
				}
				if (max_trans.y < transface.y) {
					max_trans.y = transface.y;
				}
				if (max_trans.z < transface.z) {
					max_trans.z = transface.z;
				}				
			}			
			_registry.addComponent<AABB>(_ent, math::AABB<3>(min, max), math::AABB<3>(min_trans, max_trans), 0);
		}
		static void UpdateAABB(Registry& _registry, graphics::Camera& _camera) {
			_registry.execute<Transform, AABB>([&](Transform& _transform, AABB& _aabb) {
				std::vector<glm::vec3> box;
				box.push_back(glm::vec3(_aabb.untransformed_box.min.x, _aabb.untransformed_box.min.y, _aabb.untransformed_box.min.z));
				box.push_back(glm::vec3(_aabb.untransformed_box.min.x, _aabb.untransformed_box.min.y, _aabb.untransformed_box.max.z));
				box.push_back(glm::vec3(_aabb.untransformed_box.min.x, _aabb.untransformed_box.max.y, _aabb.untransformed_box.min.z));
				box.push_back(glm::vec3(_aabb.untransformed_box.min.x, _aabb.untransformed_box.max.y, _aabb.untransformed_box.max.z));
				box.push_back(glm::vec3(_aabb.untransformed_box.max.x, _aabb.untransformed_box.min.y, _aabb.untransformed_box.min.z));
				box.push_back(glm::vec3(_aabb.untransformed_box.max.x, _aabb.untransformed_box.min.y, _aabb.untransformed_box.max.z));
				box.push_back(glm::vec3(_aabb.untransformed_box.max.x, _aabb.untransformed_box.max.y, _aabb.untransformed_box.min.z));
				box.push_back(glm::vec3(_aabb.untransformed_box.max.x, _aabb.untransformed_box.max.y, _aabb.untransformed_box.max.z));

				glm::mat4 transmat = glm::scale(glm::translate(glm::mat4(1), _transform.position) * glm::toMat4(_transform.rotation), _transform.scale);

				glm::vec3 min = glm::vec3(_camera.getViewProjection() * transmat * glm::vec4(box[0], 1));
				glm::vec3 max = glm::vec3(_camera.getViewProjection() * transmat * glm::vec4(box[0], 1));

				glm::vec3 transvec;

				for (auto& vec : box) {
					transvec = glm::vec3(_camera.getViewProjection() * transmat * glm::vec4(vec, 1));
					if (min.x > vec.x) {
						min.x = vec.x;
					}
					if (min.y > vec.y) {
						min.y = vec.y;
					}
					if (min.z > vec.z) {
						min.z = vec.z;
					}
					if (max.x < vec.x) {
						max.x = vec.x;
					}
					if (max.y < vec.y) {
						max.y = vec.y;
					}
					if (max.z < vec.z) {
						max.z = vec.z;
					}
				}
				_aabb.transformed_box.min = min;
				_aabb.transformed_box.max = max;

				});
		}

	};
	
	/*class Physics {
		Update(float _time, float _deltaTime) {

		}
	public:
	private:
	};*/
}