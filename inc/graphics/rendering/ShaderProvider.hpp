#pragma once

#include <vector>
#include <stdexcept>
#include <initializer_list>

namespace rl {
	extern "C" {
		#include "raylib.h"
	}
}

class ShaderProvider {
public:
	static constexpr int MAX_LIGHTS = 10;

	enum LightType {
		AMBIENT = 0,
		POINT = 1
	};

	struct LightSource {
		int enabled;
		LightType type;
		rl::Vector3 position;
		rl::Color color;
	};

	struct ShaderConstants {
		float shine;     // 5.38  [1-128]
		float addBase;   // 0.41  [0-1]
		float kSpecular; // 0.2   [0-1]
		float dAmbient;  // 3.61  [1-30]
		float gammaR;    // 1.6  [0.5-2.5]
		float gammaG;    // 1.6  [0.5-2.5]
		float gammaB;    // 1.6  [0.5-2.5]
	};

private:
	ShaderProvider();

	ShaderConstants constants;
	std::vector<LightSource> lights;
	std::vector<rl::Shader> shaders;

	static void setupLight(rl::Shader shader, int i, LightSource light);
	void updateConstants(rl::Shader shader);

public:
	ShaderProvider(ShaderProvider const &) = delete;
  void operator=(ShaderProvider const &) = delete;

	static ShaderProvider* getInstance();

	void addLight(LightSource light);

	void updateConstants();
	ShaderConstants& getConstants();

	void updateCameraPos(rl::Vector3 cameraPos);

	rl::Shader loadShader(const char *vsFileName, const char *fsFileName);
	void loadShaderInto(rl::Shader& shader, const char *vsFileName, const char *fsFileName);
};