#include "../stdafx.h"
#include "Player.h"
#include "EffectObject.h"

Player::Player(const int32_t pos)
	: _pos(pos),
	  m_body(0,0,0),
	  /*_sizeIndex(static_cast<int32_t>(Size::Normal)),*/
	  _sizeIndex(2),
	  _degree(3),
	  _completelyDxed(false),
	  _damaged(false),
	  _invincible(false),
	  _changedSize(false),
	  _flipped(false)
{
	accumlatedTime = 0.0000000000;
	_coolTimer.reset();
	_expTimer.reset();
	_sizeTimer.reset();
}

void Player::update(Graph* pGraph, const GraphCamera* pCamera)
{
	// 入力を受け取る.
	_buttons[PlayerAction::Left] = KeyLeft.pressed() || KeyA.pressed();
	_buttons[PlayerAction::Right] = KeyRight.pressed() || KeyD.pressed();

	// 被弾後無敵の時間が終わったとき.
	if (_coolTimer.reachedZero()) {
		_coolTimer.reset();
		_damaged = false;
	}

	// exp の無敵時間が終わったとき.
	if (_expTimer.reachedZero()) {
		_expTimer.reset();
		_invincible = false;
		pGraph->Create(pCamera, [&](const double x) { return dx2::math::EvalHoner(pGraph->CM(), x); });
		//createGraph(&_camera, [&](const double x) { return dx2::math::EvalHoner(_graph.CM(), x); });
	}

	if (_sizeTimer.reachedZero()) {
		_sizeTimer.reset();
		//_sizeIndex = static_cast<int32_t>(Size::Normal);// 半径を戻す.
		_sizeIndex = 2;// 半径を戻す.
		_changedSize = false;
	}

	accumlatedTime += Scene::DeltaTime();

	if (interval < accumlatedTime) {
		_pos += (_buttons[PlayerAction::Right] - _buttons[PlayerAction::Left]) * 6;
		_pos = (_pos < 0) ? 0 : (_pos > pGraph->NumSamples() - 1) ? pGraph->NumSamples() - 1 : _pos;
		accumlatedTime = 0.0000000000;
	}

	if (m_body.center.y < ScreenRect.y) {
		_pos -= 6;
	}
	if (m_body.center.y > ScreenRect.bottomY()) {
		_pos -= 6;
	}


	// 移動先の座標へ変更する.
	m_body = Circle(pGraph->Curve()[_pos], playerSizes[_sizeIndex]);
	//m_body = Circle(_graph.Curve()[_pos], playerSizes[_sizeIndex]);
}

void Player::draw(const Font& font) const
{
	// 点 P の描画.
	if (_invincible) {
		dx2::draw::DrawSprite(m_body, dx2::draw::ChangeColor(_expTimer, BodyColor.at(U"exp")), U"P", font, m_body.r * 1.2, dx2::palette::base::white);
	}
	else if (_damaged) {
		dx2::draw::DrawSprite(m_body, dx2::draw::ChangeColor(_coolTimer, BodyColor.at(U"normal"), true), U"P", font, m_body.r * 1.2, dx2::palette::base::white);
	}
	else {
		dx2::draw::DrawSprite(m_body, dx2::draw::ChangeColor(_sizeTimer, BodyColor.at(U"normal")), U"P", font, m_body.r * 1.2, dx2::palette::base::white);
	}
}

void Player::takeDamage(const int32_t amount)
{
	if (_invincible || _damaged) return;

	//hit.play();
	_degree = std::max(-1, _degree - amount);

	if (_degree == -1) _completelyDxed = true;

	restartCoolTimer();
	_damaged = true;
}

void Player::takeHeal(const int32_t amount)
{
	if (_invincible) return;

	//hit.play();
	_degree = std::max(5, _degree + amount);

	// 不定積分.
	//integrate().createGraph(&_camera, [&](const double x) { return dx2::math::EvalHoner(_graph.CM(), x); });
}

void Player::pickupItem(const int32_t itemType)
{
	if (itemType == static_cast<int32_t>(ItemType::Exp)) {
		_invincible = true;
		//createExpGraph();
		//createGraph(&_camera, [&](const double x) { return std::exp(x); });
		restartExpTimer();
	}
	//else if (itemType == static_cast<int32_t>(ItemType::Smaller)) {
	//	restartSizeTimer();
	//	_changedSize = true;
	//	// player サイズを一段階さらに小さくする.
	//	(_sizeIndex - 1 <= static_cast<int32_t>(Size::Smallest)) ? _sizeIndex = static_cast<int32_t>(Size::Smallest) : _sizeIndex = _sizeIndex - 1;
	//}
	//else if (itemType == static_cast<int32_t>(ItemType::Bigger)) {
	//	restartSizeTimer();
	//	_changedSize = true;
	//	// player サイズを一段階さらに大きくする.
	//	(_sizeIndex + 1 >= static_cast<int32_t>(Size::Biggest)) ? _sizeIndex = static_cast<int32_t>(Size::Biggest) : _sizeIndex = _sizeIndex + 1;
	//}
	//else if (itemType == static_cast<int32_t>(ItemType::Flipper)) {
	//	//if (_invincible) createGraph(&_camera, [&](const double x) { return std::exp(-x); });
	//	//else createGraph(&_camera, [&](const double x) { return dx2::math::EvalHoner(_graph.CM(), -x); });
	//}
	else if (itemType == static_cast<int32_t>(ItemType::Integraler)) {
		takeHeal();
	}
	else {
		Print << U"Undefined Item is called.";
		Print << U"This error is called in Player::pickupItem.";
	}
}

const std::map<String, SecondsF> Player::Cooldowns = {
		{ U"damage", 1.6s },
		{ U"exp", 3.0s },
		{ U"size", 3.0s },
		{ U"flip", 3.0s },
};

const std::map<String, ColorF> Player::BodyColor = {
	{ U"normal", dx2::palette::base::blue },
	{ U"exp", dx2::palette::base::paple }
};

const std::vector<double> Player::playerSizes = {
	10.00,
	20.00,
	30.00,
	40.00,
	50.00
};
