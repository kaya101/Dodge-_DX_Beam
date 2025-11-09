#pragma once
//#include "../Utils/Common.h"
//#include "../Utils/BaseButton.h"
//#include "../Utils/dx2Library.h"
#include "../Utils/Utils.h"

// タイトルシーン.
class Title : public App::Scene
{
public:
	Title(const InitData& init);
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
	void drawUI() const;

private:
	const AudioAsset bgm{ dx2::music::GetBGMname(dx2::music::BGMname::Title)};

	// title, subTitle.
	const String TitleText = U"避けろ!! dxビーム２";
	const String SubTitleText = U"-- Click the Button to Play --";

	// Start, Quit ボタン.
	enum class ButtonKey : int8_t { Start, Quit };
	static std::map<ButtonKey, TextButton> _buttons;

	static constexpr RectF KeyA = RectF(Arg::center(windowWidth * 0.75, windowHeight * 0.6), 60, 60);
	static constexpr RectF KeyD = RectF(Arg::center(KeyA.center() + Vec2(180, 0)), 60, 60);
	static constexpr RectF KeyLeft = RectF(Arg::center(KeyA.center() + Vec2(0, 80)), 60, 60);
	static constexpr RectF KeyRight = RectF(Arg::center(KeyLeft.center() + Vec2(180, 0)), 60, 60);
};
