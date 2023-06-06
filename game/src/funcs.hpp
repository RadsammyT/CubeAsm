#include "structs.hpp"
//#include <raylib.h>
#pragma once

Color inverseColor(Color in);
void ImGuiObjects(std::vector<Entity>);

Color inverseColor(Color in) {
	return Color {
		255 - in.r,
		255 - in.g,
		255 - in.b,
			  in.a
	};
}

void DrawObjects(std::vector<Entity> cubes, Config cfg) {
	for(Entity c : cubes) {
		switch(c.type) {
			case TYPE_CUBE:
				if(cfg.drawSolid)
					DrawCubeV(c.obj.cube.pos, c.obj.cube.size, c.obj.cube.color);
				if(cfg.drawWire)
					DrawCubeWiresV(
							c.obj.cube.pos,
							c.obj.cube.size,
							inverseColor(c.obj.cube.color)
					);
				break;
			case TYPE_SPHERE:
				if(cfg.drawSolid)
					DrawSphereEx(
								c.obj.sphere.pos, 
								c.obj.sphere.rad, 
								c.obj.sphere.rings, 
								c.obj.sphere.slices, 
								c.obj.sphere.color
					);
				if(cfg.drawWire)
					DrawSphereWires(
							c.obj.sphere.pos, 
							c.obj.sphere.rad, 
							c.obj.sphere.rings, 
							c.obj.sphere.slices, 
							inverseColor(c.obj.cube.color)
					);
				//rings and slices when initing are floats read as an int so it looks ridiculously large
				break;
			case TYPE_CYLINDER:
				if(cfg.drawSolid)
					DrawCylinderEx(
							c.obj.cylinder.startPos, 
							c.obj.cylinder.endPos, 
							c.obj.cylinder.startRadius, 
							c.obj.cylinder.endRadius, 
							c.obj.cylinder.sides, 
							c.obj.cylinder.color
					);
				break;
		}
	}
} 
