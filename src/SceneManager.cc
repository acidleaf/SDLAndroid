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
void SceneManager::replace(IScene* scene, bool transition) {
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
	
	_nextScene = scene;
	if (_nextScene->needsPreload()) {
		// Lets not deal with preloading for now
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
		if (!_nextScene->init()) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error initializing scene!");
			return;
		}
		
		// Fade out to next scene
		if (transition) fadeOut();
		else {
			// No transition
			_scenes[_curScene]->release();
			_scenes[_curScene] = _nextScene;
			_nextScene = nullptr;
		}
	}
}

void SceneManager::push(IScene* scene, bool transition) {
	if (_transitioning || _preloading) return;
	_type = TRANSITION_PUSH;
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
		// Lets not deal with preloading for now
		// ...
		
	} else {
		//SDL_assert_release(scene->init());		// Let it crash if scene failes to initialize?
		if (!scene->init()) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error initializing scene!");
			return;
		}
		
		// Fade out to next scene
		// Fade out to next scene
		if (transition) fadeOut();
		else {
			// No transition
			SDL_assert_release(_scenes[_curScene + 1] == nullptr);	// Must be nullptr
			SDL_assert_release(_curScene + 1 < MAX_SCENES);				// Must not exceed stack size too
			++_curScene;
			_scenes[_curScene] = _nextScene;
		}
	}
	
}

void SceneManager::pop(bool transition) {
	if (_transitioning || _preloading) return;
	if (_curScene <= 0) {
		SDL_Log("No scenes to pop!");
		return;
	}
	
	_type = TRANSITION_POP;
	
	if (transition) {
		_nextScene = _scenes[_curScene - 1];
		fadeOut();
	} else {
		SDL_assert_release(_curScene > 0);
		_scenes[_curScene]->release();
		_scenes[_curScene] = nullptr;
		--_curScene;
	}
	
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

void SceneManager::fadeOut() {
	_transitioning = true;
	_fadeRect.alpha() = 0.0f;
	_fadeTween.from(0.0f);
	_fadeTween.to(1.0f);
	_fadeTween.start();
}
void SceneManager::fadeIn() {
	_transitioning = true;
	_fadeRect.alpha() = 1.0f;
	_fadeTween.from(1.0f);
	_fadeTween.to(0.0f);
	_fadeTween.start();
}

void SceneManager::fadeComplete(Tween* tween, void* data) {
	SceneManager* ref = (SceneManager*)data;
	
	if (ref->_nextScene) {
		// Next scene is set, replace current scene
		
		if (ref->_type == TRANSITION_REPLACE) {
			// Release current scene as it's a replacement
			ref->_scenes[ref->_curScene]->release();
			ref->_scenes[ref->_curScene] = ref->_nextScene;
			
		} else if (ref->_type == TRANSITION_PUSH) {
			SDL_assert_release(ref->_scenes[ref->_curScene + 1] == nullptr);	// Must be nullptr
			SDL_assert_release(ref->_curScene + 1 < MAX_SCENES);				// Must not exceed stack size too
			++ref->_curScene;
			ref->_scenes[ref->_curScene] = ref->_nextScene;
			
		} else if (ref->_type == TRANSITION_POP) {
			SDL_assert_release(ref->_curScene > 0);
			ref->_scenes[ref->_curScene]->release();
			ref->_scenes[ref->_curScene] = nullptr;
			--ref->_curScene;
		}
		
		ref->_nextScene = nullptr;
		
		// Fade in to scene
		ref->fadeIn();
		
	} else {
		// Must have completed a scene in transition, mark transitioning done
		ref->_transitioning = false;
		ref->_type = TRANSITION_NONE;
	}
}