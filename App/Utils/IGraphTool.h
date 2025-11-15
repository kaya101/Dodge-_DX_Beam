#pragma once
#define _WIN32_WINNT 0x0601	// Windows 7 以降の API を使う.
#include <Windows.h>		// API 用.
#include "../Game/Graph.h"

// グラフの編集操作のインターフェース.
class IGraphTool
{
public:
	virtual ~IGraphTool() = default;
	virtual void update(Graph* graph, GraphCamera* camera) = 0;
	virtual void draw() const = 0;
};

// 掴んで滑らかに移動（パン操作）.
static WNDPROC g_originalWndProc = nullptr;
class GraphGlideTool : public IGraphTool
{
public:
	GraphGlideTool();
	~GraphGlideTool() = default;
	void update(Graph* graph, GraphCamera* camera) override;
	void draw() const override;

private:
	Vec2 _screenVel;									// スクリーン画面の移動速度.
	bool _isZoomed;										// Zoom操作をしているか.
	double _zoomRate;									// カメラの倍率.
	static constexpr double _decelerate = 0.95;			// 減速.
	static constexpr double _WheelZoomDivisor = 1200.0;	// ホイールズームの感度.
	static GraphGlideTool* s_instance;					// 自分自身を指すポインタ.

	// 平行移動.
	void Slide(Graph* graph, GraphCamera* camera);

	// 倍率操作.
	void Zoom(Graph* graph, GraphCamera* camera);

	// 自分の WndProc.
	static LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ウィンドウプロシージャの差し替え.
	void HookWndProc();
};


// 矩形で切り取って拡大（ボックスズーム）.
class GraphZoomBoxTool : public IGraphTool
{
public:
	GraphZoomBoxTool() = default;
	~GraphZoomBoxTool() = default;
	void update(Graph* graph, GraphCamera* camera) override;
	void draw() const override;
};

// 指定倍率で拡大・縮小（スコープ／ズーム）.
class GraphScopeTool : public IGraphTool
{
public:
	GraphScopeTool() = default;
	~GraphScopeTool() = default;
	void update(Graph* graph, GraphCamera* camera) override;
	void draw() const override;
};

// x, y の範囲を直接指定（範囲選択）.
class GraphRangeSelector : public IGraphTool
{
public:
	GraphRangeSelector() = default;
	~GraphRangeSelector() = default;
	void update(Graph* graph, GraphCamera* camera) override;
	void draw() const override;

private:
	LogicRange _requestedLogicRange;// 変更先の論理座標の範囲.
};
