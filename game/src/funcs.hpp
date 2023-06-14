#include "structs.hpp"
#include <vector>
#include <string>
#include <sstream>
//#include <raylib.h>
#pragma once
namespace rad {
	Color inverseColor(Color in);
	void ImGuiObjects(std::vector<Entity> cubes);
	void DrawObjects(std::vector<Entity> cubes, Config cfg);
	std::vector<std::string> tokenize(std::string in, char del);

	
	std::vector<std::string> tokenize(std::string in, char del) {
		std::string buf;
		std::vector<std::string> ret;
		std::stringstream instr(in);
		while(std::getline(instr, buf, del)) 
			ret.push_back(buf);

		return ret;
	}

	void ImGuiObjects(std::vector<Entity>* cubes, Config cfg) {
		for(int i = 0; i < cubes->size(); i++) {
			if(ImGui::Button(TextFormat("%d) Delete", i))) {
				cubes->erase(cubes->begin() + (i));
				continue;
			}
			switch(cubes->at(i).type) {
				case TYPE_CUBE:
					ImGui::SameLine();
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_POS_%d", i),
						   	&cubes->at(i).obj.cube.pos.x, cfg.dragDelta);
					ImGui::Indent(79.0f);
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_SIZE_%d", i),
						   	&cubes->at(i).obj.cube.size.x, cfg.dragDelta);
					break;

				case TYPE_SPHERE:
					ImGui::SameLine();
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_POS_%d" , i),
						   	&cubes->at(i).obj.sphere.pos.x, cfg.dragDelta);
					ImGui::Indent(79);
					ImGui::DragFloat(TextFormat("##CUBE_LIST_DRAG_RAD_%d", i),
						   	&cubes->at(i).obj.sphere.rad, cfg.dragDelta);
					ImGui::DragInt(TextFormat("##CUBE_LIST_DRAG_RING_%d", i),
						   	&cubes->at(i).obj.sphere.rings);
					ImGui::DragInt(TextFormat("##CUBE_LIST_DRAG_SLICE_%d", i),
						   	&cubes->at(i).obj.sphere.slices);
					break;

				case TYPE_CYLINDER:
					ImGui::SameLine();
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_STARTPOS_%d", i),
					&cubes->at(i).obj.cylinder.startPos.x, cfg.dragDelta);
					ImGui::Indent(79);
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_ENDPOS_%d", i),
					   	&cubes->at(i).obj.cylinder.endPos.x, cfg.dragDelta);
					ImGui::DragFloat(TextFormat("##CUBE_LIST_STARTRAD_%d", i),
					   	&cubes->at(i).obj.cylinder.startRadius, cfg.dragDelta);
					ImGui::DragFloat(TextFormat("##CUBE_LIST_ENDRAD_%d", i),
					   	&cubes->at(i).obj.cylinder.endRadius, cfg.dragDelta);
					ImGui::DragInt(TextFormat("##CUBE_LIST_DRAG_SIDES_%d", i),
					   	&cubes->at(i).obj.cylinder.sides);
					break;

				default:
					ImGui::SameLine();
					ImGui::TextColored(ImVec4{1,0,0,1}, "Invalid type: %d!", &cubes->at(i).type);
					break;
			}
			ImGui::Unindent(79);

		}
	}

	Color inverseColor(Color in) {
		unsigned char temp = 255;
		return Color {
			temp - in.r,
			temp- in.g,
			temp- in.b,
				  in.a
		};
	}
	void DrawObjects(std::vector<Entity> cubes, Config cfg) {
		for(Entity c : cubes) {
			switch(c.type) {
				case TYPE_CUBE:
					if(cfg.drawSolid)
						DrawCubeV(c.obj.cube.pos,
							   	c.obj.cube.size, 
								c.obj.cube.color);
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
					if(cfg.drawWire) {
						DrawCylinderWiresEx(
								c.obj.cylinder.startPos, 
								c.obj.cylinder.endPos, 
								c.obj.cylinder.startRadius, 
								c.obj.cylinder.endRadius, 
								c.obj.cylinder.sides, 
								inverseColor(c.obj.cylinder.color)
								);
					}
					break;
			}
		}
	}
}
