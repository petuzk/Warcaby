#pragma once

class Renderable {
public:
	enum Type {
		T2D,
		T3D
	};

protected:
	Renderable(Type type);

public:
	virtual ~Renderable();

	const Type type;

	virtual void draw() = 0;
};