#pragma once

#include "inc/graphics/primitives/Updatable.hpp"

namespace rl {
	extern "C" {
		#include "raylib.h"
	}
}

class PostProcessor: public Updatable {
	float width, height;
	rl::Shader postShader;
  rl::RenderTexture2D targetTexture;

  PostProcessor();

public:
	PostProcessor(PostProcessor const &)  = delete;
  void operator=(PostProcessor const &) = delete;

	static PostProcessor* getInstance();

	void update();
	void reloadTargetTexture();

	void beginDrawing();
	void endDrawing();
};