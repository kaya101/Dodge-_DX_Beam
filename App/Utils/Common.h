#pragma once
#include "../Game/SaveDataManager.h"

// シーンの名前.
enum class State : int8_t
{
	Title,
	Play,
	SelectFiles,
	EditFunction
};

// 共有するデータ.
struct GameData
{
	double volume = 0.0;// 音量.

	SaveData saveData;// 係数行列は 5 次までを想定.

	// グラフカメラとサンプリング数を調整する.
	void Adjust(const RectF& screenBox)
	{
		saveData.camera.SetScreenCameraPos(screenBox.center()).SetScreenCameraSize(screenBox.size);
		saveData.graph.SetNumSamples(screenBox.w);
	}

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive) { archive(volume, saveData); }
};

using App = SceneManager<State, GameData>;

// 初期データ.
static const GameData defaultGameData
{
	0.0,
	SaveData(
		SaveDataManager().SubDirNames()[0],
		GraphCamera(Vec2(0.0, 0.0), normalLR, ScreenRect.center(), ScreenRect.size),
		Graph(N, dx2::math::DefaultCM)
	)
};
