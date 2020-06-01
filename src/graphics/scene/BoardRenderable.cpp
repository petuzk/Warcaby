#include "inc/graphics/scene/BoardRenderable.hpp"
#include "src/game/board/Board_template.cpp"
#include "src/graphics/scene/CheckerRenderable_template.cpp"

rl::Model BoardRenderable::rlModel = { 0 };
int BoardRenderable::shaderHlFromLoc[SHADER_MAX_HIGHLIGHTS];
int BoardRenderable::shaderHlToLoc[SHADER_MAX_HIGHLIGHTS];
int BoardRenderable::shaderHlColorLoc[SHADER_MAX_HIGHLIGHTS];
const rl::Vector3 BoardRenderable::CAMERA_TARGET = { 0.0f, 0.5f, 0.0f };

BoardRenderable::BoardRenderable(): Renderable(Renderable::T3D), Updatable(Priority::PBoardRenderable) {
	if (rlModel.meshCount == 0) {
		// Generacja modeli planszy

		static constexpr float width = 9.0f;
		static constexpr float height = 1.0f;
		static constexpr float length = 9.0f;

		rl::Mesh topMesh = rl::GenMeshPlane(width, length, 5, 5);
		rl::Mesh sideMesh = { 0 };
		sideMesh.vboId = new unsigned int[7];
		sideMesh.vertexCount = 16;
		sideMesh.triangleCount = 8;

		float vertices[] = {
			-width/2, -height,  length/2,
			 width/2, -height,  length/2,
			 width/2,       0,  length/2,
			-width/2,       0,  length/2,
			-width/2, -height, -length/2,
			-width/2,       0, -length/2,
			 width/2,       0, -length/2,
			 width/2, -height, -length/2,
			 width/2, -height, -length/2,
			 width/2,       0, -length/2,
			 width/2,       0,  length/2,
			 width/2, -height,  length/2,
			-width/2, -height, -length/2,
			-width/2, -height,  length/2,
			-width/2,       0,  length/2,
			-width/2,       0, -length/2
		};

		float texcoords[] = {
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
		};

		float normals[] = {
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f,-1.0f,
			0.0f, 0.0f,-1.0f,
			0.0f, 0.0f,-1.0f,
			0.0f, 0.0f,-1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f
		};

		sideMesh.vertices = new float[sideMesh.vertexCount * 3];
		std::memcpy(sideMesh.vertices, vertices, sideMesh.vertexCount*3*sizeof(float));

		sideMesh.texcoords = new float[sideMesh.vertexCount * 2];
		std::memcpy(sideMesh.texcoords, texcoords, sideMesh.vertexCount*2*sizeof(float));

		sideMesh.normals = new float[sideMesh.vertexCount * 3];
		std::memcpy(sideMesh.normals, normals, sideMesh.vertexCount*3*sizeof(float));

		sideMesh.indices = new unsigned short[sideMesh.triangleCount * 3];

		for (int i = 0, k = 0; i < sideMesh.triangleCount*3; i+=6, k+=4) {
			sideMesh.indices[i+0] = k;
			sideMesh.indices[i+1] = k+1;
			sideMesh.indices[i+2] = k+2;
			sideMesh.indices[i+3] = k;
			sideMesh.indices[i+4] = k+2;
			sideMesh.indices[i+5] = k+3;
		}

		rl::rlLoadMesh(&sideMesh, false);

		// kolejny "bezpiecznik" -- obraz się nie załaduje gdy zmienimy rozmiar planszy a nie dodamy odpowiedniego obrazu
		rl::Texture2D topAlbedoTex = rl::LoadTexture(rl::TextFormat("resources/images/board/%dx%d_top_albedo.png", BoardConst::SIZE, BoardConst::SIZE));
		rl::Texture2D topNormalTex = rl::LoadTexture(rl::TextFormat("resources/images/board/%dx%d_top_normal.png", BoardConst::SIZE, BoardConst::SIZE));
		rl::Texture2D sideAlbedoTex = rl::LoadTexture("resources/images/board/side_albedo.png");
		rl::Texture2D sideNormalTex = rl::LoadTexture("resources/images/board/side_normal.png");

		rl::Shader shader = ShaderProvider::getInstance()->loadShader("resources/shaders/base.vs", "resources/shaders/board.fs");
		shader.locs[rl::LOC_MAP_ALBEDO] = rl::GetShaderLocation(shader, "albedoSampler");
		shader.locs[rl::LOC_MAP_NORMAL] = rl::GetShaderLocation(shader, "normalsSampler");
		for (int i = 0; i < SHADER_MAX_HIGHLIGHTS; i++) {
			shaderHlFromLoc[i]  = rl::GetShaderLocation(shader, rl::TextFormat("hlSquares[%d].from",  i));
			shaderHlToLoc[i]    = rl::GetShaderLocation(shader, rl::TextFormat("hlSquares[%d].to",    i));
			shaderHlColorLoc[i] = rl::GetShaderLocation(shader, rl::TextFormat("hlSquares[%d].color", i));
		}

		rlModel.transform = rl::MatrixIdentity();

		rlModel.meshCount = 2;
		rlModel.meshes = new rl::Mesh[rlModel.meshCount];
		rlModel.meshes[0] = topMesh;
		rlModel.meshes[1] = sideMesh;

		rlModel.materialCount = 2;
		rlModel.materials = new rl::Material[rlModel.materialCount];
		rlModel.materials[0] = rl::LoadMaterialDefault();
		rlModel.materials[0].shader = shader;
		rlModel.materials[0].maps[rl::MAP_ALBEDO].texture = topAlbedoTex;
		rlModel.materials[0].maps[rl::MAP_NORMAL].texture = topNormalTex;
		rlModel.materials[1] = rl::LoadMaterialDefault();
		rlModel.materials[1].shader = shader;
		rlModel.materials[1].maps[rl::MAP_ALBEDO].texture = sideAlbedoTex;
		rlModel.materials[1].maps[rl::MAP_NORMAL].texture = sideNormalTex;

		rlModel.meshMaterial = new int[rlModel.meshCount];
		rlModel.meshMaterial[0] = 0;
		rlModel.meshMaterial[1] = 1;
	}

	reset();
}

