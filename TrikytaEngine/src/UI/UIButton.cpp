#include "UIButton.hpp"
#include "misc/Font.h"
#include "UIManager.h"
#include "UI/UIText.h"
#include "core/Common/TrikytaEngine.h"
#include "core/Common/Macros.hpp"
#include "Events/UIEventHandler.h"

using namespace UI;

Button::Button(const std::string& p_text, Font* font, const Vec2i& p_pos, const Vec2i& p_size, const Color& text_color) 
{
	Manager::addElement(this, true);

	m_BtnText = Text::createText(p_text, font, p_pos, text_color, false);
	m_BtnText->setPosition((p_size - m_BtnText->getSize()) / 2);
	m_Pos = p_pos;
	m_Size = p_size;
	Button::buildWidget();
}

void Button::buildWidget()
{
	auto r = ENGINE->getRenderer();
	m_WidgetBounderies = SDL_Rect{ m_Pos.x, m_Pos.y, m_Size.x, m_Size.y };
	widget_texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidgetBounderies.w, m_WidgetBounderies.h);
	int padding = 1;
	auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
	SDL_SetRenderTarget(r, widget_texture);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	for (int i = padding; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
	}
	m_BtnText->render(0.f);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_SetRenderTarget(r, NULL);
}

void Button::render(float dt)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(widget_texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(r, widget_texture, NULL, &m_WidgetBounderies);
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}

void Button::OnUIClick(const Vec2i& pos, bool is_down)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderTarget(r, widget_texture);
	if (is_down) {
		int padding = 2;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 10, 206, 250, 220);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 0, 0, 0, 220);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
			SDL_SetRenderDrawColor(r, 0, 0, 0, sartingAlpha - i * 4);
		}
	}else {
		int padding = 1;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 10, 250, 170, 220);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
		}
	}
	m_BtnText->render(0.f);
	SDL_SetRenderTarget(r, NULL);
}

void Button::OnUIMouseHover(const Vec2i& pos, bool isHover)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderTarget(r, widget_texture);
	if (isHover) {
		int padding = 1;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 220);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 10, 250, 170, 220);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
			SDL_SetRenderDrawColor(r, 0, 0, 0, sartingAlpha - i * 4);
		}
	}else {
		int padding = 1;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
		}
		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	}
	m_BtnText->render(0.f);
	SDL_SetRenderTarget(r, NULL);
}

void Button::PorcessEvents(SDL_Event& e)
{
	Base::PorcessEvents(e);
}

void Button::setPos(const Vec2i& pos)
{
	m_WidgetBounderies.x = pos.x;
	m_WidgetBounderies.y = pos.y;
	m_Pos = pos;
}

Button::~Button()
{
	//setVisible(false);
	SDL_DestroyTexture(widget_texture);
	FREE(m_BtnText);
	//UI::Manager::getEventManager()->BlockEvents(false);
}