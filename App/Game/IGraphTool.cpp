#include "../stdafx.h"
#include "IGraphTool.h"

/* GraphGlideTool start ************************************************************************************/

GraphGlideTool::GraphGlideTool()
{
	m_screenVel = Vec2(0.0, 0.0);
	m_isZoomed = false;
	m_zoomRate = 1.0;
}

void GraphGlideTool::update(Graph* graph, GraphCamera* camera)
{
	slide(graph, camera);
	zoom(graph, camera);
}

void GraphGlideTool::draw() const
{
	// ここに描画処理を書く
	Rect(0, 0, 200, 200).draw(Palette::Skyblue);
}

void GraphGlideTool::slide(Graph* graph, GraphCamera* camera)
{
	m_screenVel *= m_decelerate;
	if (m_screenVel.length() < 0.01) m_screenVel = Vec2().Zero();
	m_isSliding = detectDoubleTap();
	if (m_isSliding && camera->ScreenBox().mouseOver()) {
		m_screenVel = Cursor::DeltaF();
	}
	if (m_screenVel.length() != Vec2().Zero().length()) {
		camera->slide(m_screenVel, graph->samples());
		graph->create(camera, [&](double x) { return dx2::math::EvalHoner(graph->cm(), x); });
	}
}

void GraphGlideTool::zoom(Graph* graph, GraphCamera* camera)
{
	m_zoomRate = calcZoomRate();
	m_isZoomed = (m_zoomRate != 1.0);
	if (camera->ScreenBox().mouseOver() && m_isZoomed) {
		if (m_zoomRate > 0.1 && m_zoomRate < 8.0) {
			camera->zoom(m_zoomRate, graph->samples());
			graph->create(camera, [&](double x) { return dx2::math::EvalHoner(graph->cm(), x); });
		}
	}
	m_isZoomed = false;
}

bool GraphGlideTool::detectDoubleTap()
{
	if (m_clickTimer.sF() < m_doubleTapThreshold) {
		if (MouseL.pressed()) {
			m_clickTimer.reset();
			m_isPressing = true;
		}
	}
	else {
		m_clickTimer.reset();
	}
	if (MouseL.down()) m_clickTimer.restart();
	if (MouseL.up()) m_isPressing = false;
	return m_isPressing;
}

/* GraphGlideTool end ************************************************************************************/


/* GraphZoomBoxTool start ************************************************************************************/

void GraphZoomBoxTool::update(Graph* graph, GraphCamera* camera)
{
}

void GraphZoomBoxTool::draw() const
{
}

/* GraphZoomBoxTool end ************************************************************************************/


/* GraphScopeTool start ************************************************************************************/

void GraphScopeTool::update(Graph* graph, GraphCamera* camera)
{
}

void GraphScopeTool::draw() const
{
}

/* GraphScopeTool end ************************************************************************************/


/* GraphRangeSelector start ************************************************************************************/

void GraphRangeSelector::update(Graph* graph, GraphCamera* camera)
{
}

void GraphRangeSelector::draw() const {}

/* GraphRangeSelector start ************************************************************************************/
