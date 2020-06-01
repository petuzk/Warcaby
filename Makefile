CC_FLAGS = --std=c++11
CC_LIBS  = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL raylib/src/libraylib.a
CC_INC   = -I. -Iraylib/src

all: build run

raylib/src/raylib.h:
	wget "https://github.com/raysan5/raylib/releases/download/3.0.0/raylib-noexamples-3.0.0.tar.gz"
	tar -xzf "raylib-noexamples-3.0.0.tar.gz"
	rm "raylib-noexamples-3.0.0.tar.gz"
	mv "raylib-3.0.0/" "raylib/"
	if [ -f "raylib/src/raylib.h" ]; then echo "raylib successfully downloaded"; else exit 1; fi

raylib/src/libraylib.a: raylib/src/raylib.h
	make -C raylib/src/
	if [ -f "raylib/src/libraylib.a" ]; then echo "raylib successfully compiled"; else exit 2; fi

build: raylib/src/libraylib.a
	clang++ $(CC_FLAGS) $(CC_LIBS) $(CC_INC) -o mygame \
		src/*.cpp src/ai/*.cpp \
		src/game/*.cpp src/game/board/*.cpp src/game/checker/*.cpp src/game/player/*.cpp \
		src/graphics/*.cpp src/graphics/overlay/*.cpp src/graphics/primitives/*.cpp \
		src/graphics/rendering/*.cpp src/graphics/scene/*.cpp

run:
	./mygame