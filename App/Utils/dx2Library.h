#pragma once

// 画面サイズの定数.
static constexpr int32_t windowWidth = 1280;
static constexpr int32_t windowHeight = 720;

// 全画面.
static constexpr RectF AllWindow(0, 0, windowWidth, windowHeight);

// 係数行列.
using CM = std::vector<double>;

namespace dx2
{
	namespace palette
	{
		namespace base
		{
			static constexpr ColorF red{ U"#e2041b" };
			static constexpr ColorF orange{ U"#f6ad49" };
			static constexpr ColorF yellow{ U"#ffd900" };
			static constexpr ColorF lime{ U"#c3d825" };
			static constexpr ColorF green{ U"#38b48b" };
			static constexpr ColorF blue{ U"#2ca9e1" };
			static constexpr ColorF deepBlue{ U"#4d5aaf" };
			static constexpr ColorF paple{ U"#9d5b8b" };
			static constexpr ColorF white{ U"#e8ecef" };
			static constexpr ColorF black{ U"#0d0015" };
			static constexpr ColorF gray{ U"#adadad" };
		};

		namespace pastel
		{
			static constexpr ColorF red{ U"#ff9999" };
			static constexpr ColorF pink{ U"#ff99ff" };
			static constexpr ColorF orange{ U"#ffcc99" };
			static constexpr ColorF yellow{ U"#ffff99" };
			static constexpr ColorF green{ U"#99ff99" };
			static constexpr ColorF water{ U"#99ffff" };
			static constexpr ColorF blue{ U"#9999ff" };
			static constexpr ColorF paple{ U"#cc99ff" };
		}

		// 明度の差が大きい方を採用.
		constexpr ColorF TextColor(const ColorF& color)
		{
			constexpr double mid = (dx2::palette::base::white.grayscale() - dx2::palette::base::black.grayscale()) * 0.5;
			return (color.grayscale() < mid) ? dx2::palette::base::white : dx2::palette::base::black;
		}

		// 色確認関数.
		inline static const void CheckColor(const Rect& rect, const ColorF& colorf)
		{
			rect.rounded(5).draw(colorf).drawFrame(5, 5, colorf * 0.8);
		}

		static const void CheckAllColor()
		{
			// 基本七色 of 私.
			CheckColor(Rect{ 20, 20, 50, 50 }, base::red);
			CheckColor(Rect{ 20, 85, 50, 50 }, base::orange);
			CheckColor(Rect{ 20, 150, 50, 50 }, base::yellow);
			CheckColor(Rect{ 20, 215, 50, 50 }, base::lime);
			CheckColor(Rect{ 20, 280, 50, 50 }, base::green);
			CheckColor(Rect{ 20, 345, 50, 50 }, base::blue);
			CheckColor(Rect{ 20, 410, 50, 50 }, base::deepBlue);
			CheckColor(Rect{ 20, 475, 50, 50 }, base::paple);

			// パステルカラー.
			CheckColor(Rect{ 90, 20, 50, 50 }, pastel::red);
			CheckColor(Rect{ 90, 85, 50, 50 }, pastel::pink);
			CheckColor(Rect{ 90, 150, 50, 50 }, pastel::orange);
			CheckColor(Rect{ 90, 215, 50, 50 }, pastel::yellow);
			CheckColor(Rect{ 90, 280, 50, 50 }, pastel::green);
			CheckColor(Rect{ 90, 345, 50, 50 }, pastel::water);
			CheckColor(Rect{ 90, 410, 50, 50 }, pastel::blue);
			CheckColor(Rect{ 90, 475, 50, 50 }, pastel::paple);
		}
	}

	namespace easing
	{
		// 単調変化.
		inline static const double Simple(const double t)
		{
			return -(cos(std::_Pi_val * t) - 1) / 2;
		}

		// 最初が早い.
		inline static const double QuickFirst(const double t)
		{
			return 1 - std::pow(1 - t, 3);
		}

