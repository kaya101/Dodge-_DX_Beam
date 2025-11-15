#pragma once
#include "../Utils/Common.h"
#include "Graph.h"

// player の入力操作.
enum class PlayerAction : size_t
{
	Left,
	Right,
	Count
};

template <class T>
struct EnumArray
{
	std::array<T, static_cast<size_t>(PlayerAction::Count)> data{};
	T& operator[](PlayerAction a) { return data[static_cast<size_t>(a)]; }
	const T& operator[](PlayerAction a) const { return data[static_cast<size_t>(a)]; }
};

using PlayerButtons = EnumArray<bool>;


// player クラス.
class Player
{
public:

	//// player の大きさ.
	//enum class Size : int8_t
	//{
	//	Smallest,// 縮小 2 段階目.
	//	Smaller, // 縮小 1 段階目.
	//	Normal,  // 基本サイズ.
	//	Bigger,  // 拡大 1 段階目.
	//	Biggest, // 拡大 2 段階目.
	//	PlayerSizeNum// これを最後に書く.
	//};

	Player(const int32_t pos);
	constexpr ~Player() = default;

	void update(Graph* pGraph, const GraphCamera* pCamera);

	void draw(const Font& font = FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))) const;


	// 一定時間しか発火させたくないもの.
	static const std::map<String, SecondsF> Cooldowns;

	// player の色.
	static const std::map<String, ColorF> BodyColor;

	static constexpr double interval = 0.016;

	// player の大きさをまとめた配列.
	static const std::vector<double> playerSizes;

	// ダメージが入る唯一の関数.
	void takeDamage(const int32_t amount = 1);

	// ダメージを回復する唯一の関数.
	void takeHeal(const int32_t amount = 1);

	// アイテム効果を反映する唯一の関数.
	void pickupItem(const int32_t itemType);

	// タイマー操作関数.
	void restartCoolTimer() { _coolTimer.restart(); }
	void restartExpTimer() { _expTimer.restart(); }
	void restartSizeTimer() { _sizeTimer.restart(); }

	// ゲッター.
	const Circle& body() const { return m_body; };
	int32_t sizeIndex() const { return _sizeIndex; }
	int32_t degree() const { return _degree; }
	bool isCompletelyDxed() const { return _completelyDxed; }
	bool isDamaged() const { return _damaged; }
	bool isInvincible() const { return _invincible; }
	bool isChangedSize() const { return _changedSize; }
	SecondsF lastCoolTime() const { return _coolTimer.remaining(); }
	SecondsF lastExpTime() const { return _expTimer.remaining(); }
	SecondsF lastSizeTime() const { return _sizeTimer.remaining(); }

private:
	Circle m_body;// 当たり判定 & 表示.
	int32_t _sizeIndex;
	int32_t _pos;// player がいるグラフ上の点.
	int32_t _degree;// 関数の今の次数.
	bool _completelyDxed;// ゲームオーバーの時に true.
	bool _damaged;// 被弾しているかどうか.
	bool _invincible;// 無敵状態かどうか.
	bool _changedSize;
	bool _flipped;
	s3d::Timer _coolTimer{ Cooldowns.at(U"damage") };// 被弾後の無敵時間.
	s3d::Timer _expTimer{ Cooldowns.at(U"exp") };// 無敵時間.
	s3d::Timer _sizeTimer{ Cooldowns.at(U"size") };
	double accumlatedTime;
	PlayerButtons _buttons;// 入力操作のボタン.
};
