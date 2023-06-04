#include <raylib.h>

struct Cube {
	Vector3 pos;
	Vector3 size;
	Color color;
};

struct Config {
	bool showDemo;
	bool mouseState;
	bool cameraMovement;
	float dragDelta;
};
