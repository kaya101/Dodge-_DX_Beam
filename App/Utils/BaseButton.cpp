#include "../stdafx.h"
#include "BaseButton.h"
#include "Common.h"

/*	BaseButton start		**************************************************************************************************/

BaseButton::BaseButton(const RectF& box, const ColorF& color, const ButtonType type)
	: _color(color)
{
	m_body.set(box, (type == ButtonType::Rect) ? 5.0: std::min(box.size.x, box.size.y) * 0.5);
	_soundName = dx2::music::GetSEname(dx2::music::SEname::Button);
	_enable = true;
}

void BaseButton::draw() const
{
	constexpr double thickness = 10.0;		// 縁の厚み.
	constexpr double frameDarkness = 0.8;	// 縁の暗さ.
	double brightness = 1.0;				// 本体の明るさ.
	ColorF color;

	// 使える時.
	if (_enable)
	{
		color = _color;
		if (m_body.mouseOver())
		{
			// 手の形にする.
			Cursor::RequestStyle(CursorStyle::Hand);

			// カーソルが上にあったら暗くする.
			brightness *= 0.8;
			if (m_body.leftPressed()) brightness *= 0.8;
		}
		else
		{
			brightness = 1.0;
		}
	}
	else
	{
		color = ColorF(0.7);
	}
	m_body.drawFrame(thickness, color * frameDarkness * brightness).draw(color * brightness);
}

/*	BaseButton end		******************************************************************************************************/


/*	TextButton start		*************************************************************************************************************/

TextButton::TextButton(const RectF& box, const ColorF& color, const String& text, const ButtonType type)
	: BaseButton(box, color, type), _text(text)
{
	_fontName = dx2::font::FontName(dx2::font::FontKey::Main);

	// 明度の差が大きい方を採用.
	_textColor = dx2::palette::TextColor(color);
}

void TextButton::draw() const
{
	BaseButton::draw();
	FontAsset(_fontName)(_text).drawAt(TextSize(), m_body.center(), _textColor);
}

/*	TextButton end		*************************************************************************************************************/
