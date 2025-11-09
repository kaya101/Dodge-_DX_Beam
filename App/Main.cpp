// dx ビームを避けるゲーム.

# include <Siv3D.hpp> // Siv3D v0.6.16
#include "AppIncludes.h"

namespace font = dx2::font;

void Main()
{
	// プレイ画面の大きさを定数で設定する.
	Window::Resize(windowWidth, windowHeight);

	// ウィンドウの枠を非表示にする.
	Window::SetStyle(WindowStyle::Frameless);// エスケープキー（ESC）でゲームを終了できるから、右上のバツが押せなくても平気.

	// bgm を設定する.
	for (const auto& it : dx2::music::bgm) AudioAsset::Register(it.first, Audio::Stream, it.second);

	// 効果音を設定する.
	for (const auto& it : dx2::music::se) AudioAsset::Register(it.first, std::get<0>(it.second), std::get<1>(it.second), std::get<2>(it.second));

	// フォントを設定する.
	FontAsset::Register(font::FontName(font::FontKey::Title), FontMethod::MSDF, 50, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(font::FontName(font::FontKey::Title)).setBufferThickness(4);
	FontAsset::Register(font::FontName(font::FontKey::Main), FontMethod::SDF, 30, Typeface::Bold);
	FontAsset(font::FontName(font::FontKey::Main)).setBufferThickness(4);

	App manager;
	manager.add<Title>(State::Title);
	manager.add<Play>(State::Play);
	manager.add<SelectFiles>(State::SelectFiles);
	manager.add<EditFunction>(State::EditFunction);

	//manager.init(State::SelectFiles, 0s);// Title シーンから始めて, 0s 後に開始.
	manager.init(State::Title, 0s);

	// ゲームシーンから開始したい場合はこのコメントを外す.
	//manager.init(State::Play, 0s);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}


//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
