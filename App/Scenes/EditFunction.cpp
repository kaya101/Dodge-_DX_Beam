#include "../stdafx.h"
#include "EditFunction.h"
#include "../Game/SaveDataManager.h"

namespace font = dx2::font;

EditFunction::EditFunction(const InitData& init)
	: IScene(init)
{
	font::MainFont().preload(EditFunctionTitle);
	getData().Adjust(ScreenRect);
}

void EditFunction::update()
{
	pGraphTool->update(pGraph.get(), pCamera.get());

	// 決定ボタン.
	if (_buttons.at(ButtonKey::Decide).IsReleased())
	{
		SaveDataManager().Store(getData());
		changeScene(State::SelectFiles, 1.0s);
	}

	// リセットボタン.
	if (_buttons.at(ButtonKey::Reset).IsReleased())
	{
		SaveDataManager().Load(getData());
	}

	// 戻るボタン.
	if (_buttons.at(ButtonKey::Return).IsReleased())
	{
		SaveDataManager().Load(getData());
		changeScene(State::SelectFiles, 1.0s);
	}
}

void EditFunction::draw() const
{
	Scene::SetBackground(dx2::palette::pastel::water);
	drawUI();
}

void EditFunction::drawFadeIn(double t) const
{
	draw();
	dx2::draw::SceneMove(dx2::palette::base::black, 1 - t);
}

void EditFunction::drawFadeOut(double t) const
{
	draw();
	dx2::draw::SceneMove(dx2::palette::base::white, t);
}

void EditFunction::drawUI() const
{
	font::MainFont()(EditFunctionTitle).draw(40, TitlePos, dx2::palette::base::black);

	getData().saveData.graph.Draw(&getData().saveData.camera);

	for (const auto& it : _buttons) it.second.draw();
}

std::map<EditFunction::ButtonKey, TextButton> EditFunction::_buttons =
{
	{
		ButtonKey::Decide,
		TextButton(RectF(Vec2(700, 260), SizeF(360, 100)), dx2::palette::base::red, U"完成!!")
	},
	{
		ButtonKey::Reset,
		TextButton(RectF(Vec2(700, 400), SizeF(360, 100)), dx2::palette::base::yellow, U"リセット")
	},
	{
		ButtonKey::Return,
		TextButton(RectF(Vec2(700, 540), SizeF(360, 100)), dx2::palette::base::green, U"もどる")
	}
};