void BoardRenderable::reset() {
	ifState = NONE;
	inputEnabled = true;
	delayedShaderUpdate = false;
	Board::reset<CheckerRenderable<CheckerMan>>();
}

void BoardRenderable::setInputEnabled(bool en) {
	inputEnabled = en;
	if (!inputEnabled) {
		prevPointed = Square();
		updateShaderColor(0);
	} else {
		delayedShaderUpdate = true;
	}
}

void BoardRenderable::draw() {
	rl::DrawModel(rlModel, (rl::Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, rl::WHITE);
}

void BoardRenderable::requestMoveFor(HumanPlayer* player, std::shared_ptr<PlayerMoveSequence> sequence) {
	updateShaderColor(0);
	delayedShaderUpdate = true;

	moveForPlayer = player;
	moveSequence = sequence;
	prevPointed = selectedMoveSquare = reSelectedCheckerSquare = Square();

	if (sequence->getOriginsForNextMove().size() > 1) {
		ifState = SELECT_CHECKER;
		possibleMoves.clear();
		selectedCheckerSquare = Square();
	} else {
		Square start = sequence->getOriginsForNextMove().at(0);
		std::shared_ptr<Checker> c = at(start);

		if (c == nullptr || c->getSide() != player->getSide())
			throw std::logic_error("bad move selection origin");

		ifState = SELECT_MOVE;
		possibleMoves = c->getPossibleMoves(this, moveSequence);
		selectedCheckerSquare = start;
	}

	Camera::getInstance()->moveToSide(player->getSide());
}

void BoardRenderable::update() {
	if (ifState == NONE || !inputEnabled) {
		return;
	}

	bool updateShader = delayedShaderUpdate;
	delayedShaderUpdate = false;
	Square pointed = getPointedSquare();

	if (pointed != prevPointed) {
		prevPointed = pointed;

		std::shared_ptr<Checker> c = at(pointed);

		if (ifState == SELECT_CHECKER) {
			if ((c == nullptr || c->getSide() != moveForPlayer->getSide())) {
				if (selectedCheckerSquare) {
					selectedCheckerSquare = Square();
					possibleMoves.clear();
					updateShader = true;
				}
			}
			else {
				selectedCheckerSquare = pointed;
				if (moveSequence->isOriginForNextMove(selectedCheckerSquare)) {
					possibleMoves = c->getPossibleMoves(this, moveSequence);
				} else {
					possibleMoves.clear();
				}
				updateShader = true;
			}
		}
		else if (ifState == SELECT_MOVE) {
			if (c == nullptr) {
				Square newSelectedMoveSquare = Square();

				for (PlayerMove& move: possibleMoves) {
					Square dest = move.getDestination();
					if (pointed == dest) {
						newSelectedMoveSquare = dest;
						break;
					}
				}

				if (selectedMoveSquare != newSelectedMoveSquare || reSelectedCheckerSquare) {
					reSelectedCheckerSquare = Square();
					selectedMoveSquare = newSelectedMoveSquare;
					updateShader = true;
				}
			}
			else {
				if (c->getSide() != moveForPlayer->getSide()) {
					if (reSelectedCheckerSquare) {
						reSelectedCheckerSquare = Square();
						updateShader = true;
					}
				}
				else if (moveSequence->isOriginForNextMove(pointed)) {
					reSelectedCheckerSquare = pointed;
					updateShader = true;
				}
			}
		}
	}

	if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON)) {
		if (ifState == SELECT_CHECKER) {
			if (selectedCheckerSquare && moveSequence->isOriginForNextMove(selectedCheckerSquare)) {
				ifState = SELECT_MOVE;
			}
		}
		else if (ifState == SELECT_MOVE) {
			if (reSelectedCheckerSquare) {
				selectedCheckerSquare = reSelectedCheckerSquare;
				possibleMoves = at(selectedCheckerSquare)->getPossibleMoves(this, moveSequence);
				updateShader = true;
			} else {
				for (PlayerMove& move: possibleMoves) {
					if (selectedMoveSquare == move.getDestination()) {
						ifState = NONE;

						/* Jeżeli zastąpić to `updateShader = true;` i `break;`, to w efekcie końcowym wywołanie
						 * Player::respond() doprowadzi do kolejnego wywołania BoardRenderable::requestMoveFor(),
						 * które zmieni ifState oraz inne zmienne stanu, i wtedy wywołanie updateShaderData() niżej
						 * narysuje te zmiany przed rozpoczęciem animacji.
						 */
						updateShaderData();
						moveForPlayer->respond(move);
						return;
					}
				}
			}
		}
	}

	if (updateShader) {
		updateShaderData();
	}
}

