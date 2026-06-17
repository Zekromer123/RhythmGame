#include "RhythmManager.h"
void RhythmManager::Initialize() {
	//BPMを秒単位のインターバルに変換
	beatInterval_ = 60.0f / bpm_;
	songTimer_ = -travelTime_;
	notes_.clear();

	for (int i = 0; i < 100; i++) {

		float hitTime = i * beatInterval_;
		if (hitTime >= 4.0f) {
			break;
		}
		Note note{};

		note.hitTime = i * beatInterval_;
		// ノーツが判定ラインに到達する時間から、移動時間を引いて生成時間を決める
		// これにより、音楽とノーツの到達タイミングを同期させる
		note.spawnTime = note.hitTime - travelTime_;
		note.lane = i % 4;
		note.active = false;
		note.judged = false;

		notes_.push_back(note); // 作成したノーツオブジェクトをnotes_ベクターに追加する
	}
}
void RhythmManager::Update(float deltaTime) {

	songTimer_ += deltaTime;
	SpawnNotes();
	CheckInput();
	UpdateNotes();
}
void RhythmManager::SpawnNotes() {

	while (nextSpawnIndex_ < notes_.size()) {

		Note& note = notes_[nextSpawnIndex_];

		if (songTimer_ >= note.spawnTime) {

			note.active = true;
			note.y = startY_;

			nextSpawnIndex_++;
		} else {
			break;
		}
	}
}
// ノーツの動く
void RhythmManager::UpdateNotes() {

	for (auto& note : notes_) {

		if (!note.active || note.judged) {
			continue;
		}

		// 時間ベースの線形補間による座標更新
		float progress = (songTimer_ - note.spawnTime) / travelTime_;

		progress = std::clamp(progress, 0.0f, 1.0f);

		note.y = startY_ + (hitLineY_ - startY_) * progress;

		if (songTimer_ > note.hitTime + 0.25f) {

			note.judged = true;

			judgement = "MISS";
		}
	}
}

void RhythmManager::CheckInput() {
	int pressedLane = -1;
	if (Input::GetInstance()->TriggerKey(DIK_A)) {

		pressedLane = 0;
	}
	if (Input::GetInstance()->TriggerKey(DIK_W)) {

		pressedLane = 1;
	}
	if (Input::GetInstance()->TriggerKey(DIK_S)) {
		pressedLane = 2;
	}
	if (Input::GetInstance()->TriggerKey(DIK_D)) {

		pressedLane = 3;
	}

	if (pressedLane == -1) {
		return;
	}
	Note* targetNote = nullptr;
	float minDiff = 100.0f;

	for (auto& note : notes_) {

		if (!note.active || note.judged || note.lane != pressedLane) {
			continue;
		}
		// プレイヤーの入力時間とノーツの目標時間の差分を計算し、判定（Perfect/Good）を行う
		float diff = fabsf(songTimer_ - note.hitTime);
		if (diff < minDiff) {
			minDiff = diff;
			targetNote = &note;
		}
	}
	if (targetNote != nullptr && minDiff <= 0.20f) {
		if (minDiff <= 0.06f)
			judgement = "PERFECT!";
		else if (minDiff <= 0.12f)
			judgement = "GOOD!";

		targetNote->judged = true;
	}
}
void RhythmManager::Draw() {

	PrimitiveDrawer* pd = PrimitiveDrawer::GetInstance();

	// --- ヒットの線---
	pd->DrawLine3d({200, hitLineY_, 0}, {1080, hitLineY_, 0}, {1, 1, 1, 1});

	// 線の設定
	float laneWidth = 100.0f;
	float spacing = 20.0f; // 線の距離
	float totalWidth = (laneWidth * 4.0f) + (spacing * 3.0f);
	float startX = (1280.0f - totalWidth) / 2.0f;

	for (int i = 0; i < 4; i++) {
		float x = startX + (i * (laneWidth + spacing));
		pd->DrawLine3d({x, hitLineY_, 0}, {x + laneWidth, hitLineY_, 0}, {1.0f, 1.0f, 1.0f, 1.0f});
	}
	// ノーツ
	for (auto& note : notes_) {
		if (!note.active || note.judged)
			continue;
		Vector4 color = {1, 0, 0, 1}; // Default Merah
		if (note.lane == 0)
			color = {1.0f, 0.2f, 0.2f, 1.0f}; // 赤色
		if (note.lane == 1)
			color = {0.2f, 1.0f, 0.2f, 1.0f}; // 緑色
		if (note.lane == 2)
			color = {0.5f, 0.5f, 1.0f, 1.0f}; // 青色
		if (note.lane == 3)
			color = {1.0f, 1.0f, 0.2f, 1.0f}; // 黄色
		float height = 20.0f;

		float laneX = startX + (note.lane * (laneWidth + spacing));
		float centerX = laneX + (laneWidth / 2.0f);
		float centerY = note.y;
		Vector3 topLeft = {centerX - laneWidth / 2, centerY - height / 2, 0};
		Vector3 topRight = {centerX + laneWidth / 2, centerY - height / 2, 0};
		Vector3 bottomLeft = {centerX - laneWidth / 2, centerY + height / 2, 0};
		Vector3 bottomRight = {centerX + laneWidth / 2, centerY + height / 2, 0};
		// 4　線を組み合わせて四角形に作る
		pd->DrawLine3d(topLeft, topRight, color);                                                            // 上
		pd->DrawLine3d(bottomLeft, bottomRight, color);                                                      // 下
		pd->DrawLine3d(topLeft, bottomLeft, color);                                                          // 左
		pd->DrawLine3d(topRight, bottomRight, color);                                                        // 右
		pd->DrawLine3d({centerX - laneWidth / 2, centerY, 0}, {centerX + laneWidth / 2, centerY, 0}, color); // 真ん中の線
	}
	// レーンを見やすくするための縦方向のガイドライン表示
	for (int i = 0; i <= 4; i++) {
		float x = (startX - spacing / 2.0f) + (i * (laneWidth + spacing));
		pd->DrawLine3d({x, 0, 0}, {x, 720, 0}, {0.2f, 0.2f, 0.2f, 1.0f});
	}

	//------UI------------
	DebugText::GetInstance()->SetPos(600, 400);
	DebugText::GetInstance()->SetScale(2.0f);
	if (judgement != "") {
		DebugText::GetInstance()->Printf("%s", judgement.c_str());
	}
	DebugText::GetInstance()->SetPos(startX + laneWidth / 2, hitLineY_ + 50.0f);
	DebugText::GetInstance()->Printf("A");

	DebugText::GetInstance()->SetPos(startX + laneWidth + spacing, hitLineY_ + 50.0f);
	DebugText::GetInstance()->Printf("W");

	DebugText::GetInstance()->SetPos(startX + laneWidth * 2 + 50.0f, hitLineY_ + 50.0f);
	DebugText::GetInstance()->Printf("S");

	DebugText::GetInstance()->SetPos(startX + laneWidth * 3 + spacing + 50.0f, hitLineY_ + 50.0f);
	DebugText::GetInstance()->Printf("D");
}

void RhythmManager::StartSong(uint32_t soundHandle) { Audio::GetInstance()->PlayWave(soundHandle, false); }

void RhythmManager::StopSong(uint32_t soundHandle) { Audio::GetInstance()->StopWave(soundHandle); }
