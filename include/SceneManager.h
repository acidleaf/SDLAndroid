#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "pch.hpp"
#include "Sprite.h"
#include "Tween.h"

enum TransitionType {
	TRANSITION_REPLACE,
	TRANSITION_PUSH
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
	
	
	static void fadeComplete(Tween* tween, void* data);
	
public:
	
	bool init();
	void release();
	void update();
	void render();
	void handleEvents(const SDL_Event& e);
	
	void replace(IScene* scene);
	void push(IScene* scene);
	void pop();
	
	IScene* activeScene();
};

#endif