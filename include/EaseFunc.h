#ifndef __EASEFUNC_H__
#define __EASEFUNC_H__

#include <glm/gtc/constants.hpp>

// -------------------- Easing functions --------------------
// http://robertpenner.com/easing/
class Ease {
public:
	static float EASE_LINEAR(float t, float d, float b, float c) { return c * t / d + b; }
	
	static float EASE_IN_QUAD(float t, float d, float b, float c) { return c * glm::pow(t / d, 2.0f) + b; }
	static float EASE_OUT_QUAD(float t, float d, float b, float c) { return c * (1.0f - glm::pow(1.0f - t / d, 2.0f)) + b; }
	static float EASE_INOUT_QUAD(float t, float d, float b, float c) {
		if ((t /= d / 2.0f) < 1.0f) return c / 2.0f * glm::pow(t, 2.0f) + b;
		--t; return -c / 2.0f * (((t - 2) * t) - 1) + b;
	}

	static float EASE_IN_CUBIC(float t, float d, float b, float c) { return c * glm::pow(t / d, 3.0f) + b; }
	static float EASE_OUT_CUBIC(float t, float d, float b, float c) { return c * (glm::pow(t / d - 1.0f, 3.0f) + 1.0f) + b; }
	static float EASE_INOUT_CUBIC(float t, float d, float b, float c) {
		if ((t /= d / 2.0f) < 1.0f) return c / 2.0f * glm::pow(t, 3.0f) + b;
		t -= 2.0f; return c / 2.0f * (glm::pow(t, 3.0f) + 2.0f) + b;
	}

	static float EASE_IN_QUART(float t, float d, float b, float c) { return c * glm::pow(t / d, 4.0f) + b; }
	static float EASE_OUT_QUART(float t, float d, float b, float c) { return -c * (glm::pow(t / d - 1.0f, 4.0f) - 1.0f) + b; }
	static float EASE_INOUT_QUART(float t, float d, float b, float c) {
		if ((t /= d / 2.0f) < 1.0f) return c / 2.0f * glm::pow(t, 4.0f) + b;
		t -= 2.0f; return -c / 2.0f * (glm::pow(t, 4.0f) - 2.0f) + b;
	}

	static float EASE_IN_QUINT(float t, float d, float b, float c) { return c * glm::pow(t / d, 5.0f) + b; }
	static float EASE_OUT_QUINT(float t, float d, float b, float c) { return c * (glm::pow(t / d - 1.0f, 5.0f) + 1.0f) + b; }
	static float EASE_INOUT_QUINT(float t, float d, float b, float c) {
		if ((t /= d / 2.0f) < 1.0f) return c / 2.0f * glm::pow(t, 5.0f) + b;
		t -= 2.0f; return c / 2.0f * (glm::pow(t, 5.0f) + 2.0f) + b;
	}

	static float EASE_IN_SINE(float t, float d, float b, float c) { return -c * glm::cos(t / d * glm::half_pi<float>()) + c + b; }
	static float EASE_OUT_SINE(float t, float d, float b, float c) { return c * glm::sin(t / d * glm::half_pi<float>()) + b; }
	static float EASE_INOUT_SINE(float t, float d, float b, float c) { return -c / 2.0f * (glm::cos(glm::pi<float>() * t / d) - 1.0f) + b; }

	static float EASE_IN_EXPO(float t, float d, float b, float c) { return (t == 0.0f) ? b : c * glm::pow(2.0f, 10.0f * (t / d - 1.0f)) + b; }
	static float EASE_OUT_EXPO(float t, float d, float b, float c) { return (t == d) ? b + c : c * (-glm::pow(2.0f, -10.0f * t / d) + 1.0f) + b; }
	static float EASE_INOUT_EXPO(float t, float d, float b, float c) {
		if (t == 0) return b;
		if (t == d) return b + c;
		if ((t /= d / 2.0f) < 1.0f) return c / 2.0f * glm::pow(2.0f, 10.0f * (t - 1.0f)) + b;
		--t; return c / 2.0f * (-glm::pow(2.0f, -10.0f * t) + 2.0f) + b;
	}

