#ifndef __ISCENE_H__
#define __ISCENE_H__

#include "pch.hpp"

class IScene {
public:
	virtual ~IScene() {}
	virtual bool init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void handleEvents(const SDL_Event& e) = 0;
	
	virtual bool needsPreload() const { return false; }
	virtual bool stopsUpdate() const { return true; }
	virtual bool stopsRender() const { return true; }
};


#endif