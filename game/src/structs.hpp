#define FILE_NULL "_CA_NULL"

#include <raylib.h>
#include <rlImGui.h>
#include <vector>
#pragma once


enum Type {
	TYPE_NULL = -1,
	TYPE_CUBE = 0,
	TYPE_SPHERE = 1,
	TYPE_CYLINDER = 2
};


struct Cube {
	Vector3 pos;
	Vector3 size;
	Color color;
};

struct Sphere {
	Vector3 pos;
	float rad;
	int rings; //RING
	int slices; //SLICE
	Color color;
};

struct Cylinder {
	Vector3 startPos; //SPOS
	Vector3 endPos; // EPOS
	float startRadius; //SRAD
	float endRadius; //ERAD
	int sides; // SIDE
	Color color;
};

struct Config {
	bool showDemo;
	float dragDelta;
	bool drawWire;
	bool drawSolid;
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