	static float EASE_IN_CIRC(float t, float d, float b, float c) { return -c * (glm::sqrt(1.0f - glm::pow(t / d, 2.0f)) - 1.0f) + b; }
	static float EASE_OUT_CIRC(float t, float d, float b, float c) { return c * glm::sqrt(1.0f - glm::pow(t / d - 1.0f, 2.0f)) + b; }
	static float EASE_INOUT_CIRC(float t, float d, float b, float c) {
		if ((t /= d / 2.0f) < 1.0f) return -c / 2.0f * (glm::sqrt(1.0f - t * t) - 1.0f) + b;
		t -= 2.0f; return c / 2.0f * (glm::sqrt(1.0f - t * t) + 1.0f) + b;
	}

	// These were taken from https://github.com/jesusgollonet/ofpennereasing/blob/master/PennerEasing/Elastic.cpp
	static float EASE_IN_ELASTIC(float t, float d, float b, float c) {
		if (t == 0.0f) return b;
		if ((t /= d) == 1.0f) return b + c;
		float p = d * 0.4f;
		float s = p / 4.0f;
		float a = c;
		float postFix = a * glm::pow(2.0f, 10.0f * --t);
		return -(postFix * glm::sin((t * d - s) * glm::two_pi<float>() / p)) + b;
	}
	static float EASE_OUT_ELASTIC(float t, float d, float b, float c) {
		if (t == 0.0f) return b;
		if ((t /= d) == 1.0f) return b + c;
		float p = d * 0.4f;
		float s = p / 4.0f;
		float a = c;
		return (a * glm::pow(2.0f, -10.0f * t) * glm::sin((t * d - s) * glm::two_pi<float>() / p) + c + b);
	}
	static float EASE_INOUT_ELASTIC(float t, float d, float b, float c) {
		if (t == 0.0f) return b;
		if (( t /= d / 2.0f) == 2.0f) return b + c;
		float p = d * (0.4f * 1.5f);
		float s = p / 4.0f;
		float a = c;
		if (t < 1.0f) {
			float postFix = a * glm::pow(2.0f, 10.0f * --t);
			return -0.5f * (postFix * glm::sin((t * d - s) * glm::two_pi<float>() / p)) + b;
		} 
		float postFix = a * glm::pow(2.0f, -10.0f * --t);
		return postFix * glm::sin((t * d - s) * glm::two_pi<float>() / p) * 0.5f + c + b;
	}

	static float EASE_IN_BACK(float t, float d, float b, float c) { float s = 1.70158f; t /= d; return c * t * t * ((s + 1.0f) * t - s) + b; }
	static float EASE_OUT_BACK(float t, float d, float b, float c) { float s = 1.70158f; t = t / d - 1.0f; return c * (t * t * ((s + 1.0f) * t + s) + 1.0f) + b; }
	static float EASE_INOUT_BACK(float t, float d, float b, float c) {
		float s = 1.70158f * 1.525f;
		if ((t /= d / 2.0f) < 1.0f) return c / 2.0f * (t * t * ((s + 1.0f) * t - s)) + b;
		float postFix = t -= 2.0f;
		return c / 2.0f* (postFix * t * ((s + 1.0f) * t + s) + 2.0f) + b;
	}


	static float EASE_OUT_BOUNCE(float t, float d, float b, float c) {
		if ((t /= d) < (1.0f / 2.75f)) {
			return c * (7.5625f * t * t) + b;
		} else if (t < (2.0f / 2.75f)) {
			float postFix = t -= (1.5f / 2.75f);
			return c * (7.5625f * postFix * t + 0.75f) + b;
		} else if (t < (2.5f / 2.75f)) {
			float postFix = t -= (2.25f / 2.75f);
			return c * (7.5625f * postFix * t + 0.9375f) + b;
		} else {
			float postFix = t -= (2.625f / 2.75f);
			return c * (7.5625f * postFix * t + 0.984375f) + b;
		}
	}
	static float EASE_IN_BOUNCE(float t, float d, float b, float c) { return c - EASE_OUT_BOUNCE(d - t, d, 0.0f, c) + b; }
	static float EASE_INOUT_BOUNCE(float t, float d, float b, float c) {
		if (t < d / 2.0f) return EASE_IN_BOUNCE(t * 2.0f, d, 0.0f, c) * 0.5f + b;
		else return EASE_OUT_BOUNCE(t * 2.0f - d, d, 0.0f, c) * 0.5f + c * 0.5f + b;
	}
};


#endif