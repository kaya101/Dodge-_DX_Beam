#include "../stdafx.h"
#include "IGraphTool.h"

/* GraphGlideTool start ************************************************************************************/

GraphGlideTool* GraphGlideTool::s_instance = nullptr;

GraphGlideTool::GraphGlideTool()
{
	s_instance = this;
	HookWndProc();
	_screenVel = Vec2(0.0, 0.0);
	_isZoomed = false;
	_zoomRate = 1.0;
}

void GraphGlideTool::Update(Graph* graph, GraphCamera* camera)
{
	Slide(graph, camera);
	Zoom(graph, camera);
}

void GraphGlideTool::Draw() const {}

void GraphGlideTool::Slide(Graph* graph, GraphCamera* camera)
{
	_screenVel *= _decelerate;
	if (_screenVel.length() < 0.01) _screenVel = Vec2(0.0, 0.0);

	if (camera->ScreenBox().mouseOver() && MouseL.pressed())
	{
		_screenVel = Cursor::DeltaF();
	}

	if (_screenVel.length() != 0.0)
	{
		camera->Slide(_screenVel, graph->NumSamples());
		graph->Create(camera, [&](double x) { return dx2::math::EvalHoner(graph->CM(), x); });
	}
}

void GraphGlideTool::Zoom(Graph* graph, GraphCamera* camera)
{
	if (camera->ScreenBox().mouseOver() && _isZoomed)
	{
		if (_zoomRate > 0.1 && _zoomRate < 8.0)
		{
			camera->Zoom(_zoomRate, graph->NumSamples());
			graph->Create(camera, [&](double x) { return dx2::math::EvalHoner(graph->CM(), x); });
		}
	}
	_zoomRate = false;
}

// 自分のWndProc. 二本指で拡大・縮小する入力を受け取る.
LRESULT CALLBACK GraphGlideTool::MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ズームジェスチャーを有効化.
	if (msg == WM_GESTURENOTIFY)
	{
		GESTURECONFIG gc = {};
		gc.dwID = GID_ZOOM; // ズーム.
		gc.dwWant = GC_ZOOM;// 欲しいジェスチャー.
		gc.dwBlock = 0;		// ブロックしない.
		SetGestureConfig(hwnd, 0, 1, &gc, sizeof(gc));
		return 0;
	}

	if (msg == WM_MOUSEWHEEL)
	{
		// ホイールの回転量（120 単位）.
		short delta = GET_WHEEL_DELTA_WPARAM(wParam);

		// Ctrlキーが押されているか.
		bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;

		if (ctrl && s_instance) // Ctrl + ホイールならズーム.
		{
			// 1 ステップあたりの倍率（調整可）.
			double rate = 1.0 + (delta / _WheelZoomDivisor);
			s_instance->_isZoomed = true;
			s_instance->_zoomRate = rate;
		}
		return 0;
	}

	// 他のメッセージは元のWndProcへ.
	return CallWindowProc(g_originalWndProc, hwnd, msg, wParam, lParam);
}

void GraphGlideTool::HookWndProc()
{
	HWND hwnd = static_cast<HWND>(s3d::Platform::Windows::Window::GetHWND());
	g_originalWndProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
	SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MyWndProc);
}

/* GraphGlideTool end ************************************************************************************/


/* GraphZoomBoxTool start ************************************************************************************/

void GraphZoomBoxTool::Update(Graph* graph, GraphCamera* camera)
{
}

void GraphZoomBoxTool::Draw() const
{
}

/* GraphZoomBoxTool end ************************************************************************************/


/* GraphScopeTool start ************************************************************************************/

void GraphScopeTool::Update(Graph* graph, GraphCamera* camera)
{
}

void GraphScopeTool::Draw() const
{
}

/* GraphScopeTool end ************************************************************************************/


/* GraphRangeSelector start ************************************************************************************/

void GraphRangeSelector::Update(Graph* graph, GraphCamera* camera)
{
}

void GraphRangeSelector::Draw() const {}

/* GraphRangeSelector start ************************************************************************************/
