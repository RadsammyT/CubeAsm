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

std::vector<Cube> cubes = {
	Cube {
		Vector3 {0,0,0},
		Vector3 {1,1,1},
		WHITE
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

//Debug Vars
float DBG_Indent = 0.0f;

int main() {
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
			UpdateCamera(&cam, CAMERA_FREE);
		}

			BeginDrawing();

			ClearBackground(BLACK);
			DrawFPS(0,0);

			//DrawText("Hello World", 0, 20, 20, WHITE);
			BeginMode3D(cam); 
				DrawGrid(15, 1.0f);
				for(Cube c : cubes) {
					DrawCubeV(c.pos, c.size, c.color);
				}
			EndMode3D();


			rlImGuiBegin();
				ImGui::Checkbox("ImGui Demo", &cfg.showDemo);
				ImGui::DragFloat("Indent", &DBG_Indent);
				if(cfg.showDemo) {
					bool open = true;
					ImGui::ShowDemoWindow(&open);
				}
				ImGui::Begin("Cube Assembler - by RadsammyT");
					ImGui::BeginTabBar("IM_LIBBING!!!!!!!!!!!!!!!!!!!!!!!!");
						if(ImGui::BeginTabItem("Cube List")) {
							for(int i = 0; i < cubes.size(); i++) {
								if(ImGui::Button(TextFormat("%d) Delete", i))) {
									cubes.erase(cubes.begin() + i);
								}
								ImGui::SameLine();
								ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_POS_%d", i), &cubes[i].pos.x, cfg.dragDelta);
								ImGui::Indent(79.0f);
								ImGui::DragFloat3(TextFormat("##CUBE_LIST_DRAG_SIZE_%d", i), &cubes[i].size.x, cfg.dragDelta);
							}
							ImGui::EndTabItem();
						}
					ImGui::EndTabBar();
				ImGui::End();
			rlImGuiEnd();
		EndDrawing();
	}
	rlImGuiShutdown();
	CloseWindow();
	return 0;
}
