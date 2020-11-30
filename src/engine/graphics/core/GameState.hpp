#pragma once
class GameState
{
public:
	void Init();
	void Cleanup();

	void onPause();
	void onResume();

	void HandleEvents();
	virtual void update(float _time, float _deltaTime) = 0;
	virtual void draw(float _time, float _deltaTime) = 0;
	bool is_finished;
	bool isFinished();
};
