#pragma once
#include "../Utils/Utils.h"

using SampleSize = std::size_t;					// サンプリング数.
using CoeffMatrix = std::vector<double>;		// 係数行列.
using Function = std::function<double(double)>;	// f(x).
using LogicalPoints = std::vector<Vec2>;		// 論理座標上の点列.
using FunctionCurve = LineString;				// 曲線. 関数のあの「線」.

// デフォルトのサンプル数.
//static constexpr SampleSize N = 640;

// 固定画面範囲.
//static constexpr RectF ScreenRect = { Arg::center(windowWidth * 0.5, windowHeight * 0.5), N, N };

// 論理座標の成分範囲.
class LogicRange
{
public:
	explicit constexpr LogicRange() = default;
	explicit constexpr LogicRange(const Vec2& min, const Vec2& max)
		: m_min(min), m_max(max) {
	}
	constexpr ~LogicRange() = default;

	// 隣のサンプリング論理座標までの距離.
	constexpr Vec2 step(const SampleSize samples) const { return (m_max - m_min) / samples; }

	// 平行移動.
	void slideMin(const Vec2& logicDelta) { m_min += logicDelta; }
	void slideMax(const Vec2& logicDelta) { m_max += logicDelta; }

	// ゲッター.
	const Vec2& min() const { return m_min; }
	const Vec2& max() const { return m_max; }

	// セッター.
	void setMin(const Vec2& min) { m_min = min; }
	void setMax(const Vec2& max) { m_max = max; }

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive) { archive(m_min, m_max); }

private:
	Vec2 m_min;	// 最小値.
	Vec2 m_max;	// 最大値.
};
// デフォルトの描画範囲.
static constexpr LogicRange normalLR{ Vec2(-3.00, -3.00), Vec2(3.00, 3.00) };



// グラフを映すカメラクラス.
class GraphCamera
{
public:
	explicit GraphCamera(const Vec2& screenCenter, const SizeF& screenBoxSize);
	~GraphCamera() = default;

	// カメラのサイズ.
	constexpr RectF ScreenBox() const { return RectF(Arg::center(m_screenCameraCenter), m_screenCameraSize); }

	// カメラの平行移動.
	void slide(const Vec2& screenVel, const SampleSize samples);

	// カメラの倍率操作. カーソルの居る所へ拡大・縮小する.
	void zoom(const double zoomRate, const SampleSize samples);

	// 論理座標から, カメラのスクリーン座標へと変換する.
	Vec2 toScreenPos(const Vec2& logicPos, const SampleSize samples) const;

	// カメラのスクリーン座標から, 論理座標へと変換する.
	Vec2 toLogicPos(const Vec2& screenCenter, const SampleSize samples) const;

	// スクリーン画面での移動量を, 論理座標での移動量へと変換する.
	Vec2 toLogicDelta(const Vec2& screenDelta, const SampleSize samples) const;

	// 平行移動.
	void slideLogicCameraCenter(const Vec2& logicDelta) { m_logicCameraCenter += logicDelta; }

	// ゲッター.
	const Vec2& logicMin() const { return m_logicCameraRange.min(); }
	const Vec2& logicMax() const { return m_logicCameraRange.max(); }
	const Vec2& logicStep(const SampleSize samples) const { return m_logicCameraRange.step(samples); }

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(m_logicCameraCenter, m_screenCameraCenter, m_screenCameraSize, m_logicCameraRange);
	}

private:
	Vec2 m_logicCameraCenter;			// カメラの今いる論理座標.
	Vec2 m_screenCameraCenter;			// カメラの今いるスクリーン座標.
	LogicRange m_logicCameraRange;	// カメラで切り取る論理座標の範囲.
	SizeF m_screenCameraSize;		// カメラの画面の大きさ.

	// y 成分だけ -1 倍する. スクリーンの座標が左上基準な為に, y 座標の増減が逆になっちゃう.
	constexpr Vec2 negateY(const Vec2& v) const { return Vec2(v.x, -v.y); }

	// カメラの倍率.
	constexpr Vec2 scale(const SampleSize samples) const { return Vec2(1.0, 1.0) / m_logicCameraRange.step(samples); }

	// ズーム倍率の変更に合わせて, グラフの論理座標範囲を変更する.
	LogicRange zoomLogicRange(const LogicRange& logicRange, const double rate) const;
};


// グラフクラス.
class Graph
{
public:
	explicit Graph(const SampleSize n = N, const CoeffMatrix& cm = dx2::math::DefaultCoeffMatrix)
		: m_samples(n), m_cm(cm) {
	}
	~Graph() = default;

	// 代入演算子.
	Graph& operator=(const Graph& other);

	// 生成する.
	void create(const GraphCamera* camera, const Function func);

	// 描く.
	void draw(const GraphCamera* camera) const;

	// 微分する.
	constexpr void Differentiate() { m_cm = dx2::math::Differentiate(m_cm); }

	// 不定積分する.
	constexpr void Integrate() { m_cm = dx2::math::Integrate(m_cm); }

	// ゲッター.
	const SampleSize samples() const { return m_samples; }
	const CoeffMatrix& cm() const { return m_cm; }
	const FunctionCurve& curve() const { return m_curve; }

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive) { archive(m_samples, m_cm); }

private:
	SampleSize m_samples;	// サンプリング数.
	CoeffMatrix m_cm;			// 係数行列.
	FunctionCurve m_curve;		// 曲線.
	LogicalPoints m_values;	// サンプリング値.

	// サンプリング値を曲線に変換する.
	FunctionCurve toFunctionCurve(const GraphCamera* camera, LogicalPoints const& values, const SampleSize samples) const;
};
