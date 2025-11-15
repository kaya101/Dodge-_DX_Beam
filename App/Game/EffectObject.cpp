#include "../stdafx.h"
#include "EffectObject.h"
#include <utility> // std::move

/*	EffectObject start		***************************************************************************************************************************/

EffectObject::EffectObject(String name, const ColorF& color, const Vec2& pos, const SizeF& size, const Vec2& velocity, const PositionUpdater positionUpdater) :
	m_name(std::move(name)), m_color(color), m_spawnPos(pos), m_velocity(velocity), m_positionUpdater(positionUpdater)
{
	m_body.set(RectF(Arg::center(m_spawnPos), size), std::min(size.x, size.y) * 0.5);
	m_active = true;
	m_consumed = false;
}

/*	EffectObject end		*************************************************************************************************************************/


/*******		DxItem start	**********************************************************************************************************************************/

DxItem::DxItem(const Vec2& pos, const Vec2& velocity, const PositionUpdater positionUpdater)
	: EffectObject(dxItemName, dxItemBodyColor, pos, dxItemBodySize, velocity, positionUpdater)
{}

void DxItem::update(UpdateContext& context)
{
	// 出現時間を過ぎたものを削除.
	/*if (_timer.reachedZero())
	{
		m_consumed = true;
		return;
	}*/
	//m_body = RoundRect(Arg::center(m_spawnPos + m_positionUpdater(_timer.sF()) * (_vanishPos - m_spawnPos)), m_body.w, m_body.h, m_body.r);
	m_body.setCenter(m_body.center() + m_positionUpdater(m_velocity));

	if (m_body.intersects(context.pPlayer.body()) && canAffect())
	{
		//getItem.play();
		applyEffect(context);
	}
}

void DxItem::draw() const
{
	m_body.draw(m_color);
	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(m_name).drawAt(std::min(m_body.w, m_body.h) * 0.8, m_body.center(), textColor);
}

void DxItem::applyEffect(UpdateContext& context)
{
	m_consumed = true;
	context.pPlayer.takeDamage();
	/*context.pGraph.Differentiate().Create(
		&context.pCamera,
		[&](const double x)
		{
			return dx2::math::EvalHoner(context.pGraph.cm(), x);
		}
	);*/
}

/*******		DxItem end	**********************************************************************************************************************************/


/*******		ExpItem	start		******************************************************************************************************************************/

ExpItem::ExpItem(const Vec2& pos, const Vec2& velocity, const PositionUpdater positionUpdater)
	: EffectObject(expItemName, expItemBodyColor, pos, expItemBodySize, velocity, positionUpdater) {
}

void ExpItem::update(UpdateContext& context)
{
	// 出現時間を過ぎたものを削除.
	/*if (_timer.reachedZero())
	{
		m_consumed = true;
		return;
	}*/
	//m_body = RoundRect(Arg::center(m_spawnPos + m_positionUpdater(_timer.sF()) * (_vanishPos - m_spawnPos)), m_body.w, m_body.h, m_body.r);
	m_body.setCenter(m_body.center() + m_positionUpdater(m_velocity));

	if (m_body.intersects(context.pPlayer.body()) && canAffect())
	{
		//getItem.play();
		applyEffect(context);
	}
}

void ExpItem::draw() const
{
	m_body.draw(m_color);
	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(m_name).drawAt(std::min(m_body.w, m_body.h) * 0.8, m_body.center(), textColor);
}

void ExpItem::applyEffect(UpdateContext& context)
{
	m_consumed = true;
	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Exp));
	/*context.pGraph.Create(&context.pCamera,
		[](const double x)
		{
			return std::exp(x);
		}
	);*/
}

/*******		Exp	end		**********************************************************************************************************************************/


/*******		Integral start	******************************************************************************************************************************/

Integraler::Integraler(const Vec2& pos, const Vec2& velocity, const PositionUpdater positionUpdater)
	: EffectObject(integralerName, dx2::palette::base::deepBlue, pos, integralerBodySize, velocity, positionUpdater) {
}

void Integraler::update(UpdateContext& context)
{
	// 出現時間を過ぎたものを削除.
	/*if (_timer.reachedZero())
	{
		m_consumed = true;
		return;
	}*/
	//m_body = RoundRect(Arg::center(m_spawnPos + m_positionUpdater(_timer.sF()) * (_vanishPos - m_spawnPos)), m_body.w, m_body.h, m_body.r);
	m_body.setCenter(m_body.center() + m_positionUpdater(m_velocity));

	if (m_body.intersects(context.pPlayer.body()) && canAffect())
	{
		//getItem.play();
		applyEffect(context);
	}
}

void Integraler::draw() const
{
	m_body.draw(m_color);
	FontAsset(dx2::font::FontName(dx2::font::FontKey::Main))(m_name).drawAt(std::min(m_body.w, m_body.h) * 0.8, m_body.center(), textColor);
}

void Integraler::applyEffect(UpdateContext& context)
{
	m_consumed = true;
	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Integraler));
	/*context.pGraph.Integrate().Create(
		&context.pCamera,
		[&](const double x)
		{
			return dx2::math::EvalHoner(context.pGraph.cm(), x);
		}
	);*/
}

/*******		Integral end		******************************************************************************************************************************/


/*******		Smaller start	******************************************************************************************************************************/

//Smaller::Smaller()
//	: EffectObject(U"S", Size(size, size), size * 0.5, smallerBodyColor, _smallerEasing) {
//}
//
//void Smaller::applyEffect(UpdateContext& context)
//{
//	m_consumed = true;
//	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Smaller));
//}

/*******		Smaller	end		******************************************************************************************************************************/


/*******		Bigger start	******************************************************************************************************************************/

//Bigger::Bigger()
//	: EffectObject(U"B", Size(size, size), size * 0.5, biggerBodyColor, _biggerEasing) {
//}
//
//void Bigger::applyEffect(UpdateContext& context)
//{
//	m_consumed = true;
//	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Bigger));
//}

/*******		Bigger end		******************************************************************************************************************************/


/*******		Flipper	start	******************************************************************************************************************************/

//Flipper::Flipper()
//	: EffectObject(U"-X", Size(size, size), size * 0.5, flipperBodyColor, _flipperEasing) {
//}
//
//void Flipper::applyEffect(UpdateContext& context)
//{
//	m_consumed = true;
//	context.pPlayer.pickupItem(static_cast<int32_t>(ItemType::Flipper));
//	context.pGraph.Create(&context.pCamera,
//		[&](const double x)
//		{
//			return dx2::math::EvalHoner(context.pGraph.cm(), -x);
//		}
//	);
//}

/*******		Flipper	end		******************************************************************************************************************************/
