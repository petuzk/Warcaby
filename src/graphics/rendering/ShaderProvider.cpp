#include "inc/graphics/rendering/ShaderProvider.hpp"

ShaderProvider::ShaderProvider() {
	// pozostawiam możliwość zmiany tych wartości dynamicznie
	// zamiast wpisać stałe do shader'ów
	constants.shine = 5.38;
	constants.addBase = 0.41;
	constants.kSpecular = 0.2;
	constants.dAmbient = 3.61;
	constants.gammaR = 1.47;
	constants.gammaG = 1.47;
	constants.gammaB = 1.47;
}

ShaderProvider* ShaderProvider::getInstance() {
	static ShaderProvider* instance = new ShaderProvider();
	return instance;
}

void ShaderProvider::addLight(LightSource light) {
	if (lights.size() >= MAX_LIGHTS) {
		throw std::range_error("too many light sources");
	}

	lights.push_back(light);
}

void ShaderProvider::updateConstants() {
	for (rl::Shader shader: shaders) {
		updateConstants(shader);
	}
}

ShaderProvider::ShaderConstants& ShaderProvider::getConstants() {
	return constants;
}

void ShaderProvider::updateCameraPos(rl::Vector3 cameraPos) {
	float cameraPosArr[3] = { cameraPos.x, cameraPos.y, cameraPos.z };
	for (rl::Shader shader: shaders) {
		SetShaderValue(shader, shader.locs[rl::LOC_VECTOR_VIEW], cameraPosArr, rl::UNIFORM_VEC3);
	}
}

rl::Shader ShaderProvider::loadShader(const char *vsFileName, const char *fsFileName) {
	rl::Shader shader;
	loadShaderInto(shader, vsFileName, fsFileName);
	return shader;
}

void ShaderProvider::loadShaderInto(rl::Shader& shader, const char *vsFileName, const char *fsFileName) {
	shader = rl::LoadShader(vsFileName, fsFileName);
	shader.locs[rl::LOC_MATRIX_MODEL] = rl::GetShaderLocation(shader, "matModel");
	shader.locs[rl::LOC_VECTOR_VIEW] = rl::GetShaderLocation(shader, "viewPos");

	shaders.push_back(shader);

	int i = 0;
	for ( ; i < lights.size(); i++)
		setupLight(shader, i, lights[i]);
	for ( ; i < MAX_LIGHTS; i++)
		setupLight(shader, i, (LightSource){ false });

	updateConstants(shader);
}

void ShaderProvider::updateConstants(rl::Shader shader) {
	rl::SetShaderValue(shader, rl::GetShaderLocation(shader, "shine"), &constants.shine, rl::UNIFORM_FLOAT);
	rl::SetShaderValue(shader, rl::GetShaderLocation(shader, "addBase"), &constants.addBase, rl::UNIFORM_FLOAT);
	rl::SetShaderValue(shader, rl::GetShaderLocation(shader, "kSpecular"), &constants.kSpecular, rl::UNIFORM_FLOAT);
	rl::SetShaderValue(shader, rl::GetShaderLocation(shader, "dAmbient"), &constants.dAmbient, rl::UNIFORM_FLOAT);
	rl::SetShaderValue(shader, rl::GetShaderLocation(shader, "gammaR"), &constants.gammaR, rl::UNIFORM_FLOAT);
	rl::SetShaderValue(shader, rl::GetShaderLocation(shader, "gammaG"), &constants.gammaG, rl::UNIFORM_FLOAT);
	rl::SetShaderValue(shader, rl::GetShaderLocation(shader, "gammaB"), &constants.gammaB, rl::UNIFORM_FLOAT);
}

void ShaderProvider::setupLight(rl::Shader shader, int i, LightSource light) {
	char enabledName[32] = "lights[x].enabled\0";
	char typeName[32] = "lights[x].type\0";
	char posName[32] = "lights[x].position\0";
	char colorName[32] = "lights[x].color\0";

	enabledName[7] = '0' + i;
	typeName[7] = '0' + i;
	posName[7] = '0' + i;
	colorName[7] = '0' + i;

	int enabledLoc = rl::GetShaderLocation(shader, enabledName);
	int typeLoc = rl::GetShaderLocation(shader, typeName);
	int posLoc = rl::GetShaderLocation(shader, posName);
	int colorLoc = rl::GetShaderLocation(shader, colorName);

	rl::SetShaderValue(shader, enabledLoc, &light.enabled, rl::UNIFORM_INT);
	rl::SetShaderValue(shader, typeLoc, &light.type, rl::UNIFORM_INT);

	float position[3] = { light.position.x, light.position.y, light.position.z };
	rl::SetShaderValue(shader, posLoc, position, rl::UNIFORM_VEC3);

	float color[4] = { light.color.r / 255.0f, light.color.g / 255.0f,
	                   light.color.b / 255.0f, light.color.a / 255.0f };
	rl::SetShaderValue(shader, colorLoc, color, rl::UNIFORM_VEC4);
}