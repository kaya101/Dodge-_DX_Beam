#pragma once
#include "../Utils/Common.h"
#include "../Game/Player.h"
#include "../Game/Graph.h"
#include "../Game/EffectObject.h"

// プレイシーン.
class Play : public App::Scene
{
public:

	// 全体の流れ.
	enum class PlayScene : int32_t
	{
		Start,
		Play,
		Result,
	};

	explicit Play(const InitData& init);
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
	void drawBackGround() const;
	void drawUI(const double, const Font&) const;

	// アイテムの残り時間の可視化関数.
	void drawItemLastTime(const Vec2& sizeVec2, const Vec2& expVec2, const Vec2& flipVec2, const int32_t r, const Player& player) const;

	void createDxBeams();
	void createItems();
	void createItems(const ItemType itemType);

private:
	// bgm.
	const Audio bgm{ Audio::Stream, U"bgm/EXP.mp3" };
	// リンク.
	// 「https://www.youtube.com/watch?v=wGwfU-1TKe8」.

	PlayScene ps = PlayScene::Start;

	// player.
	SampleCount initPlayerAxis = static_cast<SampleCount>(getData().numSamples / 2);
	std::unique_ptr<Player> pPlayer = std::make_unique<Player>(initPlayerAxis);

	// カメラ.
	//std::unique_ptr<GraphCamera> pCamera = std::make_unique<GraphCamera>(getData().camera);

	// グラフ.
	std::unique_ptr<Graph> pGraph = std::make_unique<Graph>(getData().numSamples, getData().coeffMatrix);

	// まとめセット.
	//UpdateContext updateContext{ *pCamera, *pGraph, *pPlayer };
	UpdateContext updateContext{ *pGraph, *pPlayer };

	// dx ビーム.
	//static constexpr int32_t dxBeamsSize = 4;// 同時生成される個数.
	std::vector<std::unique_ptr<EffectObject>> dxBeams;

	// dx ビームの生成頻度.
	static constexpr double CREATEPACE = 1.6;
	//double createDxPaceTime = 1.6;// 生成頻度.
	Stopwatch createPaceTimer{ StartImmediately::No };

	// アイテム.
	//static constexpr int32_t itemsSize = 4;// 同時生成される個数.
	std::vector<std::unique_ptr<EffectObject>> items;

	// アイテムの生成頻度.
	static constexpr double ITEMPACE = 1.6;
	//double createItemPaceTime = 1.6;// 生成頻度.
	Stopwatch itemPaceTimer{ StartImmediately::No };

	// 制限時間.
	static constexpr double timeLimit = 15.00;
	Stopwatch timer{ StartImmediately::No };

	// ボタンを押した時の音
	const AudioAsset pressButton{ dx2::music::GetSEname(dx2::music::SEname::Button) };

	// 被弾した時の音
	const AudioAsset hit{ dx2::music::GetSEname(dx2::music::SEname::Damaged) };

	// アイテムを獲得した時の音
	const AudioAsset getItem{ dx2::music::GetSEname(dx2::music::SEname::GetItem) };

	// ゲームクリアになった時の音.
	const Audio gameClearSound{ GMInstrument::Piano1, PianoKey::C4, 0.4s };

	// ゲームオーバーになった時の音.
	const Audio gameOverSound{ GMInstrument::Piano1, PianoKey::C4, 0.4s };

	// Start シーン.
	static constexpr RectF OK = { Arg::center(windowWidth * 0.5, windowHeight * 0.7), 120, 50 };
	const String OKText = U"START";

	// Result.
	const String MissText = U"Game Over...";
	const String ClearText = U"Game Clear!!";
	String ResultText = ClearText;           // そのテキストの受け皿.
	static constexpr RectF AgainThis = { Arg::center(windowWidth * 0.5, windowHeight * 0.55), 208, 64 };
	static constexpr RectF BackTitle = { Arg::center(windowWidth * 0.5, windowHeight * 0.7), 208, 64 };
	const String AgainThisText = U"Play Again";
	const String BackTitleText = U"Back Title";

	// 生成する数の変更ボタン.
	static constexpr int32_t dxNumLimit = 64;
	static constexpr RectF dxAddButton = RectF(Arg::center(windowWidth * 0.15, windowHeight * 0.3), 40, 40);
	static constexpr RectF dxSubButton = RectF(Arg::center(windowWidth * 0.20, windowHeight * 0.3), 40, 40);
	int32_t dxNum = 3;
	static constexpr int32_t itemNumLimit = 64;
	static constexpr RectF itemAddButton = RectF(Arg::center(windowWidth * 0.15, windowHeight * 0.4), 40, 40);
	static constexpr RectF itemSubButton = RectF(Arg::center(windowWidth * 0.20, windowHeight * 0.4), 40, 40);
	int32_t itemNum = 1;
};
