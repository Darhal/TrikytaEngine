#include <core/Objects/ObjectHandler.h>
#include <core/Utility/TimerManager.h>
#include <core/Common/defines.h>
#include <core/Drawable/Sprite.h>
#include <core/Common/Vec2.h>
#include <core/Drawable/Animation.h>
#include <core/Physics/PhysicsEngine.h>
#include <core/Events/EventManager.h>
#include <core/TiledMap/TiledMap.h>
#include "Game.h"
#include <iostream>
#include <UI/UIText.h>
#include <misc/Console/Console.h>
#include <UI/UIEditBox.h>
#include <core/Camera/Camera.h>
#include <core/Physics/Joints.h>
#include <core/TiledMap/Tilesets.h>

//UI::EditBox* editBox;
Camera* cam;
bool anti_spam = false;

void Game::On_Engine_Pre_Init()  
{ 
	LogTerminal("Trikyta Engine Pre-initializing. ");
};

void Game::On_Engine_Init()
{
	cam = Camera::CreateCamera();
	map = TiledMap::Create("assets/example/maps/map/map.tmx");
	//map = TiledMap::Create("assets/example/maps/map3.tmx");
	cam->addObjectToCamera(map);
	anim = Animation::Create("assets/anim_pack.png", "assets/anim_pack.a", Vec2i(256/7, 217/7), Vec2i(ENGINE->GetScreenWidth() / 2, (ENGINE->GetScreenHeight() / 2)-500), 0.03f);
	body = anim->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
	cam->addObjectToCamera(anim);
	anim->ToggleRotationAttachement(false);
	body->SetAngularDamping(1000.f);

	
	/*auto zombie_boy = Animation::Create("assets/chars/zombie.png", "assets/chars/zombie_attack.txt", Vec2i(430/10, 519/10), Vec2i(550, 200), 0.04f);
	zombie_boy->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.f))->SetAngularDamping(1000.f);
	zombie_boy->ToggleRotationAttachement(false);
	auto zombie_girl = Animation::Create("assets/chars/fzombie_female.png", "assets/chars/fzombie_attack.txt", Vec2i(521/10, 576 /10), Vec2i(250, 200), 0.04f);
	zombie_girl->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.f))->SetAngularDamping(1000.f);
	zombie_girl->ToggleRotationAttachement(false);

	anim = Animation::Create("assets/chars/plane.png", "assets/chars/plane_shoot.txt", Vec2i(443 / 4, 302 / 4), Vec2i(250, 120), 0.04f);
	body = anim->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::BOX, Vec2f(0.f, 0.f));
	anim->ToggleRotationAttachement(false);
	body->SetAngularDamping(1000.f);
	cam->addObjectToCamera(zombie_girl);
	cam->addObjectToCamera(zombie_boy);
	cam->addObjectToCamera(anim);*/
	//EVENT TESTING!!
	EventManager::GetEventManager()->addEventHandler<Events::ON_KEYBOARD_INPUT>(CALLBACK_2(Game::On_Input, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_START>(CALLBACK_1(Game::OnCollision, this));
	/*EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_END>(CALLBACK_1(Game::OnCollisionEnd, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_CLICK>(CALLBACK_3(Game::OnClick, this));*/
	//EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_MOVE>(CALLBACK_1(Game::OnMouseMove, this));
};


void Game::On_Engine_Render(float /*dt*/)
{
	// Offset the player quad by the camera position 
	bool b = true;
	Vec2i pos = Vec2i((int)body->GetPosition().x, (int)body->GetPosition().y) - Vec2i(cam->getCameraSize().x/2, cam->getCameraSize().y/2);
	if (b)
		cam->setCameraPosition(pos);
};

void Game::On_Input(SDL_Keycode p_Key, unsigned int p_KeyState)
{ 
	if (p_KeyState == SDL_KEYDOWN) {
		if (p_Key == SDLK_RIGHT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = 50.f;
			body->SetLinearVelocity(v);
			anim->Flip(FLIPTYPE::NONE);
		}else if (p_Key == SDLK_LEFT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = -50.f;
			body->SetLinearVelocity(v);
			anim->Flip(FLIPTYPE::HORIZONTAL);
		}else if (p_Key == SDLK_UP) {
			//body->ApplyLinearImpulse(Vec2f(0, body->GetMass() * -1000.f), body->GetWorldCenter(), false);
			Vec2f v = body->GetLinearVelocity();
			v.y = -50.f;
			body->SetLinearVelocity(v);
		}else if (p_Key == SDLK_DOWN) {
			Vec2f v = body->GetLinearVelocity();
			v.y = 50.f;
			body->SetLinearVelocity(v);
			
		}else if (p_Key == SDLK_c) {

		}
	}
};

void Game::On_Engine_Quit() 
{ 
	EngineInstance::On_Engine_Quit();
	Log("Leaving game! "); 
};

void Game::OnCollision(b2Contact* contact)
{
	/*auto bodyA = contact->GetFixtureA()->GetPhysicsBody();
	auto bodyB = contact->GetFixtureB()->GetPhysicsBody();
	auto coinbody = map->getGroupManager().getBodiesByName("test");
	bool isACoin = map->getGroupManager().isBodyInLayer(bodyA, "coins") || map->getGroupManager().isBodyInLayer(bodyB, "coins");
	if (isACoin) {
		if (bodyB == body) {
			LogConsole(LogWarning, "Contact with a coin !");
		}
		else if (bodyA == body) {
			LogConsole(LogWarning, "Contact with a coin !");
		}
	}*/
	auto bodyA = contact->GetFixtureA()->GetPhysicsBody();
	auto bodyB = contact->GetFixtureB()->GetPhysicsBody();
	bool isACoin = map->isBodyPartOfTileset(bodyA, "misc2", 3) || map->isBodyPartOfTileset(bodyB, "misc2", 3);
	bool isARedSwitch = map->isBodyPartOfTileset(bodyA, "misc2", 2) || map->isBodyPartOfTileset(bodyB, "misc2", 2);
	bool isAGreenSwitch = map->isBodyPartOfTileset(bodyA, "misc2", 1) || map->isBodyPartOfTileset(bodyB, "misc2", 1);
	
	bool isAHeart = map -> isBodyPartOfTileset (bodyA ,"misc2", 5) || map -> isBodyPartOfTileset(bodyB , "misc2" , 5);
	bool isAKey = map -> isBodyPartOfTileset (bodyA ,"sheet_key", 0) || map -> isBodyPartOfTileset(bodyB , "sheet_key" , 0);
	
	int compteurHeart = 0 ; int compteurKey = 0 ;
	
	if (isACoin) {
		if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				LogConsole(LogWarning, "Contact with a coin !");
			}
		}else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				LogConsole(LogWarning, "Contact with a coin !");
			}
		}
	}else if (isARedSwitch && !anti_spam) {
		if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 1, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a red switch !");
				anti_spam = true;
				TimerManager::CreateTimer([]() {anti_spam = false; }, 3000, 1, true);
			}
		}else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 1, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a red switch !");
				anti_spam = true;
				TimerManager::CreateTimer([]() {anti_spam = false; }, 3000, 1, true);
			}
		}
	}else if (isAGreenSwitch && !anti_spam) {
		if (bodyB == body) {
			LayerData* tileToDelete = bodyA -> getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map -> getTilset("misc2"), 2, "switch", tileToDelete -> tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a green switch !");
				anti_spam = true;
				TimerManager::CreateTimer([]() {anti_spam = false; }, 3000, 1, true);
			}
		}else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 2, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a green switch !");
				anti_spam = true;
				TimerManager::CreateTimer([]() {anti_spam = false; }, 3000, 1, true);
			}
		}
	}else if(isAHeart){
	    if (bodyA == body){
		LayerData* tileToDelete = bodyA -> getComponent<LayerData>();
		if (tileToDelete != NULL) {
			map->deleteTileInLayer(tileToDelete);
			LogConsole(LogWarning, "Contact with a heart !");
		}
	     }else if (bodyB == body) {
		  LayerData* tileToDelete = bodyA->getComponent<LayerData>();
		      if (tileToDelete != NULL) {
			map->deleteTileInLayer(tileToDelete);
			LogConsole(LogWarning, "Contact with a heart !");
		      }
		}
	}else if(isAKey){
	    if (bodyB == body){
		LayerData* tileToDelete = bodyA -> getComponent<LayerData>();
		if (tileToDelete != NULL) {
			map->deleteTileInLayer(tileToDelete);
			LogConsole(LogWarning, "Contact with a key !");
			if (compteurKey <= 3){ 
			  compteurKey++ ; LogConsole(LogWarning, "Incrémentation!"); 
			  
			}
		}
		}else if (bodyA == body) {
		  LayerData* tileToDelete = bodyA->getComponent<LayerData>();
		      if (tileToDelete != NULL) {
			map->deleteTileInLayer(tileToDelete);
			LogConsole(LogWarning, "Contact with a key !");
			if (compteurKey <= 3){ 
			  compteurKey++ ; LogConsole(LogWarning , "Incrémentation!");
			  
			}
		      }
		}
	}
	  
		    
	    
}

void Game::OnCollisionEnd(b2Contact* contact)
{
	Log("TWO ENDED BODIED COLLISION!!")
}

void Game::OnClick(unsigned int EventType, int button, Vec2i pos)
{
	if (EventType == SDL_MOUSEBUTTONDOWN && button == SDL_BUTTON_RIGHT) {
		Log("CLICK : (%d, %d)", pos.x, pos.y)
	}
}

void Game::OnMouseMove(Vec2i pos)
{
	Log("Mouse moved (%d, %d)", pos.x, pos.y);
}