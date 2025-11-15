#include "../stdafx.h"
#include "Play.h"

namespace font = dx2::font;

Play::Play(const InitData& init)
	: IScene{ init }
{
	font::MainFont().preload(OKText);
	font::MainFont().preload(MissText);
	font::MainFont().preload(ClearText);
	font::MainFont().preload(AgainThisText);
	font::MainFont().preload(BackTitleText);
	bgm.setVolume(dx2::math::LinearToLog(getData().volume));
	hit.setVolume(dx2::math::LinearToLog(getData().volume));
	getItem.setVolume(dx2::math::LinearToLog(getData().volume));
}

void Play::update()
{
	const double nowTime = timeLimit - timer.sF();// 時間変化の変数.
	const double createPaceTimerT = createPaceTimer.sF();// dx ビームの生成頻度の時間変数.
	const double itemPaceTimerT = itemPaceTimer.sF();// アイテム生成の時間変数.

	if (SimpleGUI::Slider(U"volume:", getData().volume, Vec2{ windowWidth * 0.05, windowHeight * 0.55 }, 85, 140)) {
		bgm.setVolume(dx2::math::LinearToLog(getData().volume));
	}

	if (dxAddButton.leftClicked()) {
		dxNum = (dxNum + 1 > dxNumLimit) ? dxNumLimit : dxNum + 1;
	}
	if (dxSubButton.leftClicked()) {
		dxNum = (dxNum - 1 < 0) ? 0 : dxNum - 1;
	}

	if (itemAddButton.leftClicked()) {
		itemNum = (itemNum + 1 > itemNumLimit) ? itemNumLimit : itemNum + 1;
	}
	if (itemSubButton.leftClicked()) {
		itemNum = (itemNum - 1 < 0) ? 0 : itemNum - 1;
	}

	// 全体の流れ.
	switch (ps) {
	case PlayScene::Start:

		// 開始.
		if (OK.leftClicked()) {
			pressButton.play();// 押した音を鳴らす.
			bgm.play();
			ps = PlayScene::Play;
			timer.restart();

			// dx を生成.
			createDxBeams();

			// アイテムを生成.
			//createItems(ItemType::Integraler);
			createItems();
			//createItems(ItemType::Flipper);

			// グラフを計算する.
			pGraph->Create(pCamera.get(), [&](const double x) { return dx2::math::EvalHoner(pGraph->CM(), x); });
		}

		break;

	case PlayScene::Play:

		// 終了.
		if (nowTime <= 0.0) {// クリアしたとき.
			gameClearSound.play();
			timer.pause();
			ResultText = ClearText;
			dxBeams.clear();
			items.clear();
			ps = PlayScene::Result;
		}
		if (pPlayer->isCompletelyDxed()) {// 微分されて亡くなったとき.
			gameOverSound.play();
			timer.pause();
			ResultText = MissText;
			dxBeams.clear();
			items.clear();
			ps = PlayScene::Result;
		}


		// アイテムの状態を更新する.
		for (auto& it : items) {
			if (it == nullptr) continue;
			it->update(updateContext);
			if (it->isConsumed()) it = nullptr;
		}

		// プレイヤーの状態を更新する.
		pPlayer->update(pGraph.get(), pCamera.get());

		// dx ビームの状態を更新する.
		for (auto& it : dxBeams) {
			if (it == nullptr) continue;
			it->update(updateContext);
			if (it->isConsumed()) it = nullptr;
		}

		// 次の dx ビームを生成する.
		if (createPaceTimerT >= CREATEPACE) createDxBeams();

		// 次のアイテムを生成する.
		if (itemPaceTimerT >= ITEMPACE) createItems();
		//if (itemPaceTimerT >= ITEMPACE) createItems(ItemType::Integraler);
		//if (itemPaceTimerT >= ITEMPACE) createItems(ItemType::Flipper);

		break;

	case PlayScene::Result:

		// もう一回やるとき.
		if (AgainThis.leftClicked()) {
			pressButton.play();// 押した音を鳴らす.
			timer.reset();

			// player を初期化する.
			pPlayer = nullptr;
			pPlayer = std::make_unique<Player>(initPlayerAxis);

			// グラフを復活させる.
			pGraph = nullptr;
			pGraph = std::make_unique<Graph>(getData().saveData.graph.NumSamples(), getData().saveData.graph.CM());

			ps = PlayScene::Start;
		}

		// タイトルに戻るとき.
		if (BackTitle.leftClicked()) {
			pressButton.play();// 押した音を鳴らす.
			changeScene(State::Title, 1.0s);
		}

		break;

	default:
		break;
	}
}

