#pragma once
#include "engine/game/registry/Components.hpp"
#include <engine/graphics/renderer/meshrenderer.hpp>
#include <glm/gtx/quaternion.hpp>
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

	};
	
	/*class Physics {
		Update(float _time, float _deltaTime) {

		}
	public:
	private:
	};*/
}