#include "../stdafx.h"
#include "EffectObject.h"

/*	EffectObject start		***************************************************************************************************************************/

EffectObject::EffectObject(const String& name, const Size& bodySize, const double rounded, const ColorF& bodyColor, const EasingFunc easing)
	: _name(name), _bodyColor(bodyColor), _easing(easing)
{
	_textColor = dx2::palette::TextColor(bodyColor);
	_spawnPos = SetPoint();
	_body.set(RectF(Arg::center(_spawnPos), bodySize), rounded);
	_vanishPos = SetPoint();
	_timer.set(timeLimit);
	_timer.start();
	_active = true;
	_consumed = false;
}

void EffectObject::Update(UpdateContext& context)
{
	// 出現時間を過ぎたものを削除.
	if (_timer.reachedZero())
	{
		_consumed = true;
		return;
	}
	//_body = RoundRect(Arg::center(_spawnPos + _easing(_timer.sF()) * (_vanishPos - _spawnPos)), _body.w, _body.h, _body.r);

	if (_body.intersects(context.pPlayer.body()) && CanAffect())
	{
		//getItem.play();
		ApplyEffect(context);
	}
}

void EffectObject::Draw() const
{
	_body.draw(_bodyColor);
	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(_name).drawAt(std::min(_body.w, _body.h) * 0.8, _body.center(), _textColor);
}

Vec2 EffectObject::SetPoint() const
{
	Vec2 pos{};
	// 上下.
	if (rand() % 2 == 0) {
		pos.x = ScreenRect.pos.x + rand() % static_cast<int32_t>(ScreenRect.w + 1);
		pos.y = (rand() % 2 == 0) ? ScreenRect.y : ScreenRect.bottomCenter().y;
	}

	// 左右.
	else {
		pos.x = (rand() % 2 == 0) ? ScreenRect.x : ScreenRect.rightCenter().x;
		pos.y = ScreenRect.pos.y + rand() % static_cast<int32_t>(ScreenRect.h + 1);
	}
	return pos;
}

/*	EffectObject end		*************************************************************************************************************************/


/*******		Shake start	**********************************************************************************************************************************/

Shake::Shake()
	: EffectObject(U"dx", BodySize(), 8, shakeBodyColor, _shakeEasing) {
}

void Shake::ApplyEffect(UpdateContext& context)
{
	context.pPlayer.takeDamage();
	context.pGraph.Differentiate().Create(
		&context.pCamera,
		[&](const double x)
		{
			return dx2::math::EvalHoner(context.pGraph.CM(), x);
		}
	);
	_consumed = true;
}

/*******		Shake end	**********************************************************************************************************************************/


/*******		Shot start	**********************************************************************************************************************************/

Shot::Shot()
	: EffectObject(U"dx", BodySize(), 8, shotBodyColor, _shotEasing) {
}

void Shot::Draw() const
{
	EffectObject::Draw();
	// 進む方向を描く.
	if (_timer.progress0_1() < 0.4)
	{
		Line(_spawnPos, _vanishPos).drawArrow(4, SizeF(32, 32), dx2::palette::base::red);
	}
}

void Shot::ApplyEffect(UpdateContext& context)
{
	context.pPlayer.takeDamage();
	context.pGraph.Differentiate().Create(
		&context.pCamera,
		[&](const double x)
		{
			return dx2::math::EvalHoner(context.pGraph.CM(), x);
		}
	);
	_consumed = true;
}

/*******		Shot end	**********************************************************************************************************************************/


/*******		ExpItem	start		******************************************************************************************************************************/

ExpItem::ExpItem()
	: EffectObject(U"exp", Size(size, size), size * 0.5, expItemBodyColor, _expItemEasing) {
}

void ExpItem::ApplyEffect(UpdateContext& context)
{
	_consumed = true;
	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Exp));
	context.pGraph.Create(&context.pCamera,
		[](const double x)
		{
			return std::exp(x);
		}
	);
}

/*******		Exp	end		**********************************************************************************************************************************/


/*******		Smaller start	******************************************************************************************************************************/

Smaller::Smaller()
	: EffectObject(U"S", Size(size, size), size * 0.5, smallerBodyColor, _smallerEasing) {
}

void Smaller::ApplyEffect(UpdateContext& context)
{
	_consumed = true;
	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Smaller));
}

/*******		Smaller	end		******************************************************************************************************************************/


/*******		Bigger start	******************************************************************************************************************************/

Bigger::Bigger()
	: EffectObject(U"B", Size(size, size), size * 0.5, biggerBodyColor, _biggerEasing) {
}

void Bigger::ApplyEffect(UpdateContext& context)
{
	_consumed = true;
	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Bigger));
}

/*******		Bigger end		******************************************************************************************************************************/


/*******		Flipper	start	******************************************************************************************************************************/

Flipper::Flipper()
	: EffectObject(U"-X", Size(size, size), size * 0.5, flipperBodyColor, _flipperEasing) {
}

void Flipper::ApplyEffect(UpdateContext& context)
{
	_consumed = true;
	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Flipper));
	context.pGraph.Create(&context.pCamera,
		[&](const double x)
		{
			return dx2::math::EvalHoner(context.pGraph.CM(), -x);
		}
	);
}

/*******		Flipper	end		******************************************************************************************************************************/


/*******		Integral start	******************************************************************************************************************************/

Integraler::Integraler()
	: EffectObject(U"∫", Size(size, size), size * 0.5, integralerBodyColor, _integralerEasing) {
}

void Integraler::ApplyEffect(UpdateContext& context)
{
	_consumed = true;
	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Integraler));
	context.pGraph.Integrate().Create(
		&context.pCamera,
		[&](const double x)
		{
			return dx2::math::EvalHoner(context.pGraph.CM(), x);
		}
	);
}

/*******		Integral end		******************************************************************************************************************************/
