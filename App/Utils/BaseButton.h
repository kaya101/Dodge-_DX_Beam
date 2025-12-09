#pragma once

// ボタンの基本クラス.
class BaseButton
{
public:
	enum class ButtonType : int8_t { Rect, Circle };// 四角形と円.
	explicit BaseButton() = default;
	BaseButton(const RectF& box, const ColorF& color, const ButtonType type = ButtonType::Rect);
	virtual void draw() const;
	void DisableButtonIfUnavailable() { _enable = false; }
	void EnableButtonIfAvailable() { _enable = true; }
	bool IsReleased() const { PlaySound(); return m_body.leftReleased(); }
	bool IsEnable() const { return _enable; }

protected:
	s3d::RoundRect m_body;	// 当たり判定.
	ColorF _color;			// ボタンの色.
	String _soundName;		// 押したときの音.
	bool _enable;			// 押せるかどうか.

	// 押した音を鳴らす.
	void PlaySound() const { /*AudioAsset(_soundName).play();*/ }
};


// 文字付きのボタン.
class TextButton : public BaseButton
{
public:
	explicit TextButton() = default;
	TextButton(const RectF& box, const ColorF& color, const String& text, const ButtonType type = ButtonType::Rect);
	void draw() const override;
	const String& Text() const { return _text; }

private:
	String _text;		// 文字.
	String _fontName;	// フォントの名前.
	ColorF _textColor;	// 文字の色.

	// 幅の狭い方に合わせた文字の大きさ.
	constexpr double TextSize() const { return std::min(m_body.w, m_body.h) * 0.32; }
};
