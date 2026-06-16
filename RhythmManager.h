#pragma once
#include <vector>
#include <algorithm>
#include "KamataEngine.h"

using namespace KamataEngine;
struct Note {
	float hitTime;
	float spawnTime;
	float y;
	int lane;
	bool active;
	bool judged;
};

class RhythmManager {
public:
	void Initialize();
	void Update(float deltaTime);
	void Draw();
	void StartSong(uint32_t soundHandle);
	void StopSong(uint32_t soundHandle);
	float GetSongTimer() { return songTimer_; };

private:
	void SpawnNotes();
	void UpdateNotes();
	void CheckInput();

private:
	std::vector<Note> notes_;
	std::string judgement = "";

	//float kAudioOffset = -0.5f;
	float songTimer_ = 0.0f;
	float bpm_ = 120.0f;
	float beatInterval_ = 0.0f;

	float travelTime_ = 2.0f;

	float startY_ = -100.0f;
	float hitLineY_ = 600.0f;

	int nextSpawnIndex_ = 0;
};