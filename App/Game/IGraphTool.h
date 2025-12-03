#pragma once
#include "Graph.h"

// グラフの編集操作のインターフェース.
class IGraphTool
{
public:
	virtual void update(Graph* graph, GraphCamera* camera) = 0;
	virtual void draw() const = 0;
};

// 掴んで滑らかに移動（パン操作）.
class GraphGlideTool : public IGraphTool
{
public:
	explicit GraphGlideTool();
	~GraphGlideTool() = default;
	void update(Graph* graph, GraphCamera* camera) override;
	void draw() const override;

private:
	Vec2 m_screenVel;									// スクリーン画面の移動速度.

	static constexpr double m_decelerate = 0.98;			// 減速.

	// 平行移動.
	bool m_isSliding = false;
	static constexpr double m_doubleTapThreshold = 0.3; // ダブルタップ閾値（秒）.
	void slide(Graph* graph, GraphCamera* camera);

	// ダブルタップを検知する関数.
	bool m_isPressing = false;
	Stopwatch m_clickTimer{ StartImmediately::Yes };
	bool detectDoubleTap();

	// 倍率操作.
	bool m_isZoomed;								// Zoom操作をしているか.
	double m_zoomRate;								// カメラの倍率.
	void zoom(Graph* graph, GraphCamera* camera);	// そのメソッド.

	// 倍率計算.
	const double calcZoomRate() const { return 1.0 - (Mouse::Wheel() / 10); }
};


// 矩形で切り取って拡大（ボックスズーム）.
class GraphZoomBoxTool : public IGraphTool
{
public:
	explicit GraphZoomBoxTool() = default;
	~GraphZoomBoxTool() = default;
	void update(Graph* graph, GraphCamera* camera) override;
	void draw() const override;
};

// 指定倍率で拡大・縮小（スコープ／ズーム）.
class GraphScopeTool : public IGraphTool
{
public:
	explicit GraphScopeTool() = default;
	~GraphScopeTool() = default;
	void update(Graph* graph, GraphCamera* camera) override;
	void draw() const override;
};

// x, y の範囲を直接指定（範囲選択）.
class GraphRangeSelector : public IGraphTool
{
public:
	explicit GraphRangeSelector() = default;
	~GraphRangeSelector() = default;
	void update(Graph* graph, GraphCamera* camera) override;
	void draw() const override;

private:
	LogicRange _requestedLogicRange;// 変更先の論理座標の範囲.
};
