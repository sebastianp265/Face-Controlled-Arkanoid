#include "TextUtils.h"

sf::Vector2f getLocalSize(const sf::Text& text)
{
	const sf::String str = text.getString() + '\n';

	float maxLineWidth = 0.f;
	float lineWidth = 0.f;
	unsigned int lines = 0;

	for (sf::String::ConstIterator itr = str.begin(); itr != str.end(); ++itr)
	{
		if (*itr == '\n')
		{
			++lines;
			maxLineWidth = std::max(maxLineWidth, lineWidth);
			lineWidth = 0.f;
		}
		else
		{
			lineWidth += text.getFont()->getGlyph(*itr, text.getCharacterSize(), text.getStyle() & sf::Text::Bold).advance;
		}
	}

	const float lineHeight = static_cast<float>(text.getFont()->getLineSpacing(text.getCharacterSize()));
	return sf::Vector2f(maxLineWidth, lines * lineHeight);
}
