#include "SceneManager.h"
#include "IScene.h"
#include "App.h"

bool SceneManager::init() {
	auto app = App::getInstance();
	
	memset(_scenes, 0, sizeof(IScene*));
	_curScene = -1;
	
	uint8_t* data = new uint8_t[app->resX() * app->resY()];
	if (!_fadeRect.init(data, app->resX(), app->resY(), 1)) {
		delete[] data;
		return false;
	}
	_fadeRect.alpha() = 0.0f;
	if (data) delete[] data;
	
	_fadeTween = Tween(&_fadeRect.alpha(), 1.0f, 0.25f);
	_fadeTween.onComplete(fadeComplete, this);
	
	return true;
}

void SceneManager::release() {
	_fadeRect.release();
	while (_curScene >= 0) {
		_scenes[_curScene]->release();
		_scenes[_curScene] = nullptr;
		--_curScene;
	}
}
void SceneManager::replace(IScene* scene) {
	if (_transitioning || _preloading) return;
	
	_type = TRANSITION_REPLACE;
	
	if (_curScene < 0) {
		if (!scene->init()) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error initializing scene!");
			return;
		}
		_curScene = 0;
		_scenes[_curScene] = scene;
		return;
	}
	
	// Store the pointer first
	_nextScene = scene;
	
	if (scene->needsPreload()) {
		_preloading = true;
		
		// First release current
		_scenes[_curScene]->release();
		
		// Spawn new thread to preload
		// ...
		
		// Show loading screen
		// ...
		
	} else {
		// Preloading not requested, must be quick
		//SDL_assert_release(scene->init());		// Let it crash if scene failes to initialize?
		if (!scene->init()) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error initializing scene!");
			return;
		}
		
		// Fade out to next scene
		_fadeRect.alpha() = 0.0f;
		_transitioning = true;
		_fadeTween.from(0.0f);
		_fadeTween.to(1.0f);
		_fadeTween.start();
	}
}

void SceneManager::push(IScene* scene) {
	
}

void SceneManager::pop() {
	
}

IScene* SceneManager::activeScene() {
	if (_curScene < 0) return nullptr;
	return _scenes[_curScene];
}

void SceneManager::update() {
	if (_transitioning) {
		_fadeTween.update();
		return;
	}
	
	// Update normally here
	if (_curScene < 0 || !_scenes[_curScene]) return;
	_scenes[_curScene]->update();
}

void SceneManager::render() {
	if (_curScene >= 0 && _scenes[_curScene]) _scenes[_curScene]->render();
	
	if (_transitioning) {
		_fadeRect.render();
		return;
	}
}

void SceneManager::handleEvents(const SDL_Event& e) {
	if (_curScene < 0 || !_scenes[_curScene]) return;
	_scenes[_curScene]->handleEvents(e);
}



void SceneManager::fadeComplete(Tween* tween, void* data) {
	SceneManager* ref = (SceneManager*)data;
	
	if (ref->_nextScene) {
		// Next scene is set, replace current scene
		ref->_scenes[ref->_curScene]->release();
		if (ref->_type == TRANSITION_REPLACE) {
			ref->_scenes[ref->_curScene] = ref->_nextScene;
			SDL_Log("replacing scene");
		} else if (ref->_type == TRANSITION_PUSH) {
			ref->_scenes[ref->_curScene] = nullptr;
			--ref->_curScene;
			ref->_scenes[ref->_curScene] = ref->_nextScene;
		}
		
		ref->_nextScene = nullptr;
		
		// Fade in to scene
		ref->_fadeRect.alpha() = 1.0f;
		ref->_fadeTween.from(1.0f);
		ref->_fadeTween.to(0.0f);
		ref->_fadeTween.start();
	} else {
		// Must have completed a scene in transition, mark transitioning done
		ref->_transitioning = false;
		SDL_Log("transition complete");
	}
}