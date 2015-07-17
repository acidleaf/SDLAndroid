#include "App.h"
#include "Timer.h"

static App* _appInstance = nullptr;

App*& App::getInstance() {
	if (!_appInstance) _appInstance = new App();
	return _appInstance;
}

bool App::init(int pixelScale) {
	_pixelScale = pixelScale;
	
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	
	
#ifdef __ANDROID__
	// Get display size since we're on mobile
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
		_resX = displayMode.w;
		_resY = displayMode.h;
		
	} else {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error retrieving display mode: %s", SDL_GetError());
	}
#else
	_resX = 360;
	_resY = 640;
#endif
	
	
	// Init window
	_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _resX, _resY, 0);
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
	
#ifdef __ANDROID__
	// Set the pixel size
	_resX /= _pixelScale;
	_resY /= _pixelScale;
	SDL_RenderSetLogicalSize(_renderer, _resX, _resY);
#endif
	
	// Setup event filters to handle entering background
	SDL_SetEventFilter(eventFilter, this);
	
	// Use nearest neighbor filtering
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	
	SDL_version v;
	SDL_GetVersion(&v);
	SDL_Log("SDL version: %d.%d.%d\n", v.major, v.minor, v.patch);
	
	
	if (!_fonts.init()) return false;
	_defaultFont = _fonts.getFont("04b03.ttf", 8);
	if (!_scene.init()) return false;
	_scene.push(_scene.mainScene(), false);
	
	
	
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
		
		
	}
	
	//float t = float(curTime + SKIP_TICKS - _nextTick) / (float)SKIP_TICKS;
	render();
	_fps.update();
}

void App::render() {
	SDL_RenderClear(_renderer);
	
	// Render stuff here
	_scene.render();
	
	// Render FPS in bottom left
	_fonts.writeLine(_defaultFont, 2, _resY - 2, "%.1f", _fps());
	
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