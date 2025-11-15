#pragma once
#include "../Utils/Common.h"
#include "Player.h"// 前方宣言だけではポインタでやりくりできないらしい.
#include "Graph.h"

// ゲームシーンでまとめて渡すもの.
struct UpdateContext
{
	//GraphCamera& pCamera;
	Graph& pGraph;
	Player& pPlayer;
};

// player, Graph に影響を及ぼすオブジェクト 抽象クラス.
class EffectObject
{
public:
	using PositionUpdater = std::function<Vec2(Vec2)>;// 移動関数.

	// 名前を値渡しに変更（内部で move して保持）
	EffectObject(String name, const ColorF& color, const Vec2& pos, const SizeF& size, const Vec2& velocity, const PositionUpdater positionUpdater);
	~EffectObject() = default;
	virtual void update(UpdateContext& context) = 0;
	virtual void draw() const = 0;
	virtual void applyEffect(UpdateContext& context) = 0;
	bool isConsumed() const { return m_consumed; }
	bool canAffect() const { return m_active && not m_consumed; }

protected:
	String m_name;						// アイテム名.
	ColorF m_color;						// 本体の色.
	Body m_body;						// 当たり判定.
	Vec2 m_velocity;					// 速度.
	Vec2 m_spawnPos;					// 出現点.
	PositionUpdater m_positionUpdater;	// 動きの関数.
	bool m_active;						// 効果を発動できるかフラグ.
	bool m_consumed;					// 削除フラグ.
};

static constexpr int32_t size = 60;

static const Vec2 setSpawnPoint()
{
	Vec2 pos{};
	// 上下.
	if (rand() % 2 == 0) {
		pos.x = size * 0.52 + ScreenRect.pos.x + rand() % static_cast<int32_t>(ScreenRect.w + 1 - size);
		pos.y = (rand() % 2 == 0) ? ScreenRect.y + size * 0.52 : ScreenRect.bottomCenter().y - size * 0.52;
	}

	// 左右.
	else {
		pos.x = (rand() % 2 == 0) ? ScreenRect.x + size * 0.52 : ScreenRect.rightCenter().x - size * 0.52;
		pos.y = ScreenRect.pos.y + size * 0.52 + rand() % static_cast<int32_t>(ScreenRect.h + 1 - size);
	}
	return pos;
}
//static constexpr Size BodySize() { return Size(12 + rand() % 160, 12 + rand() % 160); }

enum class ItemType : int8_t
{
	Dx,
	Exp,
	//Smaller,
	//Bigger,
	//Flipper,
	Integraler
};

// dx アイテム.
class DxItem : public EffectObject
{
public:
	static constexpr ColorF dxItemBodyColor = dx2::palette::pastel::red;
	static constexpr SizeF dxItemBodySize = SizeF(size, size);
	static constexpr ColorF textColor = dx2::palette::TextColor(dxItemBodyColor);
	DxItem(const Vec2& pos, const Vec2& velocity, const PositionUpdater positionUpdater);
	DxItem() = default;
	~DxItem() = default;
	void update(UpdateContext& context) override;
	void draw() const override;
	void applyEffect(UpdateContext& context) override;
private:
	inline static const String dxItemName = U"dx";
};

// exp アイテム.
class ExpItem : public EffectObject
{
public:
	static constexpr ColorF expItemBodyColor = dx2::palette::pastel::blue;
	static constexpr SizeF expItemBodySize = SizeF(size, size);
	static constexpr ColorF textColor = dx2::palette::TextColor(expItemBodyColor);
	ExpItem(const Vec2& pos, const Vec2& velocity, const PositionUpdater positionUpdater);
	ExpItem() = default;
	~ExpItem() = default;
	void update(UpdateContext& context) override;
	void draw() const override;
	void applyEffect(UpdateContext& context) override;

private:
	inline static const String expItemName = U"exp";
};


// integral(不定積分)アイテム.
class Integraler : public EffectObject
{
public:
	static constexpr ColorF integralerBodyColor = dx2::palette::base::deepBlue;
	static constexpr SizeF integralerBodySize = SizeF(size, size);
	static constexpr ColorF textColor = dx2::palette::TextColor(integralerBodyColor);
	Integraler(const Vec2& pos, const Vec2& velocity, const PositionUpdater positionUpdater);
	Integraler() = default;
	~Integraler() = default;
	void update(UpdateContext& context) override;
	void draw() const override;
	void applyEffect(UpdateContext& context) override;

private:
	inline static const String integralerName = U"∫";
};





//// player を小さくするアイテム.
//class Smaller : public EffectObject
//{
//public:
//	Smaller();
//	~Smaller() = default;
//	void applyEffect(UpdateContext& context) override;
//
//private:
//	static constexpr ColorF smallerBodyColor = dx2::palette::pastel::paple;
//	PositionUpdater _smallerEasing = dx2::easing::Simple;
//};


//// player を大きくするアイテム.
//class Bigger : public EffectObject
//{
//public:
//	Bigger();
//	~Bigger() = default;
//	void applyEffect(UpdateContext& context) override;
//
//private:
//	static constexpr ColorF biggerBodyColor = dx2::palette::base::yellow;
//	PositionUpdater _biggerEasing = dx2::easing::Simple;
//};


//// グラフを x 軸反転させるアイテム.
//class Flipper : public EffectObject
//{
//public:
//	Flipper();
//	~Flipper() = default;
//	void applyEffect(UpdateContext& context) override;
//
//private:
//	static constexpr ColorF flipperBodyColor = dx2::palette::base::black;
//	PositionUpdater _flipperEasing = dx2::easing::Simple;
//};