void Play::draw() const
{
	const double nowTime = timeLimit - timer.sF();// 時間変化の変数.
	const double createPaceTimerT = createPaceTimer.sF();// dx ビームの生成頻度の時間変数.

	// 背景を描く.
	drawBackGround();

	// 生成個数.
	font::MainFont()(U" dx : {}コ"_fmt(dxNum)).draw(30, dxAddButton.pos + Vec2(-160, 0), dx2::palette::base::black);
	dx2::draw::DrawHoverDarkenedButton(dxAddButton, dx2::palette::base::deepBlue, U"↑", font::MainFont(), 30, dx2::palette::base::white);
	dx2::draw::DrawHoverDarkenedButton(dxSubButton, dx2::palette::base::deepBlue, U"↓", font::MainFont(), 30, dx2::palette::base::white);
	font::MainFont()(U"item: {}コ"_fmt(itemNum)).draw(30, itemAddButton.pos + Vec2(-160, 0), dx2::palette::base::black);
	dx2::draw::DrawHoverDarkenedButton(itemAddButton, dx2::palette::base::deepBlue, U"↑", font::MainFont(), 30, dx2::palette::base::white);
	dx2::draw::DrawHoverDarkenedButton(itemSubButton, dx2::palette::base::deepBlue, U"↓", font::MainFont(), 30, dx2::palette::base::white);

	// 全体の流れ.
	switch (ps) {
	case PlayScene::Start:

		// 開始ボタン.
		dx2::draw::DrawHoverDarkenedButton(OK, dx2::palette::base::red, OKText, font::MainFont(), 12, dx2::palette::base::black);
		break;

	case PlayScene::Play:

		pGraph->Draw(pCamera.get());

		for (const auto& it : dxBeams) {
			if (it != nullptr) it->draw();
		}

		for (const auto& it : items) {
			if (it != nullptr) it->draw();
		}

		pPlayer->draw();

		drawUI(nowTime, font::MainFont());

		break;

	case PlayScene::Result:

		if (pPlayer->isCompletelyDxed()) {
			AllWindow.draw(ColorF(0, 0, 0, 0.7));
		}

		font::MainFont()(ResultText).drawAt(40, Vec2(windowWidth * 0.5, windowHeight * 0.3), dx2::palette::base::black);
		dx2::draw::DrawHoverDarkenedButton(AgainThis, dx2::palette::base::green, AgainThisText, font::MainFont(), 32, dx2::palette::base::black);
		dx2::draw::DrawHoverDarkenedButton(BackTitle, dx2::palette::base::green, BackTitleText, font::MainFont(), 32, dx2::palette::base::black);

		break;

	default:
		break;
	}

	//checkAllColor();// 色の確認.
}

void Play::drawFadeIn(double t) const
{
	draw();

	dx2::draw::SceneMove(dx2::palette::base::white, 1 - t);
}

void Play::drawFadeOut(double t) const
{
	draw();

	dx2::draw::SceneMove(dx2::palette::base::black, t);
}

// 背景を描く関数.
void Play::drawBackGround() const
{
	// 背景色.
	Scene::SetBackground(dx2::palette::base::green);

	// グラフ背景.
	ScreenRect.draw(dx2::palette::base::white);
	Line{ ScreenRect.x, ScreenRect.center().y, (ScreenRect.x + ScreenRect.w), ScreenRect.center().y }.drawArrow(2, SizeF{ 20, 20 }, dx2::palette::base::black);// x 軸.
	font::MainFont()(U"x").drawAt(22, Vec2{ ScreenRect.x + ScreenRect.w, ScreenRect.center().y } + Vec2{ -20, 20 }, dx2::palette::base::black);
	Line{ ScreenRect.center().x, (ScreenRect.y + ScreenRect.h), ScreenRect.center().x, ScreenRect.y }.drawArrow(2, SizeF{ 20, 20 }, dx2::palette::base::black);// y 軸.
	font::MainFont()(U"y").drawAt(22, Vec2{ ScreenRect.center().x, ScreenRect.y } + Vec2{ -20, 20 }, dx2::palette::base::black);
	font::MainFont()(U"O").drawAt(22, ScreenRect.center() + Vec2{ -16, 16 }, dx2::palette::base::black);

	// 平面の格子線.
	//player->graph().gs().drawGrid(ScreenRect, player->graph().pa(), player->graph().NumSamples());
}

// UI を描く関数.
void Play::drawUI(const double nowTime, const Font& font) const
{
	// 残り時間.
	font(U"残り:{:.1f}s"_fmt((nowTime >= 0.0) ? nowTime : 0.0)).draw(30, Vec2(40, 50), dx2::palette::base::black);

	// アイテムの残り時間.
	drawItemLastTime(Vec2(windowWidth * 0.8, windowHeight * 0.3), Vec2(windowWidth * 0.8, windowHeight * 0.4), Vec2(windowWidth * 0.8, windowHeight * 0.5), 30, *pPlayer);
}

