#pragma once
#include "Player.h"// 前方宣言だけではポインタでやりくりできないらしい.
#include "Graph.h"

// ゲームシーンでまとめて渡すもの.
struct UpdateContext
{
	GraphCamera& pCamera;
	Graph& pGraph;
	Player& pPlayer;
};

// player, Graph に影響を及ぼすオブジェクト基本クラス.
class EffectObject
{
public:
	using EasingFunc = std::function<double(double)>;// イージング関数.

	EffectObject(const String& name, const Size& bodySize, const double rounded, const ColorF& bodyColor, const EasingFunc easing);
	~EffectObject() = default;
	void Update(UpdateContext& context);
	void Draw() const;
	bool CanAffect() const { return _active && not _consumed; }
	bool IsConsumed() const { return _consumed; }
	virtual void ApplyEffect(UpdateContext& context) = 0;

	static constexpr SecondsF timeLimit = 1.6s;// 出現時間.

protected:
	String _name;			// アイテム名.
	ColorF _textColor;		// テキストの色.
	s3d::RoundRect _body;	// 当たり判定.
	ColorF _bodyColor;		// 本体の色.
	Vec2 _spawnPos;			// 出現点.
	Vec2 _vanishPos;		// 消滅点.
	s3d::Timer _timer;		// 移動のタイマー.
	bool _active;			// 効果を発動できるかフラグ.
	bool _consumed;			// 削除フラグ.
	EasingFunc _easing;		// イージング関数.

private:
	Vec2 SetPoint() const;
};




static constexpr Size BodySize() { return Size(12 + rand() % 160, 12 + rand() % 160); }

// 左右に揺れる dx クラス.
class Shake : public EffectObject
{
public:
	Shake();
	~Shake() = default;
	void ApplyEffect(UpdateContext& context) override;

private:
	static constexpr ColorF shakeBodyColor = dx2::palette::base::orange;
	EasingFunc _shakeEasing = dx2::easing::Simple;
};


// 狙い撃ちする dx クラス.
class Shot : public EffectObject
{
public:
	Shot();
	~Shot() = default;
	void Draw() const;
	void ApplyEffect(UpdateContext& context) override;

private:
	static constexpr ColorF shotBodyColor = dx2::palette::base::lime;
	EasingFunc _shotEasing = dx2::easing::ChargeShot;
};







enum class ItemType : int8_t
{
	Exp,
	Smaller,
	Bigger,
	Flipper,
	Integraler
};

static constexpr int32_t size = 60;

// exp アイテム.
class ExpItem : public EffectObject
{
public:
	ExpItem();
	~ExpItem() = default;
	void ApplyEffect(UpdateContext& context) override;

private:
	static constexpr ColorF expItemBodyColor = dx2::palette::pastel::blue;
	EasingFunc _expItemEasing = dx2::easing::Simple;
};


// player を小さくするアイテム.
class Smaller : public EffectObject
{
public:
	Smaller();
	~Smaller() = default;
	void ApplyEffect(UpdateContext& context) override;

private:
	static constexpr ColorF smallerBodyColor = dx2::palette::pastel::paple;
	EasingFunc _smallerEasing = dx2::easing::Simple;
};


// player を大きくするアイテム.
class Bigger : public EffectObject
{
public:
	Bigger();
	~Bigger() = default;
	void ApplyEffect(UpdateContext& context) override;

private:
	static constexpr ColorF biggerBodyColor = dx2::palette::base::yellow;
	EasingFunc _biggerEasing = dx2::easing::Simple;
};


// グラフを x 軸反転させるアイテム.
class Flipper : public EffectObject
{
public:
	Flipper();
	~Flipper() = default;
	void ApplyEffect(UpdateContext& context) override;

private:
	static constexpr ColorF flipperBodyColor = dx2::palette::base::black;
	EasingFunc _flipperEasing = dx2::easing::Simple;
};


// 不定積分をするアイテム.
class Integraler : public EffectObject
{
public:
	Integraler();
	~Integraler() = default;
	void ApplyEffect(UpdateContext& context) override;

private:
	static constexpr ColorF integralerBodyColor = dx2::palette::base::deepBlue;
	EasingFunc _integralerEasing = dx2::easing::Simple;
};
