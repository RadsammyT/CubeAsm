/*
 *
 * TODO:
 *
 */

#define FMT_HEADER_ONLY

#include <raylib.h>
#include "structs.hpp"
#include <iostream>
#include <vector>
#include <rlImGui.h>
#include <rcamera_blender.h>
#include "funcs.hpp"
#include <fmt/format.h>
#include "files.hpp"
#include <tinyfiledialogs.h>
#include <imgui_internal.h>

std::vector<Entity> cubes = {
	Entity {
		TYPE_CUBE,
		Cube {
			Vector3{0,0,0},
			Vector3{1,1,1},
			WHITE
		}
	}
};

struct {
	int indexA = 0;
	int indexB = 0;
} swap;

Config cfg = {
	.dragDelta = 0.05f,
	.drawSolid = true,
};

BlenderCamera bcam = CreateBlenderCamera();
int main() {
	bcam.freeFly = false;
	SetConfigFlags(FLAG_VSYNC_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1000, 1000, "CubeAsm");
	SetTargetFPS(60);
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	while(!WindowShouldClose()) {

		if(!(io.WantCaptureMouse || io.WantCaptureKeyboard))
			BlenderCameraUpdate(&bcam);

		BeginDrawing();

			ClearBackground(BLACK);
			DrawFPS(0,0);

			BeginMode3D(bcam.camera); 
				DrawGrid(15, 1.0f);
				rad::DrawObjects(cubes, cfg);
			EndMode3D();

			rlImGuiBegin();
				ImGui::Checkbox("ImGui Demo", &cfg.showDemo);
				ImGui::DragFloat("Indent", &DBG_Indent);
				if(cfg.showDemo) {
					bool open = true;
					ImGui::ShowDemoWindow(&open);
				}
				ImGui::Begin("Cube Assembler - by RadsammyT", 
						NULL, 
						ImGuiWindowFlags_MenuBar);
					
					if(ImGui::BeginMenuBar()) {
						if(ImGui::BeginMenu("New Object")) {

							if(ImGui::MenuItem("Cube")) {
								cubes.push_back(
									Entity {
										.type =	TYPE_CUBE,
										.obj = Cube { 
											Vector3{0,0,0}, 
											Vector3{1,1,1}, 
											WHITE
										}
									}		
								);
							}
							if(ImGui::MenuItem("Sphere")) {
								Sphere dummy = { Vector3{0,0,0},4,10,10,WHITE};
								cubes.push_back(
									Entity {
										.type = TYPE_SPHERE,
										.obj = { .sphere = dummy }
									}
								);
							}
							if(ImGui::MenuItem("Cylinder")) {
								cubes.push_back(
									Entity {
										.type = TYPE_CYLINDER,
										.obj = {.cylinder = Cylinder {
											Vector3{0,0,0},
											Vector3{0,0,0},
											1.0f,
											1.0f,
											5,
											WHITE
										}}
									}		
								);
							}
							ImGui::EndMenu();
						}
						ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
						if(ImGui::BeginMenu("File")) {
							if(ImGui::MenuItem("Save Scene")) {
								rad::file::saveScene(
										rad::saveDialog(fp, 1, "CubeAsm Files"), 
										cubes);
							}

							if(ImGui::MenuItem("Load Scene")) {
								rad::file::loadScene(
										rad::loadDialog(fp, 1, "CubeAsm Files"),
										&cubes
									);
							}
							ImGui::EndMenu();
						}
						ImGui::EndMenuBar();
					}

					if(ImGui::BeginTabBar("IM_LIBBING!!!!!!!!!!!!!!!!!!!!")) {
						if(ImGui::BeginTabItem("Cube List")) {
							if(ImGui::CollapsingHeader("Swap Indexes")) {
								ImGui::SliderInt("Index A", &swap.indexA, 0, cubes.size() - 1); 
								ImGui::SliderInt("Index B", &swap.indexB, 0, cubes.size() - 1); 
								if(ImGui::Button("Swap Indexes") && !cubes.empty()) {
									// FIXME: swap doesnt swap two elements for some
									// bizarre reason.
									std::swap(cubes[swap.indexA], cubes[swap.indexB]);
								}
							}
							rad::ImGuiObjects(&cubes, cfg);
							ImGui::EndTabItem();
						}
						if(ImGui::BeginTabItem("Settings")) {
							ImGui::DragFloat("Cube List Drag Delta", 
									&cfg.dragDelta,
								   	0.1);
							ImGui::Checkbox("Draw Wires", &cfg.drawWire);
							ImGui::Checkbox("Draw Solids", &cfg.drawSolid);
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}
				ImGui::End();
			rlImGuiEnd();
		EndDrawing();
	}
	rlImGuiShutdown();
	CloseWindow();
	return 0;
}
