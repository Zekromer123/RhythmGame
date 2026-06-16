#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	camera.Initialize();
	camera.matProjection = Mathematics::MakeOrthographicMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 100.0f);
	camera.translation_ = {0.0f, 0.0f, -1.0f};
	camera.UpdateMatrix();

	DebugText::GetInstance()->Initialize();

	rhythmManager = new RhythmManager();
	rhythmManager->Initialize();
	soundHandle_ = Audio::GetInstance()->LoadWave("60bpm.wav");
}

void GameScene::Update() {
	PrimitiveDrawer::GetInstance()->Reset();
	camera.UpdateMatrix();
	camera.matProjection = Mathematics::MakeOrthographicMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 100.0f);
	camera.TransferMatrix();

	if (audioPlayTimer > 0) {
		audioPlayTimer--;
		countdown = audioPlayTimer / 60;
	}

	if (!isFinished) {
		float deltaTime = 1.0f / 60.0f;
		rhythmManager->Update(deltaTime);
		if (!isPlaying && countdown <= 0) {
			isPlaying = true;
			rhythmManager->StartSong(soundHandle_);
		}

		if (rhythmManager->GetSongTimer() >= 4.0f) {
			rhythmManager->StopSong(soundHandle_);
		}
		if (rhythmManager->GetSongTimer() >= 6.0f) {
			isPlaying = false;
			isFinished = true;
		}
	}
}

void GameScene::Draw() {
	PrimitiveDrawer::GetInstance()->SetCamera(&camera);
	if (!isFinished) {
		rhythmManager->Draw();
	} else {
		DebugText::GetInstance()->Print("FINISHED", 540, 360, 3.0f);
	}

	Sprite::PreDraw();

	DebugText::GetInstance()->DrawAll();
	if (audioPlayTimer > 0) {
		DebugText::GetInstance()->SetPos(600.0f, 360.0f);
		DebugText::GetInstance()->Printf("%d", countdown);
	}
	Sprite::PostDraw();
}