		// 真ん中だけ早い.
		inline static const double QuickMid(const double t)
		{
			return t < 0.5 ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
		}

		// 最後だけ早い.
		inline static const double QuickLast(const double t)
		{
			return t * t * t * t * t;
		}


		// ためてから動く.
		inline static const double BackAndQuick(const double t)
		{
			const double c1 = 1.70158;
			const double c3 = c1 + 1;

			return c3 * t * t * t - c1 * t * t;
		}

		// 最大までためて撃つ.
		static const double ChargeShot(const double t)
		{
			const double c5 = (2 * std::_Pi_val) / 4.5;

			return t == 0
				? 0
				: t == 1
				? 1
				: t < 0.5
				? -(std::pow(2, 20 * t - 10) * std::sin((20 * t - 11.125) * c5)) / 2
				: (std::pow(2, -20 * t + 10) * std::sin((20 * t - 11.125) * c5)) / 2 + 1;
		}
	}

	namespace draw
	{
		// シーン切り替えをやる関数.
		inline const void SceneMove(const ColorF& colorf, const double t)
		{
			AllWindow.draw(ColorF{ colorf, t });// t 秒経つごとにその色に近づいていく.
		}

		// 図形を描く.
		inline const void DrawSprite(const RectF& shape, const ColorF& bodyColor, const String& text, const Font& font, const double size, const ColorF& textColor)
		{
			shape.rounded(5).draw(bodyColor);
			font(text).drawAt(size, shape.center(), textColor);
		}
		inline const void DrawSprite(const Circle& shape, const ColorF& bodyColor, const String& text, const Font& font, const double size, const ColorF& textColor)
		{
			shape.draw(bodyColor);
			font(text).drawAt(size, shape.center, textColor);
		}

		// 残り時間で点滅する関数.
		inline const ColorF ChangeColor(const s3d::Timer& timer, const ColorF& baseColor, const bool isSlowBlink = false)
		{
			// 残り時間.
			const double lastTime = timer.sF();

			// 設定時間.
			const double limit = timer.duration().count();

			if (lastTime < limit * 0.04) {// 戻った瞬間は点滅しない.
				return baseColor;
			}
			else if (lastTime < limit * 0.2) {// もうすぐ時間が切れるときの点滅.
				return (static_cast<int32_t>(lastTime * 30) % 2 == 0) ? baseColor : ColorF{ baseColor, 0.2 };
			}
			else if (isSlowBlink) {// 初めはゆっくりと点滅する.
				return (static_cast<int32_t>(lastTime * 10) % 2 == 0) ? baseColor : ColorF{ baseColor, 0.2 };
			}
			else {
				return baseColor;
			}
		}

		// カーソルと重なると少し暗くなるボタン.
		inline const void DrawHoverDarkenedButton(const RectF& box, const ColorF& color, const String& text, const Font& font, const uint16 textSize, const ColorF& textColor)
		{
			if (box.mouseOver()) {
				Cursor::RequestStyle(CursorStyle::Hand);
				box.rounded(5).drawFrame(5, 5, color * 0.6).draw(color * 0.8);
				font(text).drawAt(textSize, box.center(), textColor);
			}
			else {
				box.rounded(5).draw(color);
				font(text).drawAt(textSize, box.center(), textColor);
			}
		}
	}

	namespace font
	{
		enum class FontKey : int8_t { Title, Main };

		// 名前の文字列に変換. AuioAsset で文字列を使用するため.
		inline const String& FontName(const FontKey key)
		{
			switch (key)
			{
			case FontKey::Title: return U"TitleFont";
			case FontKey::Main: return U"MainFont";
			default: return U"TitleFont";
			}
		}

		inline const Font& TitleFont()
		{
			static const Font font = FontAsset(FontName(FontKey::Title));
			return font;
		}

