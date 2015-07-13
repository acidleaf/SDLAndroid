#include "App.h"
#include "Timer.h"

static App* _appInstance = nullptr;

App*& App::getInstance() {
	if (!_appInstance) _appInstance = new App();
	return _appInstance;
}

bool App::init(const char* title) {
	
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	
	// Get display size since we're on mobile
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
		_resX = displayMode.w;
		_resY = displayMode.h;
	} else {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error retrieving display mode: %s", SDL_GetError());
	}
	
	
	
	// Init window
	_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _resX, _resY, 0);
	if (!_window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating window: %s\n", SDL_GetError());
		return false;
	}
	// Init renderer
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating renderer: %s\n", SDL_GetError());
		return false;
	}
	
	// Setup event filters to handle entering background
	SDL_SetEventFilter(eventFilter, this);
	
	
	SDL_version v;
	SDL_GetVersion(&v);
	SDL_Log("SDL version: %d.%d.%d\n", v.major, v.minor, v.patch);
	
	
	if (!_scene.init()) return false;
	_scene.push(&_scene1, false);
	
	
	return true;
}

void App::release() {
	_scene.release();
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}




void App::handleEvents() {
	if (_paused) return;
	
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) _done = true;
		
		_scene.handleEvents(e);
		
		if (e.type == SDL_FINGERDOWN) {
			if (_scene.activeScene() == &_scene1) _scene.push(&_scene2);
			//else if (_scene.activeScene() == &_scene2) _scene.replace(&_scene1);
		} else if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_AC_BACK) {
				_scene.pop();
			}
		}
	}
}



void App::update() {
	if (_paused) return;
	
	const uint32_t curTime = Timer::getTimeMs();
	int loops = 0;
	
	
	while(curTime > _nextTick && loops < MAX_FRAMESKIP) {
		// Update stuff here
		_scene.update();
		
		_nextTick += SKIP_TICKS;
		++loops;
		
		_fps.update();
	}
	
	//float t = float(curTime + SKIP_TICKS - _nextTick) / (float)SKIP_TICKS;
	render();
}

void App::render() {
	SDL_RenderClear(_renderer);
	
	// Render stuff here
	_scene.render();
	
	SDL_RenderPresent(_renderer);
}


int App::eventFilter(void* ptr, SDL_Event* e) {
	App* app = (App*)ptr;
	if (e->type == SDL_APP_DIDENTERBACKGROUND) {
		app->_paused = true;
	} else if (e->type == SDL_APP_DIDENTERFOREGROUND) {
		app->_paused = false;
		app->_nextTick = Timer::getTimeMs();
	}
	return 1;
}