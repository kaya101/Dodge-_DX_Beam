#pragma once
#include "../Utils/Common.h"
#include "../Utils/BaseButton.h"

// ファイル選択シーン.
class SelectFiles : public App::Scene
{
public:
	explicit SelectFiles(const InitData& init);
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
	void drawUI() const;
	
private:
	// ファイルボタン.
	static std::map<String, TextButton> _fileButtons;// <セーブデータの名前, ボタン>.

	// 見出し.
	const String SelectFilesTitle = U"ファイルセレクト";
	static constexpr Vec2 TitlePos = Vec2(20, 15);

	// あそぶ, 編集, 左ページへ進む, 右ページへ進むボタン.
	enum class ButtonKey : int8_t { Decide, Edit, File1, File2, File3 };
	static std::map<ButtonKey, TextButton> _buttons;

	// そのファイルの関数グラフ.
	static constexpr RectF GraphOfFile = RectF(50, 100, 540, 540);
};
