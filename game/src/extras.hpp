#include <raylib.h>

enum Type {
	TYPE_CUBE = 0,
	TYPE_SPHERE,
	TYPE_CYLINDER
};


struct Cube {
	Vector3 pos;
	Vector3 size;
	Color color;
};

struct Sphere {
	Vector3 pos;
	float rad;
	int rings;
	int slices;
	Color color;
};

struct Cylinder {
	Vector3 startPos;
	Vector3 endPos;
	float startRadius;
	float endRadius;
	int sides;
	Color color;
};

struct Config {
	bool showDemo;
	bool mouseState;
	bool cameraMovement;
	float dragDelta;
};

union Object {
	Cube cube;
	Sphere sphere;
	Cylinder cylinder;
};

struct Entity {
	Type type;
	Object obj;	
};