void BoardRenderable::updateShaderPos(int i, Square sq) {
	float xpos = sq.col() / 9.0f + 1.0f / 18.0f;
	float zpos = (BoardConst::SIZE - sq.row() - 1) / 9.0f + 1.0f / 18.0f;

	rl::SetShaderValue(rlModel.materials[0].shader, shaderHlFromLoc[i], (float[2]){ xpos, zpos }, rl::UNIFORM_VEC2);
	rl::SetShaderValue(rlModel.materials[0].shader, shaderHlToLoc[i], (float[2]){ xpos + 1.0f/9.0f, zpos + 1.0f/9.0f }, rl::UNIFORM_VEC2);
}

void BoardRenderable::updateShaderColor(int i, rl::Color c) {
	float color[4] = { c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f };
	rl::SetShaderValue(rlModel.materials[0].shader, shaderHlColorLoc[i], &color, rl::UNIFORM_VEC4);
}

void BoardRenderable::updateShaderData() {
	int i = 0, size = possibleMoves.size();
	if (SHADER_MAX_HIGHLIGHTS < size) // sytuacja (prawdopodobnie) niemożliwa
		size = SHADER_MAX_HIGHLIGHTS;
	rl::Shader shader = rlModel.materials[0].shader;

	if (ifState == NONE) {
		updateShaderColor(0);
	}
	else if (ifState == SELECT_CHECKER) {
		if (!selectedCheckerSquare) {
			updateShaderColor(0);
		} else {
			updateShaderPos(0, selectedCheckerSquare);

			if (size == 0) {
				updateShaderColor(0, rl::RED);    // maroon, orange
			} else {
				updateShaderColor(0, rl::GREEN);  //lime, darkgreen

				for ( ; i < size; i++) {
					updateShaderPos(i + 1, possibleMoves[i].getDestination());
					updateShaderColor(i + 1, rl::BROWN);
				}
			}

			if (i + 1 < SHADER_MAX_HIGHLIGHTS) {
				updateShaderColor(i + 1);
			}
		}
	}
	else if (ifState == SELECT_MOVE) {
		updateShaderPos(0, selectedCheckerSquare);
		updateShaderColor(0, rl::GREEN);

		for ( ; i < size; i++) {
			Square dest = possibleMoves[i].getDestination();
			updateShaderPos(i + 1, dest);
			updateShaderColor(i + 1, dest == selectedMoveSquare ? rl::BEIGE : rl::BROWN);
		}

		if (i + 1 < SHADER_MAX_HIGHLIGHTS && reSelectedCheckerSquare) {
			updateShaderPos(i + 1, reSelectedCheckerSquare);
			updateShaderColor(i + 1, rl::GREEN);
			i++;
		}

		if (i + 1 < SHADER_MAX_HIGHLIGHTS) {
			updateShaderColor(i + 1);
		}
	}
}

Square BoardRenderable::getPointedSquare() {
	static const rl::Vector3 triangles[] = {
		{ -4.0f, 0.0f, -4.0f },
		{  4.0f, 0.0f,  4.0f },
		{  4.0f, 0.0f, -4.0f },

		{ -4.0f, 0.0f, -4.0f },
		{ -4.0f, 0.0f,  4.0f },
		{  4.0f, 0.0f,  4.0f },
	};

	rl::Ray ray = Camera::getInstance()->getMouseRay();
	rl::RayHitInfo hitInfo;

	for (int i = 0; i < 6; i += 3) {
		hitInfo = rl::GetCollisionRayTriangle(ray, triangles[i], triangles[i+1], triangles[i+2]);
		if (hitInfo.hit) {
			int xsq = hitInfo.position.x + 4.0f;
			int zsq = hitInfo.position.z + 4.0f;
			return Square(xsq, BoardConst::SIZE - zsq - 1);
		}
	}

	return Square();
}