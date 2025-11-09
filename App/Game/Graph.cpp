#include "../stdafx.h"
#include "Graph.h"


/*		GraphCamera start		*************************************************************************************************/

void GraphCamera::Slide(const Vec2& screenVel, const SampleCount numSamples)
{
	Vec2 logicDelta = ToLogicDelta(screenVel, numSamples);
	_logicCameraPos -= logicDelta;
	_logicCameraRange.min -= logicDelta;
	_logicCameraRange.max -= logicDelta;
}

void GraphCamera::Zoom(const double zoomRate, const SampleCount numSamples)
{
	//Print << zoomRate;// 拡大するときに > 1.0 となる.
	const Vec2 screenFocusPos = Cursor::PosF();							// カーソルのスクリーン座標を取得.
	const Vec2 logicBeforePos = ToLogicPos(screenFocusPos, numSamples);	// 変更前のカーソルの論理座標を取得.
	_logicCameraRange = ZoomLogicRange(_logicCameraRange, zoomRate);	// 全体を拡大・縮小する.
	const Vec2 logicAfterPos = ToLogicPos(screenFocusPos, numSamples);	// 変更後のカーソルの論理座標を取得.
	const Vec2 logicDelta = logicAfterPos - logicBeforePos;				// 論理座標でのカーソルの移動量を取得.
	_logicCameraPos -= logicDelta;										// カーソルの移動量だけ平行移動.
	_logicCameraRange.min -= logicDelta;								// 上と同じ.
	_logicCameraRange.max -= logicDelta;								// 上と同じ.
}

Vec2 GraphCamera::ToScreenPos(const Vec2& logicPos, const SampleCount numSamples) const
{
	return _screenCameraPos + NegateY(logicPos - _logicCameraPos) * Scale(numSamples);
}

Vec2 GraphCamera::ToLogicPos(const Vec2& screenPos, const SampleCount numSamples) const
{
	return _logicCameraPos + NegateY(screenPos - _screenCameraPos) / Scale(numSamples);
}

Vec2 GraphCamera::ToLogicDelta(const Vec2& screenDelta, const SampleCount numSamples) const
{
	return NegateY(screenDelta) / Scale(numSamples);
}

LogicRange GraphCamera::ZoomLogicRange(const LogicRange& logicRange, const double rate) const
{
	// 全体を均一に拡大・縮小する.
	LogicRange lr{};
	const Vec2 midpoint = logicRange.max + logicRange.min;
	const Vec2 range = logicRange.max - logicRange.min;
	lr.min = (midpoint - (range / rate)) / 2;
	lr.max = (midpoint + (range / rate)) / 2;
	return lr;
}

/*		GraphCamera end			*************************************************************************************************/


/*		Graph start		*********************************************************************************************************/

Graph& Graph::operator=(const Graph& other)
{
	if (this != &other) {
		_numSamples = other._numSamples;
		_cm = other._cm;
	}
	return *this;
}

void Graph::Create(const GraphCamera* camera, const Function func)
{
	_values.clear();
	_values.resize(_numSamples + 1);
	for (size_t i = 0; i < static_cast<int32_t>(_values.size()); ++i)
	{
		_values[i].x = (camera->Min().x + camera->Step(_numSamples).x * i);
		_values[i].y = func(_values[i].x);
	}
	_curve = ToFunctionCurve(camera, _values, _numSamples);
}

void Graph::Draw(const GraphCamera* camera) const
{
	camera->ScreenBox().draw(dx2::palette::base::white);
	_curve.draw(6, dx2::palette::base::black);
}

Graph::FunctionCurve Graph::ToFunctionCurve(const GraphCamera* camera, std::vector<Vec2> const& values, const SampleCount numSamples) const
{
	FunctionCurve ls(static_cast<int32_t>(values.size()));
	for (size_t i = 0; i < static_cast<int32_t>(ls.size()); ++i)
	{
		// 論理座標で範囲外の物を生成しない.
		if (values[i].y < camera->Min().y || values[i].y > camera->Max().y)
		{
			ls[i] = Vec2(0.0, 0.0);// 範囲外に設定.
		}
		else
		{
			ls[i] = camera->ToScreenPos(values[i], numSamples);
		}
	}
	ls.remove_if([](Vec2 v) { return v.length() == 0.0; });// その範囲外を削除.
	return ls;
}

/*		Graph end		*********************************************************************************************************/
