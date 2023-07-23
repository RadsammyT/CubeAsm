/*
File saving/loading
-	a "language" for saving and loading CubeAsm.

-language example?

*
TYPE CUBE
POS 0.00,0.00,0.00 //declare cube position
SIZE 1.00,1.00,1.00 // size...
COLOR 255,255,255,255 //color...
PUSH // push set config to entity vector

TYPE CIRCLE
POS 10.00,10.00,10.00
RAD 1.00
RING 20
SLICE 20
COLOR 255,255,255,255
PUSH

TYPE CYLINDER
SPOS 10.00,10.00,0.00
EPOS 0.00,10.00,0.00
SRAD 10.
ERAD 15.00
SIDE 15
COLOR 255,255,255,255
PUSH
*


*/
#if 0
#include <fmt/format.h>
#else 
#include "../../fmt_lib/include/fmt/format.h"
#endif
#include <string>
#include <tinyfiledialogs.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "structs.hpp"
#include "funcs.hpp"
#pragma once

char const *fp[1] = {"*.CubeAsm"};
char const *ex[1] = {"*.h"};

namespace rad { 

//yoinked from https://en.cppreference.com/w/cpp/string/basic_string/replace
	std::size_t replace_all(std::string& inout, 
			std::string_view what, 
			std::string_view with) {
		std::size_t count{};
		for (std::string::size_type pos{};
			 inout.npos != (pos = inout.find(what.data(), pos, what.length()));
			 pos += with.length(), ++count) {
			inout.replace(pos, what.length(), with.data(), with.length());
		}
		return count;
	}

	std::string loadDialog(char const *filter[], int filterNum, const char* desc) {
		char* origin = tinyfd_openFileDialog(
				"Load .CubeAsm File",
				NULL,
				filterNum,
				filter,
				desc,
				0
				);
		if(origin == NULL)
			return FILE_NULL;
		std::string out = origin;
		replace_all(out, "\\", "/");
		return out;
	}

	std::string saveDialog(char const *filter[], int filterNum, const char* desc) {
		char* origin = tinyfd_saveFileDialog(
				"Save File",
				NULL,
				filterNum,
				filter,
				desc
				);
		if(origin == NULL)
			return FILE_NULL;
		std::string out = origin;
		if(!out.ends_with(".CubeAsm")) {
			out += ".CubeAsm";
		}
		replace_all(out, "\\", "/");
		return out;
	}

	namespace file {
		bool loadScene(std::string in, std::vector<Entity>* ents) {
			if(in == FILE_NULL) {
				return false;
			}

			std::ifstream f(in);
			if(!f.is_open()) {
				TraceLog(LOG_ERROR, TextFormat("File not found: %s!", in.c_str()));
				return false;
			}
			Entity temp;
			temp.type = TYPE_NULL;
			std::vector<Entity> newEnts;
			std::vector<std::string> tokens;
			while(std::getline(f, in)) {
				tokens = rad::tokenize(in, ' ');
				#if 0 // Use this switch for debugging tokens
				for(auto i: tokens) {
					fmt::print("{} ", i);
				}
				printf("\n");
				#endif
				if(tokens[0] == "TYPE") {
					if(tokens[1] == "CUBE") {
						temp.type = TYPE_CUBE;
					}
					if(tokens[1] == "SPHERE") {
						temp.type = TYPE_SPHERE;
					}
					if(tokens[1] == "CYLINDER") {
						temp.type = TYPE_CYLINDER;
					}

					/*
					 *	I put this there because I accidentally put ==
					 *	instead of = when setting up the type for a cylinder
					 *	when loading a scene, so it would be read as the wrong
					 *	type and not display the shape.
					 *
					 *	I didn't know this back then and it took me ~1 hour to
					 *	figure this shit out.
					 */
					if(temp.type == TYPE_NULL || 
							(temp.type > 2 && temp.type < 0)) {
						tinyfd_messageBox("ERROR! Type is invalid!",
								TextFormat("Temp type is :%d:", temp.type),
								"ok",
								"info",
								1);
					}
				}

				if(tokens[0] == "POS") {
					if(temp.type == TYPE_CUBE) {
						temp.obj.cube.pos = Vector3 {
							std::stof(tokens[1]),
							std::stof(tokens[2]),
							std::stof(tokens[3])
						};
					}
					if(temp.type == TYPE_SPHERE) {
						temp.obj.sphere.pos = Vector3 {
							std::stof(tokens[1]),
							std::stof(tokens[2]),
							std::stof(tokens[3])
						};
					}
				}

				if(tokens[0] == "SIZE") {
					if(temp.type == TYPE_CUBE) {
						temp.obj.cube.size = Vector3 {
							std::stof(tokens[1]),
							std::stof(tokens[2]),
							std::stof(tokens[3])
						};
					}
				}

				if(tokens[0] == "RAD") {
					if(temp.type == TYPE_SPHERE) {
						temp.obj.sphere.rad = std::stof(tokens[1]);
					}
				}

				if(tokens[0] == "RING") {
					if(temp.type == TYPE_SPHERE) {
						temp.obj.sphere.rings = std::stoi(tokens[1]);
					}
				}

				if(tokens[0] == "SLICE") {
					if(temp.type == TYPE_SPHERE) {
						temp.obj.sphere.slices = std::stoi(tokens[1]);
					}
				}

				if(tokens[0] == "SPOS") {
					if(temp.type == TYPE_CYLINDER) {
						temp.obj.cylinder.startPos = Vector3 {
							std::stof(tokens[1]),
							std::stof(tokens[2]),
							std::stof(tokens[3])
						};
					}
				}

				if(tokens[0] == "EPOS") {
					if(temp.type == TYPE_CYLINDER) {
						temp.obj.cylinder.endPos = Vector3 {
							std::stof(tokens[1]),
							std::stof(tokens[2]),
							std::stof(tokens[3])
						};
					}
				}

				if(tokens[0] == "SRAD") {
					if(temp.type == TYPE_CYLINDER) {
						temp.obj.cylinder.startRadius = std::stof(tokens[1]);
					}
				}

				if(tokens[0] == "ERAD") {
					if(temp.type == TYPE_CYLINDER) {
						temp.obj.cylinder.endRadius = std::stof(tokens[1]);
					}
				}

				if(tokens[0] == "SIDE") {
					if(temp.type == TYPE_CYLINDER) {
						temp.obj.cylinder.sides = std::stoi(tokens[1]);
					}
				}

				if(tokens[0] == "COLOR") {
					if(temp.type == TYPE_CUBE) {
						temp.obj.cube.color = Color{
						(unsigned char)	std::stoi(tokens[1]),
						(unsigned char) std::stoi(tokens[2]),
						(unsigned char) std::stoi(tokens[3]),
						(unsigned char)	std::stoi(tokens[4]),
						};
					}

					if(temp.type == TYPE_SPHERE) {
						temp.obj.sphere.color = Color{
						(unsigned char)	std::stoi(tokens[1]),
						(unsigned char) std::stoi(tokens[2]),
						(unsigned char) std::stoi(tokens[3]),
						(unsigned char)	std::stoi(tokens[4]),
						};
					}

					if(temp.type == TYPE_CYLINDER) {
						temp.obj.cylinder.color = Color{
						(unsigned char)	std::stoi(tokens[1]),
						(unsigned char) std::stoi(tokens[2]),
						(unsigned char) std::stoi(tokens[3]),
						(unsigned char)	std::stoi(tokens[4]),
						};
					}
				}

				if(tokens[0] == "PUSH") {
					newEnts.push_back(temp);
				}
			}
			*ents = newEnts;
			//TODO: make loading shit work
			return true;
		}

