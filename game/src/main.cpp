/*
 *
 * TODO:
 * 	Get camera working correctly
 * 	Test out cube creation/manipulation
 *
 */

#include <raylib.h>
#include "extras.hpp"
#include <iostream>
#include <vector>
#include <rlImGui.h>
#include <rcamera_blender.h>

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
Config cfg;
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

	bcam.freeFly = true;
	SetConfigFlags(FLAG_VSYNC_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1000, 1000, "YEEHAW");
	SetTargetFPS(60);
	rlImGuiSetup(true);
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
		if(cameraMovement){
			BlenderCameraUpdate(&bcam);
		}

			BeginDrawing();

			ClearBackground(BLACK);
			DrawFPS(0,0);

			//DrawText("Hello World", 0, 20, 20, WHITE);
			BeginMode3D(bcam.camera); 
				DrawGrid(15, 1.0f);
				for(Entity c : cubes) {
					switch(c.type) {
						case TYPE_CUBE:
							DrawCubeV(c.obj.cube.pos, c.obj.cube.size, c.obj.cube.color);
							break;
						case TYPE_SPHERE:
							DrawSphereEx(c.obj.sphere.pos, c.obj.sphere.rad, c.obj.sphere.rings, c.obj.sphere.slices, c.obj.sphere.color);
							//rings and slices when initing are floats "bit-casted" as an int so it looks ridiculously large
							break;
						case TYPE_CYLINDER:
							DrawCylinderEx(c.obj.cylinder.startPos, c.obj.cylinder.endPos, c.obj.cylinder.startRadius, c.obj.cylinder.endRadius, c.obj.cylinder.sides, c.obj.cylinder.color);
							break;
					}
				}
			EndMode3D();


			rlImGuiBegin();
				ImGui::Checkbox("ImGui Demo", &cfg.showDemo);
				ImGui::DragFloat("Indent", &DBG_Indent);
				if(cfg.showDemo) {
					bool open = true;
					ImGui::ShowDemoWindow(&open);
				}
				ImGui::Begin("Cube Assembler - by RadsammyT", NULL, ImGuiWindowFlags_MenuBar);
					
					if(ImGui::BeginMenuBar()) {
						if(ImGui::BeginMenu("New")) {

							if(ImGui::MenuItem("Cube")) {
								cubes.push_back(
									Entity {
										TYPE_CUBE,
										Cube { Vector3{0,0,0}, Vector3{1,1,1}, WHITE}
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
							ImGui::EndMenu();
						}
						ImGui::EndMenuBar();
					}

					if(ImGui::BeginTabBar("IM_LIBBING!!!!!!!!!!!!!!!!!!!!!!!!")) {
						if(ImGui::BeginTabItem("Cube List")) {
							for(int i = 0; i < cubes.size(); i++) {
								if(ImGui::Button(TextFormat("%d) Delete", i))) {
									cubes.erase(cubes.begin() + i);
								}
								switch(cubes[i].type) {
									case TYPE_CUBE:
										ImGui::SameLine();
										ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_POS_%d", i), &cubes[i].obj.cube.pos.x, cfg.dragDelta);
										ImGui::Indent(79.0f);
										ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_SIZE_%d", i), &cubes[i].obj.cube.size.x, cfg.dragDelta);
										break;

									case TYPE_SPHERE:
										ImGui::SameLine();
										ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_POS_%d" , i), &cubes[i].obj.sphere.pos.x, cfg.dragDelta);
										ImGui::Indent(79);
										ImGui::DragFloat(TextFormat("##CUBE_LIST_DRAG_RAD_%d", i), &cubes[i].obj.sphere.rad, cfg.dragDelta);
										ImGui::DragInt(TextFormat("##CUBE_LIST_DRAG_RING_%d", i), &cubes[i].obj.sphere.rings);
										ImGui::DragInt(TextFormat("##CUBE_LIST_DRAG_SLICE_%d", i), &cubes[i].obj.sphere.slices);
										break;

									case TYPE_CYLINDER:
										ImGui::Text("Unimplemented");
										break;
								}
								ImGui::Unindent(79);

							}
							ImGui::EndTabItem();
						}

						if(ImGui::BeginTabItem("Settings")) {
							ImGui::DragFloat("Cube List Drag Delta", &cfg.dragDelta, 0.1);
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
