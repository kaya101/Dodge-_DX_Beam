#pragma once
#include "../Utils/Common.h"
#include "../Utils/BaseButton.h"
#include "../Game/Graph.h"
#include "../Utils/IGraphTool.h"

// 関数を編集するシーン.
class EditFunction : public App::Scene
{
public:
	EditFunction(const InitData& init);
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
	void drawUI() const;

private:
	// 見出し.
	const String EditFunctionTitle = U"編集画面";
	static constexpr Vec2 TitlePos = Vec2(20, 15);

	// グラフ編集ツールのポインタ.
	std::unique_ptr<IGraphTool> pGraphTool = std::make_unique<GraphGlideTool>();

	// カメラ.
	//std::unique_ptr<GraphCamera> pCamera = std::make_unique<GraphCamera>(getData().saveData.camera);

	// グラフ.
	std::unique_ptr<Graph> pGraph = std::make_unique<Graph>(getData().numSamples, getData().coeffMatrix);

	// 決定, リセット, 戻る, ボタン.
	enum class ButtonKey : int8_t { Decide, Reset, Return };
	static std::map<ButtonKey, TextButton> _buttons;
};
