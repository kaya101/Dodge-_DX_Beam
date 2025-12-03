#include "../stdafx.h"
#include "Graph.h"


/*		GraphCamera start		*************************************************************************************************/

GraphCamera::GraphCamera(const Vec2& screenCenter, const SizeF& screenSize)
	: m_screenCameraCenter(screenCenter), m_screenCameraSize(screenSize)
{
	m_logicCameraCenter = Vec2(0.0, 0.0);
	m_logicCameraRange = normalLR;
}

void GraphCamera::slide(const Vec2& screenVel, const SampleSize samples)
{
	Vec2 logicDelta = toLogicDelta(screenVel, samples);
	slideLogicCameraCenter(-logicDelta);
	m_logicCameraRange.slideMin(-logicDelta);
	m_logicCameraRange.slideMax(-logicDelta);
}

void GraphCamera::zoom(const double zoomRate, const SampleSize samples)
{
	//Print << zoomRate;// 拡大するときに > 1.0 となる.
	const Vec2 screenFocusPos = Cursor::PosF();							// カーソルのスクリーン座標を取得.
	const Vec2 logicBeforePos = toLogicPos(screenFocusPos, samples);	// 変更前のカーソルの論理座標を取得.
	m_logicCameraRange = zoomLogicRange(m_logicCameraRange, zoomRate);	// 全体を拡大・縮小する.
	const Vec2 logicAfterPos = toLogicPos(screenFocusPos, samples);		// 変更後のカーソルの論理座標を取得.
	const Vec2 logicDelta = logicAfterPos - logicBeforePos;				// 論理座標でのカーソルの移動量を取得.
	slideLogicCameraCenter(-logicDelta);								// カーソルの移動量だけ平行移動.
	m_logicCameraRange.slideMin(-logicDelta);							// 上と同じ.
	m_logicCameraRange.slideMax(-logicDelta);							// 上と同じ.
}

Vec2 GraphCamera::toScreenPos(const Vec2& logicPos, const SampleSize samples) const
{
	return m_screenCameraCenter + negateY(logicPos - m_logicCameraCenter) * scale(samples);
}

Vec2 GraphCamera::toLogicPos(const Vec2& screenCenter, const SampleSize samples) const
{
	return m_logicCameraCenter + negateY(screenCenter - m_screenCameraCenter) / scale(samples);
}

Vec2 GraphCamera::toLogicDelta(const Vec2& screenDelta, const SampleSize samples) const
{
	return negateY(screenDelta) / scale(samples);
}

LogicRange GraphCamera::zoomLogicRange(const LogicRange& logicRange, const double rate) const
{
	// 全体を均一に拡大・縮小する.
	LogicRange lr{};
	const Vec2 midpoint = logicRange.max() + logicRange.min();
	const Vec2 range = logicRange.max() - logicRange.min();
	lr.setMin((midpoint - (range / rate)) / 2);
	lr.setMax((midpoint + (range / rate)) / 2);
	return lr;
}

/*		GraphCamera end			*************************************************************************************************/


/*		Graph start		*********************************************************************************************************/

//Graph::Graph()
//{
//	// デフォルトの設定値.
//	m_samples = N;
//	m_cm = dx2::math::DefaultCM;
//}

Graph& Graph::operator=(const Graph& other)
{
	if (this != &other) {
		m_samples = other.m_samples;
		m_cm = other.m_cm;
	}
	return *this;
}

void Graph::create(const GraphCamera* camera, const Function func)
{
	m_values.clear();
	m_values.resize(m_samples + 1);
	for (size_t i = 0; i < static_cast<int32_t>(m_values.size()); ++i) {
		m_values[i].x = (camera->logicMin().x + camera->logicStep(m_samples).x * i);
		m_values[i].y = func(m_values[i].x);
	}
	m_curve = toFunctionCurve(camera, m_values, m_samples);
}

void Graph::draw(const GraphCamera* camera) const
{
	camera->ScreenBox().draw(dx2::palette::base::white);
	m_curve.draw(6, dx2::palette::base::black);
}

FunctionCurve Graph::toFunctionCurve(const GraphCamera* camera, LogicalPoints const& values, const SampleSize samples) const
{
	FunctionCurve ls(static_cast<int32_t>(values.size()));
	for (size_t i = 0; i < static_cast<int32_t>(ls.size()); ++i) {
		// 論理座標で範囲外の物を生成しない.
		if (values[i].y < camera->logicMin().y || values[i].y > camera->logicMax().y) {
			ls[i] = Vec2(0.0, 0.0);// 範囲外に設定.
		}
		else {
			ls[i] = camera->toScreenPos(values[i], samples);
		}
	}
	ls.remove_if([](Vec2 v) { return v.length() == 0.0; });// その範囲外を削除.
	return ls;
}

/*		Graph end		*********************************************************************************************************/
