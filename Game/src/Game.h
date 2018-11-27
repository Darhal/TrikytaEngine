#pragma once
#include "core/Common/EngineInstance.h"

struct ENGINE_CONFIG;

class Game : public EngineInstance
{
public:
	Game(ENGINE_CONFIG& p_Config) :
		EngineInstance(p_Config)
	{}

	void On_Engine_Pre_Init() override;
	void On_Engine_Init() override;
	void On_Engine_Render(float) override;
	void On_Engine_Quit() override;

	void On_Input(int, unsigned int);
	void OnCollision(class b2Contact*);
	void OnCollisionEnd(class b2Contact*);
	void OnClick(unsigned int, int, Vec2i);
	void OnMouseMove(Vec2i);

	class Sprite* obj;
	class AnimationSet* anim;
	class UI::Text* text;
	class Physics2D::PhysicsBody* body;
	class Physics2D::PhysicsBody* body2;
	class TiledMap* map;
};