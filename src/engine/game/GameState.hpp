#pragma once

namespace game {
	class GameState
	{
	public:
		explicit GameState() = default;
		virtual void onPause() = 0;
		virtual void onResume() = 0;

		virtual void update(float _time, float _deltaTime) = 0;
		virtual void draw(float _time, float _deltaTime) = 0;

		virtual bool getIsFinished() = 0;
	};
}
