#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	using namespace KamataEngine;
	Initialize(L"LE2C_30_リザルデイ_クリスチャン_アングリアワン_ミンタラ_AL3");
	DirectXCommon* dxCommonPtr = DirectXCommon::GetInstance();
	
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();


	while (true)
	{
		if (Update()) 
		{
			break;
		}
	
		gameScene->Update();

		// 画面がここから
		dxCommonPtr->PreDraw();

		gameScene->Draw();
		// ここまで
		dxCommonPtr->PostDraw();
	}

	delete gameScene;
	gameScene = nullptr;
	Finalize();

	return 0;
}
