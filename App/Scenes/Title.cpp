#include "../stdafx.h"
#include "Title.h"

namespace font = dx2::font;

Title::Title(const InitData& init)
	: IScene{ init }
{
	font::TitleFont().preload(TitleText);
	font::TitleFont().preload(SubTitleText);
	for (const auto& it : _buttons) font::MainFont().preload(it.second.Text());
	bgm.setVolume(dx2::math::LinearToLog(getData().volume));
	srand(time(0));
}

void Title::update()
{
	bgm.play();

	if (SimpleGUI::Slider(U"volume:", getData().volume, Vec2{ windowWidth * 0.05, windowHeight * 0.55 }, 85, 140))
	{
		bgm.setVolume(dx2::math::LinearToLog(getData().volume));
	}

	// スタートボタン.
	if (_buttons.at(ButtonKey::Start).IsReleased())
	{
		changeScene(State::SelectFiles, 1.0s);
	}

	// やめるボタン.
	if (_buttons.at(ButtonKey::Quit).IsReleased())
	{
		System::Exit();
	}
}

void Title::draw() const
{
	Scene::SetBackground(dx2::palette::base::white);
	drawUI();
}

void Title::drawFadeIn(double t) const
{
	draw();
	dx2::draw::SceneMove(dx2::palette::base::black, 1 - t);
}

void Title::drawFadeOut(double t) const
{
	draw();
	dx2::draw::SceneMove(dx2::palette::base::white, t);
}

void Title::drawUI() const
{
	font::TitleFont()(TitleText).drawAt(64, Vec2(windowWidth * 0.5, windowHeight * 0.2), dx2::palette::base::black);
	font::TitleFont()(SubTitleText).drawAt(32, Vec2(windowWidth * 0.5, windowHeight * 0.3), dx2::palette::base::black);

	for (const auto& it : _buttons) it.second.draw();

	font::MainFont()(U"・負の方向へ").drawAt(26, KeyA.center() + Vec2(0, -KeyA.h), dx2::palette::base::black);
	KeyA.rounded(5).draw(dx2::palette::pastel::blue);
	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(U"A").drawAt(26, KeyA.center(), dx2::palette::base::black);
	KeyLeft.rounded(5).draw(dx2::palette::pastel::blue);
	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(U"←").drawAt(26, KeyLeft.center(), dx2::palette::base::black);

	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(U"・正の方向へ").drawAt(26, KeyD.center() + Vec2(0, -KeyD.h), dx2::palette::base::black);
	KeyD.rounded(5).draw(dx2::palette::pastel::blue);
	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(U"D").drawAt(26, KeyD.center(), dx2::palette::base::black);
	KeyRight.rounded(5).draw(dx2::palette::pastel::blue);
	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(U"→").drawAt(26, KeyRight.center(), dx2::palette::base::black);
}

std::map<Title::ButtonKey, TextButton> Title::_buttons =
{
	{
		ButtonKey::Start,
		TextButton(RectF(Arg::center(windowWidth * 0.5, windowHeight * 0.6), SizeF(270, 90)), dx2::palette::base::yellow, U"スタート")
	},
	{
		ButtonKey::Quit,
		TextButton(RectF(Arg::center(windowWidth * 0.5, windowHeight * 0.8), SizeF(270, 90)), dx2::palette::base::yellow, U"やめる")
	}
};
