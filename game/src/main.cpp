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
Config cfg = {
	.dragDelta = 0.05f,
	.drawSolid = true,
};
bool mouseState = true;
bool cameraMovement = false;
Camera3D cam = {
	Vector3 {0,10.0f,10.0f},
	Vector3 {0,0,0},
	Vector3 {0,1.0f,0},
	90.0f,
	CAMERA_PERSPECTIVE

};

BlenderCamera bcam = CreateBlenderCamera();
//Debug Vars
float DBG_Indent = 0.0f;

int main() {
	fmt::print("HELLO FMT\n");
	bcam.freeFly = false;
	SetConfigFlags(FLAG_VSYNC_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1000, 1000, "YEEHAW");
	SetTargetFPS(60);
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	while(!WindowShouldClose()) {
		if(IsKeyPressed(KEY_C)) {
			cameraMovement = !cameraMovement;
			for(;;) {
				if(mouseState) {
					mouseState = false;
					DisableCursor();
					break;
				}

				if(!mouseState) {
					mouseState = true;
					EnableCursor();
					break;
				}

			}

		}
		if(!(io.WantCaptureMouse || io.WantCaptureKeyboard))
			BlenderCameraUpdate(&bcam);

		BeginDrawing();

			ClearBackground(BLACK);
			DrawFPS(0,0);

			//DrawText("Hello World", 0, 20, 20, WHITE);
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
						if(ImGui::BeginMenu("New")) {

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
						ImGui::Text("     ");
						if(ImGui::BeginMenu("File")) {
							ImGui::Text("WIP!!!");
							if(ImGui::MenuItem("Save Scene (CubesOnly)")) {
								rad::file::saveScene(
										rad::saveDialog(fp, 1, "CubeAsm Files"), 
										cubes);
							}
							ImGui::EndMenu();
						}
						ImGui::EndMenuBar();
					}

					if(ImGui::BeginTabBar("IM_LIBBING!!!!!!!!!!!!!!!!!!!!")) {
						if(ImGui::BeginTabItem("Cube List")) {
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
