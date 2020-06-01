#include "inc/graphics/rendering/PostProcessor.hpp"

PostProcessor::PostProcessor(): Updatable(Priority::PPostProcessor) {
	postShader = rl::LoadShader(nullptr, "resources/shaders/post.fs");
	targetTexture = { 0 };
	reloadTargetTexture();
}

PostProcessor* PostProcessor::getInstance() {
	static PostProcessor* instance = new PostProcessor();
	return instance;
}

void PostProcessor::update() {
	if (rl::IsWindowResized()) {
		reloadTargetTexture();
	}
}

void PostProcessor::reloadTargetTexture() {
	width = rl::GetScreenWidth();
	height = rl::GetScreenHeight();

	rl::SetShaderValue(postShader, rl::GetShaderLocation(postShader, "renderWidth"), &width, rl::UNIFORM_FLOAT);
	rl::SetShaderValue(postShader, rl::GetShaderLocation(postShader, "renderHeight"), &height, rl::UNIFORM_FLOAT);

	rl::UnloadRenderTexture(targetTexture);
	targetTexture = rl::LoadRenderTexture(width, height);
}

void PostProcessor::beginDrawing() {
	rl::BeginDrawing();
	rl::ClearBackground(rl::BLACK);
	rl::BeginTextureMode(targetTexture);
}

void PostProcessor::endDrawing() {
	rl::EndTextureMode();
	rl::BeginShaderMode(postShader);
	rl::DrawTextureRec(
		targetTexture.texture,
		(rl::Rectangle){ 0, 0, (float) targetTexture.texture.width, (float) -targetTexture.texture.height },
		(rl::Vector2){ 0, 0 },
		rl::WHITE);
	rl::EndShaderMode();
	rl::EndDrawing();
}
