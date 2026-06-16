#include "RhythmManager.h"
void RhythmManager::Initialize() {

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
		note.spawnTime = note.hitTime - travelTime_;
		note.lane = i % 4;
		note.active = false;
		note.judged = false;

		notes_.push_back(note);
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
void RhythmManager::UpdateNotes() {

	for (auto& note : notes_) {

		if (!note.active || note.judged) {
			continue;
		}

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

	// --- Gambar Garis Hit ---
	pd->DrawLine3d({200, hitLineY_, 0}, {1080, hitLineY_, 0}, {1, 1, 1, 1});

	// konfigurasi jalur
	float laneWidth = 100.0f;
	float spacing = 20.0f; // jarak antar jalur
	float totalWidth = (laneWidth * 4.0f) + (spacing * 3.0f);
	float startX = (1280.0f - totalWidth) / 2.0f;

	for (int i = 0; i < 4; i++) {
		float x = startX + (i * (laneWidth + spacing));
		pd->DrawLine3d({x, hitLineY_, 0}, {x + laneWidth, hitLineY_, 0}, {1.0f, 1.0f, 1.0f, 1.0f});
	}

	// 2. Gambar Note (Balok) berdasarkan Lane masing-masing
	for (auto& note : notes_) {
		if (!note.active || note.judged)
			continue;
		// Tentukan warna berdasarkan lane (A,W,S,D) agar pemain tidak bingung
		Vector4 color = {1, 0, 0, 1}; // Default Merah
		if (note.lane == 0)
			color = {1.0f, 0.2f, 0.2f, 1.0f}; // Merah (A)
		if (note.lane == 1)
			color = {0.2f, 1.0f, 0.2f, 1.0f}; // Hijau (W)
		if (note.lane == 2)
			color = {0.5f, 0.5f, 1.0f, 1.0f}; // Biru (S)
		if (note.lane == 3)
			color = {1.0f, 1.0f, 0.2f, 1.0f}; // Kuning (D)
		float height = 20.0f;
		// Hitung centerX berdasarkan lane note tersebut
		float laneX = startX + (note.lane * (laneWidth + spacing));
		float centerX = laneX + (laneWidth / 2.0f);
		float centerY = note.y;
		// Hitung 4 titik sudut balok wireframe
		Vector3 topLeft = {centerX - laneWidth / 2, centerY - height / 2, 0};
		Vector3 topRight = {centerX + laneWidth / 2, centerY - height / 2, 0};
		Vector3 bottomLeft = {centerX - laneWidth / 2, centerY + height / 2, 0};
		Vector3 bottomRight = {centerX + laneWidth / 2, centerY + height / 2, 0};
		// Gambar 4 garis membentuk kotak
		pd->DrawLine3d(topLeft, topRight, color);       // Atas
		pd->DrawLine3d(bottomLeft, bottomRight, color); // Bawah
		pd->DrawLine3d(topLeft, bottomLeft, color);     // Kiri
		pd->DrawLine3d(topRight, bottomRight, color);   // Kanan
		                                                // Garis tengah horizontal (agar balok terlihat lebih padat)
		pd->DrawLine3d({centerX - laneWidth / 2, centerY, 0}, {centerX + laneWidth / 2, centerY, 0}, color);
	}
	// 3. (Opsional) Gambar Garis Pembatas Vertikal agar jalur terlihat jelas
	for (int i = 0; i <= 4; i++) {
		float x = (startX - spacing / 2.0f) + (i * (laneWidth + spacing));
		pd->DrawLine3d({x, 0, 0}, {x, 720, 0}, {0.2f, 0.2f, 0.2f, 1.0f}); // Garis abu-abu gelap
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
