#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "pch.hpp"
#include "Sprite.h"
#include "Tween.h"

// Major scenes should be here
#include "MainMenu.h"
#include "MainScene.h"

enum TransitionType {
	TRANSITION_NONE,
	TRANSITION_REPLACE,
	TRANSITION_PUSH,
	TRANSITION_POP
};

class IScene;
class SceneManager {
protected:
	static const int MAX_SCENES = 8;
	IScene* _scenes[MAX_SCENES];
	IScene* _nextScene = nullptr;
	int _curScene = -1;
	
	TransitionType _type;
	
	Sprite _fadeRect;
	Tween _fadeTween;
	
	bool _preloading = false;
	bool _transitioning = false;
	
	// Scenes
	MainMenu _mainMenu;
	MainScene _mainScene;
	
	
	
	void fadeOut();
	void fadeIn();
	static void fadeComplete(Tween* tween, void* data);
	
public:
	
	bool init();
	void release();
	void update();
	void render();
	void handleEvents(const SDL_Event& e);
	
	void replace(IScene* scene, bool transition = true);
	void push(IScene* scene, bool transition = true);
	void pop(bool transition = true);
	
	IScene* activeScene();
	
	MainMenu* mainMenu() { return &_mainMenu; }
	MainScene* mainScene() { return &_mainScene; }
};

#endif