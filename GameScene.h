#pragma once
#include "KamataEngine.h"
#include "RhythmManager.h"
#include "Mathematics.h"

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize() { delete rhythmManager; }
	KamataEngine::Camera& GetCamera() { return camera; }

private:
	KamataEngine::Camera camera{};
	RhythmManager* rhythmManager = nullptr;
	uint32_t soundHandle_ = 0;
	int audioPlayTimer = 180;
	bool isPlaying = false;
	bool isFinished = false;
	int countdown = 0;
};
