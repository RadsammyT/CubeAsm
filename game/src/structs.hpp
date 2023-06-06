/*
 *	a few notes on why there are functions on the model structs:
 *	per [https://stackoverflow.com/questions/18840205/are-class-struct-functions-stored-in-an-object]:
 *	
 *	Class functions in C++ are not linked (pointed) from the object itself, 
 *	they are simply stored as any other function. When you call a class function, 
 *	you are simply calling a normal function (you are not calling it from a pointer). 
 *	This is why doing something like b.func = another_func; is illegal in C++.
 *	
 *	
 */

#include <raylib.h>
#include <rlImGui.h>
#include <vector>
#pragma once
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
