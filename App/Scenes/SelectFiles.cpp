#include "../stdafx.h"
#include "SelectFiles.h"
#include "../Game/SaveDataManager.h"

namespace font = dx2::font;

SelectFiles::SelectFiles(const InitData& init)
	: IScene(init)
{
	font::MainFont().preload(SelectFilesTitle);

	// セーブデータの親ディレクトリが無かった時.
	if (!FileSystem::IsDirectory(SaveDataManager().RootDirName()))
	{
		// 初期セーブデータを 3 ファイル分だけ生成する.
		for (size_t i = 0; i < static_cast<size_t>(SaveDataManager().SubDirNames().size()); ++i)
		{
			SaveDataManager().Store(defaultGameData);
		}
	}
}

void SelectFiles::update()
{
	// ファイルボタン.
	for (const auto& it : SaveDataManager().SubDirNames())
	{
		if (_fileButtons.at(it).IsReleased())
		{
			Print << U"{}が選択されました。"_fmt(it);
			getData().saveData.saveDataSubDirName = it;

			// 子ディレクトリの名前からそのデータを読み込む.
			SaveDataManager().Load(getData());
			getData().saveData.saveDataSubDirName = it;

			// グラフを生成する.
			getData().Adjust(GraphOfFile);
			getData().saveData.graph.Create(&getData().saveData.camera,
				[&](const double x)
				{
					return dx2::math::EvalHoner(getData().saveData.graph.CM(), x);
				}
			);
		}
	}

	// 決定ボタン.
	if (_buttons.at(ButtonKey::Decide).IsReleased())
	{
		// セーブデータを選択していなかった時.
		if (getData().saveData.saveDataSubDirName == U"")
		{
			Print << U"セーブデータを選択してください。";
			return;
		}
		// カメラとグラフを調整する.
		getData().Adjust(ScreenRect);
		changeScene(State::Play, 1.0s);
	}

	// 編集ボタン.
	if (_buttons.at(ButtonKey::Edit).IsReleased())
	{
		// セーブデータを選択していなかった時.
		if (getData().saveData.saveDataSubDirName == U"")
		{
			Print << U"セーブデータを選択してください。";
			return;
		}
		getData().Adjust(ScreenRect);
		changeScene(State::EditFunction, 1.0s);
	}
}

void SelectFiles::draw() const
{
	Scene::SetBackground(dx2::palette::base::blue);
	drawUI();
}

void SelectFiles::drawFadeIn(double t) const
{
	draw();
	dx2::draw::SceneMove(dx2::palette::base::black, 1 - t);
}

void SelectFiles::drawFadeOut(double t) const
{
	draw();
	dx2::draw::SceneMove(dx2::palette::base::white, t);
}

void SelectFiles::drawUI() const
{
	font::MainFont()(SelectFilesTitle).draw(40, TitlePos, dx2::palette::base::black);

	for (const auto& it : _fileButtons) it.second.Draw();

	for (const auto& it : _buttons) it.second.Draw();

	GraphOfFile.drawFrame(5, dx2::palette::base::black);
	if (getData().saveData.saveDataSubDirName != U"")
	{
		getData().saveData.graph.Draw(&getData().saveData.camera);
	}
}

std::map<String, TextButton> SelectFiles::_fileButtons =
{
	{
		SaveDataManager().SubDirNames()[0],
		TextButton(RectF(Vec2(700, 160), SizeF(300, 80)), dx2::palette::base::white, U"ファイル１")
	},
	{
		SaveDataManager().SubDirNames()[1],
		TextButton(RectF(Vec2(700, 280), SizeF(300, 80)), dx2::palette::base::white, U"ファイル２")
	},
	{
		SaveDataManager().SubDirNames()[2],
		TextButton(RectF(Vec2(700, 400), SizeF(300, 80)), dx2::palette::base::white, U"ファイル３")
	}
};

std::map<SelectFiles::ButtonKey, TextButton> SelectFiles::_buttons =
{
	{
		ButtonKey::Decide,
		TextButton(RectF(Vec2(700, 540), SizeF(360, 100)), dx2::palette::base::red, U"あそぶ")
	},
	{
		ButtonKey::Edit,
		TextButton(RectF(Vec2(1100, 540), SizeF(100, 100)), dx2::palette::base::paple, U"編集")
	}
};
