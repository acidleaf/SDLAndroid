#ifndef __TWEEN_H__
#define __TWEEN_H__

#include "pch.hpp"
#include "Timer.h"
#include "EaseFunc.h"


class Tween {
protected:
	typedef float (*EaseFunc)(float t, float d, float b, float c);
	typedef void (*TweenCallback)(Tween* tween, void* data);
	
	float* _prop = nullptr;
	float _b, _c;
	float _start = 0;
	float _duration = 0.0;
	float _delay = 0.0f;
	int _repeat = 0;
	
	EaseFunc _ease = Ease::EASE_LINEAR;
	
	TweenCallback _onComplete = nullptr;
	void* _data = nullptr;
	
	bool _reverse = false;
	bool _done = true;
	
	void reverse() {
		_c = -_c;
		_b = *_prop;
	}
	
public:
	Tween() {}
	Tween(float* prop, float to, double duration, float delay = 0.0f, int repeat = 0, bool reverse = false) :
		_prop(prop), _b(*_prop), _c(to - *_prop),
		_duration(duration), _delay(delay), _repeat(repeat), _reverse(reverse)
		{}
	
	
	void update() {
		if (!_prop || (_done && !_repeat)) return;
		
		const float curTime = Timer::getTime();
		if (curTime < _start + _delay) return;
		
		float dt = curTime - _start - _delay;
		if (dt > _duration + _delay) {
			dt = _duration + _delay;
			_done = true;
		}
		if (dt <= _duration) *_prop = _ease(dt, _duration, _b, _c);
		
		if (_done) {
			if (_repeat == 0) {
				if (_onComplete) _onComplete(this, _data);
				return;
			} else {
				if (_repeat > 0) --_repeat;
				if (!_reverse) reset();
				else reverse();
				
				_start = curTime;
				_done = false;
			}
		}
	}
	
	void start(float delay = 0.0f) {
		_done = false;
		_delay = delay;
		_start = Timer::getTime();
	}
	void reset() { *_prop = _b; }
	void restart() { *_prop = _b; start(); }
	
	// Setters
	void from(float from) { _b = from; }
	void to(float to) { _c = to - _b; }
	void ease(EaseFunc func) { _ease = func; }
	void duration(float duration) { _duration = duration; }
	void onComplete(TweenCallback callback, void* data) { _onComplete = callback; _data = data; }
	
	
	// Getters
	const float state() const { return *_prop; }
};


#endif