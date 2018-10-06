#include "UIText.h"
#include "core/Common/TrikytaEngine.h"
#include "core/Common/Color.h"

using namespace UI;

Text::Text(const std::string& p_Text,const std::string& p_Font, uint8 p_TextSize,Vec2i p_Pos, Color p_Color, bool p_IsRegister):
	Drawable(p_Pos, Vec2i(0, 0), p_IsRegister),
	m_Text(p_Text),
	m_FontPath(p_Font),
	m_TextSize(p_TextSize),
	m_Color(p_Color),
	m_Scale(1),
	m_BGColor(0,0,0,0)
{
	init();
}

Text::~Text()
{
	m_Font->Destory();
}

bool Text::init()
{
	m_Font = Font::createOrGetFont(m_FontPath, m_TextSize);
	if (m_Font == NULL) {
		LogInfoConsole("ERROR: loading font path %s", m_FontPath.c_str());
	}
	///TODO: IMPLMENT BG COLLOUR
	SDL_Surface* textSurface = TTF_RenderText_Blended(m_Font->getFont(), m_Text.c_str(), { m_Color.r,m_Color.g, m_Color.b });
	if (textSurface == NULL) {
		LogInfoConsole("ERROR: loading text with font %s", m_FontPath.c_str());
		return false;
	}
	m_Texture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);

	int temp_w, temp_h;
	TTF_SizeText(m_Font->getFont(), m_Text.c_str(), &temp_w, &temp_h);
	setSize(Vec2i(temp_w, temp_h));
	m_SourceDrawCoord = {0,0,temp_w,temp_h};
	SetRotationCenter(Vec2i(m_Size->x / 2, m_Size->y / 2));

	SDL_FreeSurface(textSurface);
	//TTF_CloseFont(font);
	return true;
}

void Text::updateText(const std::string& p_Text)
{
	m_Text = p_Text;
	if (m_Font == NULL) {
		LogInfoConsole("ERROR: No font found while updating text! (Font Path: %s)", m_FontPath.c_str());
		return;
	}

	SDL_Surface* textSurface = TTF_RenderText_Solid(m_Font->getFont(), m_Text.c_str(), { m_Color.r,m_Color.g, m_Color.b });
	SDL_DestroyTexture(m_Texture);
	m_Texture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);
	SDL_FreeSurface(textSurface);

	int temp_w, temp_h;
	TTF_SizeText(m_Font->getFont(), m_Text.c_str(), &temp_w, &temp_h);
	setSize(Vec2i(temp_w*m_Scale, temp_h*m_Scale));
	m_SourceDrawCoord = { 0,0,temp_w*m_Scale,temp_h*m_Scale };
	SetRotationCenter(Vec2i(m_Size->x / 2, m_Size->y / 2));
}

void Text::setScale(uint8 p_Scale)
{
	m_Scale = p_Scale;
	setSize(Vec2i((int)(m_Size->x*p_Scale), (int)(m_Size->y*p_Scale)));
	m_SourceDrawCoord = { 0,0,(m_Size->x*p_Scale), (int)(m_Size->y*p_Scale)};
}

void Text::setColor(Color p_Color)
{
	m_Color = p_Color;
	if (m_Font == NULL) {
		LogInfoConsole("ERROR: No font found while updating text! (Font Path: %s)", m_FontPath.c_str());
		return;
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_Font->getFont(), m_Text.c_str(), { m_Color.r,m_Color.g, m_Color.b });
	SDL_DestroyTexture(m_Texture);
	m_Texture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);
	SDL_FreeSurface(textSurface);
}