		inline const Font& MainFont()
		{
			static const Font font = FontAsset(FontName(FontKey::Main));
			return font;
		}
	}

	namespace math
	{
		// 線形から対数的な変化へと変換する関数.
		static constexpr double LinearToLog(double value)
		{
			if (value == 0.0) return 0.0;

			constexpr double minDb = -40.0; // 必要に応じて調整
			const double db = (minDb * (1.0 - value));
			return std::pow(10.0, (db / 20.0));
		}

		// 係数行列から f(x) の値を計算する関数.
		static const double EvalHoner(const CM& cm, const double x)
		{
			// Horner 法で求めると計算量が減らせる.
			const int32_t n = static_cast<int32_t>(cm.size());
			double y = cm[n - 1];
			for (int32_t i = n - 2; i >= 0; --i) y = std::fma(y, x, cm[i]);
			return y;
		}

		// 微分する関数.
		static const CM Differentiate(const CM& cm)
		{
			const int32_t n = static_cast<int32_t>(cm.size());
			CM altered(n, 0.0);
			for (int32_t i = 0; i < n - 1; ++i) altered[i] = cm[i + 1] * (i + 1);
			return altered;
		}

		// 不定積分する関数. 最高次の項が消失するため, 使用時は配列に余白を持たせること.
		static const CM Integrate(const CM& cm, const double C = 0.0)
		{
			const int32_t n = static_cast<int32_t>(cm.size());
			CM altered(n, 0.0);
			altered[0] = C;
			for (int32_t i = 1; i < n; ++i) altered[i] = cm[i - 1] / i;
			return altered;
		}

		// 定積分する関数.
		static const double DefiniteIntegral(const CM& cm, const double a, const double b)
		{
			CM altered = Integrate(cm);
			const double F_b = EvalHoner(altered, b);
			const double F_a = EvalHoner(altered, a);
			return F_b - F_a;
		}

		// デフォルトの係数行列.
		static const CM DefaultCM
		{
			0.0,
			-1.5,
			0.0,
			0.1666666666666666,
			0.0,
			0.0
		};
	}

	namespace music
	{
		enum class BGMname : int8_t
		{
			Title,
			Play
		};

		enum class SEname : int8_t
		{
			Button,
			Decide,
			Damaged,
			GetItem
		};

		// 名前の文字列に変換. AuioAsset で文字列を使用するため.
		inline static const String GetBGMname(const BGMname name)
		{
			switch (name)
			{
			case BGMname::Title: return U"Title";
			case BGMname::Play: return U"Play";
			default: return U"Title";
			}
		}

		inline static const String GetSEname(const SEname name)
		{
			switch (name)
			{
			case SEname::Button: return U"Button";
			case SEname::Decide: return U"Decide";
			case SEname::Damaged: return U"Damaged";
			case SEname::GetItem: return U"GetItem";
			default: return U"Button";
			}
		}

		// 背景音.
		static const std::map<String, String> bgm = {
			{ GetBGMname(BGMname::Title), U"../Assets/bgm/Action.mp3"},// 「https://www.youtube.com/watch?v=srx4u883Dys」.
			{ GetBGMname(BGMname::Play), U"../Assets/bgm/EXP.mp3"}// 「https://www.youtube.com/watch?v=wGwfU-1TKe8」.
		};

		// 効果音.
		static const std::map<String, std::tuple<GMInstrument, uint8, Duration>> se = {
			{ GetSEname(SEname::Button), { GMInstrument::Piano1, PianoKey::C4, 0.4s }},
			{ GetSEname(SEname::Decide), { GMInstrument::Piano1, PianoKey::C4, 0.4s }},
			{ GetSEname(SEname::Damaged), { GMInstrument::Piano1, PianoKey::C4, 0.4s }},
			{ GetSEname(SEname::GetItem), { GMInstrument::Piano1, PianoKey::C4, 0.4s }}
		};
	}
}
