#pragma once
#include "../Utils/Common.h"

// 衝突時の処理を行うクラス.
class Collider
{
public:

private:
};

// グラフのスクリーンの x 範囲を超えたか検知する関数.
inline static const bool isOutOfScreenX(const Vec2& pos)
{
	return (pos.x < ScreenRect.x) || (pos.x > ScreenRect.rightCenter().x);
}

// グラフのスクリーンの y 範囲を超えたか検知する関数.
inline static const bool isOutOfScreenY(const Vec2& pos)
{
	return (pos.y < ScreenRect.y) || (pos.x > ScreenRect.rightCenter().x);
}