void Play::drawItemLastTime(const Vec2& sizeVec2, const Vec2& expVec2, const Vec2& flipVec2, const int32_t r, const Player& player) const
{
	auto drawLastTimeCircle = [](const Vec2& center, const int32_t r, const ColorF& color, const SecondsF remaining, const SecondsF limit, const bool haveItem)
		{
			const Circle circle(center, r);
			constexpr double thickness = 5.0;
			if (haveItem) {
				circle.drawFrame(thickness, color).drawPie(0, -2 * std::_Pi_val * (remaining / limit), color);
			}
			else {
				circle.drawFrame(thickness, color);
			}
		};

	ClearPrint();
	Print << U"lastExpTime: {}"_fmt(player.lastExpTime());

	// size.
	drawLastTimeCircle(sizeVec2, r, Player::BodyColor.at(U"normal"), player.lastSizeTime(), player.Cooldowns.at(U"size"), player.isChangedSize());
	font::MainFont()(U": size").draw(30, sizeVec2 + Vec2(r * 2, -r), dx2::palette::base::black);

	// exp.
	drawLastTimeCircle(expVec2, r, Player::BodyColor.at(U"exp"), player.lastExpTime(), player.Cooldowns.at(U"exp"), player.isInvincible());
	font::MainFont()(U": exp").draw(30, expVec2 + Vec2(r * 2, -r), dx2::palette::base::black);
}

void Play::createDxBeams()
{
	// 同時に発生する個数を決める.
	//dxBeams.resize(dxBeamsSize);
	dxBeams.resize(dxNum);

	// dx ビームの中身を詰める.
	for (int32_t i = 0; i < dxBeams.size(); ++i) {
		/*if (rand() % 2 == 0) {
			dxBeams[i] = std::make_unique<Shake>();
		}
		else {
			dxBeams[i] = std::make_unique<Shot>();
		}*/
		dxBeams[i] = std::make_unique<DxItem>(
			setSpawnPoint(),
			Vec2(10.0, 10.0),
			[](const Vec2& velocity) { return ItemMovement::linear(velocity); }
		);
	}

	// 生成頻度のタイマーをスタート.
	createPaceTimer.restart();
}

void Play::createItems()
{
	// 同時に発生する個数を決める.
	//items.resize(itemsSize);
	items.resize(itemNum);

	// アイテムの中身を詰める.
	for (int32_t i = 0; i < items.size(); ++i) {
		const int32_t type = rand() % 5;
		if (type == 0) {
			items[i] = std::make_unique<ExpItem>(
				setSpawnPoint(),
				Vec2(1.0, 1.0),
				[](const Vec2& velocity) { return ItemMovement::wave(velocity); }
			);
		}
		else {
			items[i] = std::make_unique<Integraler>(
				setSpawnPoint(),
				Vec2(1.0, 1.0),
				[](const Vec2& velocity) { return ItemMovement::wave(velocity); }
			);
		}
	}
	//for (int32_t i = 0; i < items.size(); ++i) {
	//	const int32_t type = rand() % 5;
	//	if (type == 0) {
	//		items[i] = std::make_unique<ExpItem>();
	//	}
	//	/*else if (type == 1) {
	//		items[i] = std::make_unique<Smaller>();
	//	}
	//	else if (type == 2) {
	//		items[i] = std::make_unique<Bigger>();
	//	}
	//	else if (type == 3) {
	//		items[i] = std::make_unique<Flipper>();
	//	}*/
	//	else {
	//		items[i] = std::make_unique<Integraler>();
	//	}
	//}

	// 生成頻度のタイマーをスタート.
	itemPaceTimer.restart();
}

void Play::createItems(const ItemType itemType)
{
	items.resize(itemNum);
	if (itemType == ItemType::Exp) {
		for (int32_t i = 0; i < items.size(); ++i) {
			items[i] = std::make_unique<ExpItem>(
				setSpawnPoint(),
				Vec2(1.0, 1.0),
				[](const Vec2& velocity) { return ItemMovement::wave(velocity); }
			);
		}
	}
	/*else if (itemType == ItemType::Smaller) {
		for (int32_t i = 0; i < items.size(); ++i) items[i] = std::make_unique<Smaller>();
	}
	else if (itemType == ItemType::Bigger) {
		for (int32_t i = 0; i < items.size(); ++i) items[i] = std::make_unique<Bigger>();
	}
	else if (itemType == ItemType::Flipper) {
		for (int32_t i = 0; i < items.size(); ++i) items[i] = std::make_unique<Flipper>();
	}*/
	else if (itemType == ItemType::Integraler) {
		for (int32_t i = 0; i < items.size(); ++i) {
			items[i] = std::make_unique<Integraler>(
				setSpawnPoint(),
				Vec2(1.0, 1.0),
				[](const Vec2& velocity) { return ItemMovement::wave(velocity); }
			);
		}
	}
	else {
		Print << U"Undefined Item is called.";
		Print << U"This error is called in Play::createItems.";
	}
	itemPaceTimer.restart();
}
