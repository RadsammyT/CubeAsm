#include "structs.hpp"
#include <vector>
#include <string>
#include <sstream>
//#include <raylib.h>
#pragma once

float DBG_Indent = 0.0f;

namespace rad {
	Color inverseColor(Color in);
	void ImGuiObjects(std::vector<Entity> cubes);
	void DrawObjects(std::vector<Entity> cubes, Config cfg);
	std::vector<std::string> tokenize(std::string in, char del);
	void Qpop(std::string in);
	
	void Qpop(std::string in) {
		if(ImGui::IsItemHovered()) {
			ImGui::SetTooltip(in.c_str());
		}
	}

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
					ImGui::SameLine();
					ImGui::Text("Cube");
					rad::Qpop("Position");
					ImGui::Indent(79.0f);
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_SIZE_%d", i),
						   	&cubes->at(i).obj.cube.size.x, cfg.dragDelta);
					rad::Qpop("Size");
					ImGui::DragScalarN(
							TextFormat("##CUBE_LIST_DRAG_COLOR_%d",i),
							ImGuiDataType_U8,
							&cubes->at(i).obj.cube.color,
							4);
					rad::Qpop("Color (RGBA)");
					break;

				case TYPE_SPHERE:
					ImGui::SameLine();
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_POS_%d" , i),
						   	&cubes->at(i).obj.sphere.pos.x, cfg.dragDelta);
					ImGui::SameLine();
					ImGui::Text("Sphere");
					rad::Qpop("Position");
					ImGui::Indent(79);
					ImGui::DragFloat(TextFormat("##CUBE_LIST_DRAG_RAD_%d", i),
						   	&cubes->at(i).obj.sphere.rad, cfg.dragDelta);
					rad::Qpop("Radius");
					ImGui::DragInt(TextFormat("##CUBE_LIST_DRAG_RING_%d", i),
						   	&cubes->at(i).obj.sphere.rings);
					rad::Qpop("Rings");
					ImGui::DragInt(TextFormat("##CUBE_LIST_DRAG_SLICE_%d", i),
						   	&cubes->at(i).obj.sphere.slices);
					rad::Qpop("Slices");
					ImGui::DragScalarN(
							TextFormat("##CUBE_LIST_DRAG_COLOR_%d",i),
							ImGuiDataType_U8,
							&cubes->at(i).obj.sphere.color,
							4);
					rad::Qpop("Color (RGBA)");
					break;

				case TYPE_CYLINDER:
					ImGui::SameLine();
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_STARTPOS_%d", i),
					&cubes->at(i).obj.cylinder.startPos.x, cfg.dragDelta);
					rad::Qpop("Start Position");
					ImGui::SameLine();
					ImGui::Text("Cylinder");
					ImGui::Indent(79);
					ImGui::DragFloat3(TextFormat("##CUBE_LIST_ENDPOS_%d", i),
					   	&cubes->at(i).obj.cylinder.endPos.x, cfg.dragDelta);
					rad::Qpop("End Position");
					ImGui::DragFloat(TextFormat("##CUBE_LIST_STARTRAD_%d", i),
					   	&cubes->at(i).obj.cylinder.startRadius, cfg.dragDelta);
					rad::Qpop("Start Radius");
					ImGui::DragFloat(TextFormat("##CUBE_LIST_ENDRAD_%d", i),
					   	&cubes->at(i).obj.cylinder.endRadius, cfg.dragDelta);
					rad::Qpop("End Radius");
					ImGui::DragInt(TextFormat("##CUBE_LIST_DRAG_SIDES_%d", i),
					   	&cubes->at(i).obj.cylinder.sides);
					rad::Qpop("Slices");
					ImGui::DragScalarN(
							TextFormat("##CUBE_LIST_DRAG_COLOR_%d",i),
							ImGuiDataType_U8,
							&cubes->at(i).obj.cylinder.color,
							4);
					rad::Qpop("Color (RGBA)");
					break;

				default:
					ImGui::SameLine();
					ImGui::TextColored(ImVec4{1,0,0,1}, "Invalid type: %d!", &cubes->at(i).type);
					break;
			}
			ImGui::Unindent(79);
			ImGui::Separator();
		}
	}

	Color inverseColor(Color in) {
		return Color {
		(unsigned char)(255 - in.r),
		(unsigned char)(255 - in.g),
		(unsigned char)(255 - in.b),
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
