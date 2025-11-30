#pragma once
#include "../Utils/Common.h"

// 論理座標の成分範囲.
class LogicRange
{
public:
	constexpr LogicRange() = default;
	constexpr LogicRange(const Vec2& min, const Vec2& max)
		: min(min), max(max) {
	}
	constexpr ~LogicRange() = default;

	// 隣のサンプリング論理座標までの距離.
	constexpr Vec2 step(const SampleCount numSamples) const { return (max - min) / numSamples; }

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive) { archive(min, max); }

	Vec2 min;	// 最小値.
	Vec2 max;	// 最大値.
};
// デフォルトの描画範囲.
static constexpr LogicRange normalLR{ Vec2(-3.00, -3.00), Vec2(3.00, 3.00) };



// グラフを映すカメラクラス.
class GraphCamera
{
public:
	constexpr GraphCamera() = default;
	constexpr GraphCamera(const Vec2& logicCameraPos, const LogicRange& logicCameraRange, const Vec2& screenCameraPos, const SizeF& screenCameraSize)
		: _logicCameraPos(logicCameraPos), _logicCameraRange(logicCameraRange), _screenCameraPos(screenCameraPos), _screenCameraSize(screenCameraPos) {
	}
	constexpr ~GraphCamera() = default;

	// カメラのサイズ.
	constexpr RectF ScreenBox() const { return RectF(Arg::center(_screenCameraPos), _screenCameraSize); }

	// カメラの平行移動.
	void Slide(const Vec2& screenVel, const SampleCount numSamples);

	// カメラの倍率操作. カーソルの居る所へ拡大・縮小する.
	void Zoom(const double zoomRate, const SampleCount numSamples);

	// 論理座標から, カメラのスクリーン座標へと変換する.
	Vec2 ToScreenPos(const Vec2& logicPos, const SampleCount numSamples) const;

	// カメラのスクリーン座標から, 論理座標へと変換する.
	Vec2 ToLogicPos(const Vec2& screenPos, const SampleCount numSamples) const;

	// スクリーン画面での移動量を, 論理座標での移動量へと変換する.
	Vec2 ToLogicDelta(const Vec2& screenDelta, const SampleCount numSamples) const;

	// セッター.
	GraphCamera& SetScreenCameraPos(const Vec2& screenCameraPos) { _screenCameraPos = screenCameraPos; return *this; }
	GraphCamera& SetScreenCameraSize(const SizeF& screenCameraSize) { _screenCameraSize = screenCameraSize; return *this; }

	// ゲッター.
	const Vec2& Min() const { return _logicCameraRange.min; }
	const Vec2& Max() const { return _logicCameraRange.max; }
	const Vec2& step(const SampleCount numSamples) const { return _logicCameraRange.step(numSamples); }

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(_logicCameraPos, _logicCameraRange, _screenCameraPos, _screenCameraSize);
	}

private:
	Vec2 _logicCameraPos;			// カメラの今いる論理座標.
	LogicRange _logicCameraRange;	// カメラで切り取る論理座標の範囲.
	Vec2 _screenCameraPos;			// カメラの今いるスクリーン座標.
	SizeF _screenCameraSize;		// カメラの画面の大きさ.

	// y 成分だけ -1 倍する. スクリーンの座標が左上基準な為に, y 座標の増減が逆になっちゃう.
	constexpr Vec2 NegateY(const Vec2& v) const { return Vec2(v.x, -v.y); }

	// カメラの倍率.
	constexpr Vec2 Scale(const SampleCount numSamples) const { return Vec2(1.0, 1.0) / _logicCameraRange.step(numSamples); }

	// ズーム倍率の変更に合わせて, グラフの論理座標範囲を変更する.
	LogicRange ZoomLogicRange(const LogicRange& range, const double rate) const;
};


// グラフクラス.
class Graph
{
public:
	using Functioncurve = LineString;				// 曲線. 関数のあの「線」.

	constexpr Graph() = default;
	constexpr Graph(const SampleCount numSamples, const CoeffMatrix& cm)
		: m_numSamples(numSamples), m_cm(cm) {
	}
	constexpr ~Graph() = default;

	// 代入演算子.
	Graph& operator=(const Graph& other);

	// 生成する.
	void Create(const GraphCamera* camera, const Function func);

	// 描く.
	void Draw(const GraphCamera* camera) const;

	// 微分する.
	constexpr Graph& Differentiate() { m_cm = dx2::math::Differentiate(m_cm); return *this; }

	// 不定積分する.
	constexpr Graph& Integrate() { m_cm = dx2::math::Integrate(m_cm); return *this; }

	// セッター.
	Graph& SetnumSamples(const SampleCount numSamples) { m_numSamples = numSamples; return *this; }

	// ゲッター.
	const SampleCount numSamples() const { return m_numSamples; }
	const CoeffMatrix& cm() const { return m_cm; }
	const Functioncurve& curve() const { return m_curve; }

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive) { archive(m_numSamples, m_cm); }

private:
	SampleCount m_numSamples;	// サンプリング数.
	CoeffMatrix m_cm;			// 係数行列.
	Functioncurve m_curve;		// 曲線.
	std::vector<Vec2> m_values;	// サンプリング値.

	// サンプリング値を曲線に変換する.
	Functioncurve ToFunctioncurve(const GraphCamera* camera, std::vector<Vec2> const& values, const SampleCount numSamples) const;
};