		bool saveScene(std::string in, std::vector<Entity> ents) {
			if(in == FILE_NULL) {
				tinyfd_messageBox("Prompt Cancel",
						"The dialog was cancelled. If this was intentional, you can safely"
						" ignore this.",
						"ok",
						"info",
						1
						);
				return false;
			}

			std::ofstream file;
			file.open(in);
			for(Entity c: ents) {
				if(c.type == TYPE_CUBE) {
					file << fmt::format("TYPE CUBE\n");
					file << fmt::format("POS {} {} {} \n",
								c.obj.cube.pos.x,
								c.obj.cube.pos.y,
								c.obj.cube.pos.z
							);
					file << fmt::format("SIZE {} {} {} \n",
								c.obj.cube.size.x,
								c.obj.cube.size.y,
								c.obj.cube.size.z
							);
					file << fmt::format("COLOR {} {} {} {} \n",
								c.obj.cube.color.r,
								c.obj.cube.color.g,
								c.obj.cube.color.b,
								c.obj.cube.color.a
							);
					file << fmt::format("PUSH \n");
				}

				if(c.type == TYPE_SPHERE) {
					file << fmt::format("TYPE SPHERE \n");
					file << fmt::format("POS {} {} {} \n",
								c.obj.sphere.pos.x,
								c.obj.sphere.pos.y,
								c.obj.sphere.pos.z
							);
					file << fmt::format("RAD {} \n", 
								c.obj.sphere.rad
							);
					file << fmt::format("RING {} \n", 
								c.obj.sphere.rings	
							);
					file << fmt::format("SLICE {} \n",
								c.obj.sphere.slices
							);
					file << fmt::format("COLOR {} {} {} {} \n",
								c.obj.sphere.color.r,
								c.obj.sphere.color.g,
								c.obj.sphere.color.b,
								c.obj.sphere.color.a
							);
					file << fmt::format("PUSH \n");
				}

				if(c.type == TYPE_CYLINDER) {
					file << fmt::format("TYPE CYLINDER \n");
					file << fmt::format("SPOS {} {} {} \n",
								c.obj.cylinder.startPos.x,
								c.obj.cylinder.startPos.y,
								c.obj.cylinder.startPos.z
							);
					file << fmt::format("EPOS {} {} {} \n",
								c.obj.cylinder.endPos.x,
								c.obj.cylinder.endPos.y,
								c.obj.cylinder.endPos.z
							);
					file << fmt::format("SRAD {} \n",
								c.obj.cylinder.startRadius
							);
					file << fmt::format("ERAD {} \n",
								c.obj.cylinder.endRadius
							);
					file << fmt::format("SIDE {} \n",
								c.obj.cylinder.sides
							);
					file << fmt::format("COLOR {} {} {} {} \n",
								c.obj.cylinder.color.r,
								c.obj.cylinder.color.g,
								c.obj.cylinder.color.b,
								c.obj.cylinder.color.a
							);
					file << fmt::format("PUSH \n");
				}
			}
			file.close();
			return true;
		}
	}
}
