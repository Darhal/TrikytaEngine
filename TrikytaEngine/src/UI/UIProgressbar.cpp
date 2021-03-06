#include "UIProgressbar.hpp"
#include "UIManager.h"
#include "UIText.h"
#include <core/Common/TrikytaEngine.h>
#include "misc/Font.h"
#include "Events/UIEventHandler.h"

using namespace UI;

Progressbar::Progressbar(const Vec2i& m_Pos, const Vec2i& m_Size, const Color& inner_color, const Color& bar_color,
					const std::string& text, const Color& text_color, Font* font) : 
	m_Pos(m_Pos), m_Size(m_Size), widgetColor(inner_color), progressbarColor(bar_color), m_Progress(0)
{
	Manager::addElement(this, true);
	auto r = ENGINE->getRenderer();
	m_WidgetBounderies = SDL_Rect{ m_Pos.x, m_Pos.y, m_Size.x, m_Size.y };
	widget_texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidgetBounderies.w, m_WidgetBounderies.h); 
	if (text != "") {
		m_ProgressbarText = Text::createText(text, font != nullptr ? font : Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 16), Vec2i(0, 0), text_color, false);
		m_ProgressbarText->setPosition((m_Size - m_ProgressbarText->getSize()) / 2);
	}else {
		m_ProgressbarText = nullptr;
	}
	Progressbar::buildWidget();
}

void Progressbar::buildWidget()
{
	int padding = 1;
	auto r = ENGINE->getRenderer();
	auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
	SDL_SetRenderTarget(r, widget_texture);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, widgetColor.r, widgetColor.g, widgetColor.b, widgetColor.a);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	for (int i = padding; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
	}
	if (m_ProgressbarText != nullptr) {
		m_ProgressbarText->render(0.f);
	}
	SDL_SetRenderTarget(r, NULL);
}

void Progressbar::render(float dt)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(widget_texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(r, widget_texture, NULL, &m_WidgetBounderies);
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}

void Progressbar::setProgress(int p)
{
	p = (p < 0) ? 0 : (p > 100) ? 100 : p; // p is clamped between 0 and 100
	if (m_Progress == p) {
		return;
	}
	int padding = 1;
	auto r = ENGINE->getRenderer();
	auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
	SDL_SetRenderTarget(r, widget_texture);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, widgetColor.r, widgetColor.g, widgetColor.b, widgetColor.a);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	for (int i = padding; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
	}

	SDL_SetRenderDrawColor(r, progressbarColor.r, progressbarColor.g, progressbarColor.b, progressbarColor.a);
	widgetBounderies = SDL_Rect{padding, padding, int((float(p)/100.f)*(m_Size.x - padding * 2)),  m_Size.y - padding * 2 };
	SDL_RenderFillRect(r, &widgetBounderies);

	if (m_ProgressbarText != nullptr) {
		m_ProgressbarText->render(0.f);
	}
	SDL_SetRenderTarget(r, NULL);
	m_Progress = p;
}

//TODO: fix it with setting the max to 100 and divide it till it hit progress in the param
void Progressbar::setProgress(int p, Color&& starting_color, Color&& ending_color, int step)
{
	p = (p < 0) ? 0 : (p > 100) ? 100 : p; // p is clamped between 0 and 100
	if (m_Progress == p) {
		return;
	}
	int padding = 1;
	auto r = ENGINE->getRenderer();
	auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
	SDL_SetRenderTarget(r, widget_texture);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, widgetColor.r, widgetColor.g, widgetColor.b, widgetColor.a);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	for (int i = padding; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
	}
	
	if (p != 0) {
		bool isFirstColor = true;
		widgetBounderies = SDL_Rect{ padding, padding, 0,  m_Size.y - padding * 2 };
		int max = int((float(p) / 100.f)*(m_Size.x - padding * 2));
		int step_adv = int((float(step) / 100.f)*(m_Size.x - padding * 2));
		while ((widgetBounderies.x + widgetBounderies.w) - step_adv < max) {
			widgetBounderies.w = step_adv;
			isFirstColor ? SDL_SetRenderDrawColor(r, starting_color.r, starting_color.g, starting_color.b, starting_color.a) :
				SDL_SetRenderDrawColor(r, ending_color.r, ending_color.g, ending_color.b, ending_color.a);
			SDL_RenderFillRect(r, &widgetBounderies);
			isFirstColor = !isFirstColor;
			widgetBounderies.x += widgetBounderies.w;
		}
	}
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	if (m_ProgressbarText != nullptr) {
		m_ProgressbarText->render(0.f);
	}
	SDL_SetRenderTarget(r, NULL);
	m_Progress = p;
}

Progressbar::~Progressbar()
{
	SDL_DestroyTexture(widget_texture);
	widget_texture = nullptr;
	if (m_ProgressbarText != nullptr) {
		FREE(m_ProgressbarText);
	}
	//UI::Manager::getEventManager()->BlockEvents(false);
}