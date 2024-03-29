
// 1. Bajar freeglut http://www.lighthouse3d.com/tutorials/glut-tutorial/
// 2. Bajar AntTweakBar http://anttweakbar.sourceforge.net/doc/
// 3. Bajar "Open File Dialog", "open"
#define FREEGLUT_STATIC
#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif
// FreeGlut
#include "./GL/freeglut.h"
// ImGui
#include "./imgui/imgui.h"
//#include "imgui/imgui_draw.cpp"
#include "./imgui/backends/imgui_impl_glut.h"
#include "./imgui/backends/imgui_impl_opengl2.h"
// Utils
#include <list>
#include <list_handler.h>
#include <iostream>
#include <string>
#include <memory>
#include <colors.h>
#include <status.h>
#include <utils.h>
#include <vertex2d.h>
#include <line.h>
#include <triangle.h>
#include <rectangle.h>
#include <circle.h>
#include <elipse.h>
#include <bezier.h>

#include <filename.h>
#include <FileManager.h>
#include <algorithm>  // For String to UPPER

string current_file_name = "./Saved Canvas/ShapesFile.txt";
int current_file_mode = 0;

using namespace std;
void MouseStyle(bool state);
void DrawSelectedFigure(int figure, ImVec4 &border_color, ImVec4 &fill_color, shared_ptr<Vertex2D>* _v);
void TEST();

void SaveFile() {
	// Create and open a text file
	ofstream MyFile(current_file_name);
	// Aplication Code
	int i = 0;
	cout << "SAVING FILE" << endl;
	MyFile << "BACKGROUND " << clear_color.x << " " << clear_color.y << " " << clear_color.z << endl;
	cout << "BACKGROUND " << clear_color.x << " " << clear_color.y << " " << clear_color.z << endl;
	for (auto const& x : shapes) {
		// Write to the file
		x->printShape(MyFile);
		//cout << "render shape " << i << endl;
		i++;
	}
	// Close the file
	MyFile.close();
	cout << "FILE SAVED" << endl;
}

void arrange(string str, list<string> &result)
{
	string w = "";
	for (auto x : str)
	{
		if (x == ' ')
		{
			result.push_back(w);
			//cout << w << endl;
			w = "";
		}
		else {
			if (x == '\n' or x == ' ') {
				continue;
			}
			w = w + x;
		}
	}
	result.push_back(w);
	//cout << w << endl;
}

void LoadFile() {
	// Create a text string, which is used to output the text file
	string myText;

	// Read from the text file
	ifstream MyReadFile(current_file_name);

	cout << "LOADING FILE" << endl;
	// Use a while loop together with the getline() function to read the file line by line
	while (getline(MyReadFile, myText)) {
		list<string> command;
		// Output the text from the file
		//toupper(myText);
		transform(myText.begin(), myText.end(), myText.begin(), ::toupper);
		cout << myText << endl;
		try
		{
			//ImVec4 new_fill_color = ImVec4(0.01f, 0.71f, 0.31f, 0.5f);
			//ImVec4 new_border_color = ImVec4(0.1f, 0.17f, 0.13f, 1.00f);
			arrange(myText, command);
			if (command.size() > 1) {
				auto it = command.begin();
				if (it->compare("LINE") == 0) {

					if (command.size() != 8) {
						cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 8 but " << command.size() << " were given." << endl;
						continue;
					}

					FigureClicked = 1;
					isborder = true;
					isfill = true;
					isvertex = true;

					it++;
					int temp = stoi(*it);
					buffer[0]->Xs(temp);

					it++;
					temp = stoi(*it);
					buffer[0]->Ys(temp);

					it++;
					temp = stoi(*it);
					buffer[1]->Xs(temp);

					it++;
					temp = stoi(*it);
					buffer[1]->Ys(temp);

					clicks_on_buffer = 2;

					it++;
					float temp2 = stof(*it);
					new_border_color.x = temp2;

					it++;
					temp2 = stof(*it);
					new_border_color.y = temp2;

					it++;
					temp2 = stof(*it);
					new_border_color.z = temp2;

					DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
					clicks_on_buffer = 0;
				}
				else {
					if (it->compare("CIRCLE") == 0) {

						if (command.size() != 8) {
							cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 8 but " << command.size() << " were given." << endl;
							continue;
						}

						FigureClicked = 2;
						isborder = true;
						isfill = false;
						isvertex = true;

						it++;
						int temp = stoi(*it);
						buffer[0]->Xs(temp);

						it++;
						temp = stoi(*it);
						buffer[0]->Ys(temp);

						it++;
						temp = stoi(*it);
						buffer[1]->Xs(temp);

						it++;
						temp = stoi(*it);
						buffer[1]->Ys(temp);

						clicks_on_buffer = 2;

						it++;
						float temp2 = stof(*it);
						new_border_color.x = temp2;

						it++;
						temp2 = stof(*it);
						new_border_color.y = temp2;

						it++;
						temp2 = stof(*it);
						new_border_color.z = temp2;

						DrawSelectedFigure(FigureClicked, new_border_color, no_fill_color, buffer);
						clicks_on_buffer = 0;
					}
					else {
						if (it->compare("FILLED_CIRCLE") == 0) {

							if (command.size() != 11) {
								cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 11 but " << command.size() << " were given." << endl;
								continue;
							}

							FigureClicked = 2;
							isborder = true;
							isfill = true;
							isvertex = true;


							it++;
							int temp = stoi(*it);
							buffer[0]->Xs(temp);

							it++;
							temp = stoi(*it);
							buffer[0]->Ys(temp);

							it++;
							temp = stoi(*it);
							buffer[1]->Xs(temp);

							it++;
							temp = stoi(*it);
							buffer[1]->Ys(temp);

							clicks_on_buffer = 2;

							it++;
							float temp2 = stof(*it);
							new_border_color.x = temp2;

							it++;
							temp2 = stof(*it);
							new_border_color.y = temp2;

							it++;
							temp2 = stof(*it);
							new_border_color.z = temp2;

							it++;
							temp2 = stof(*it);
							new_fill_color.x = temp2;

							it++;
							temp2 = stof(*it);
							new_fill_color.y = temp2;

							it++;
							temp2 = stof(*it);
							new_fill_color.z = temp2;

							DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
							clicks_on_buffer = 0;
						}
						else {
							if (it->compare("ELLIPSE") == 0) {

								if (command.size() != 8) {
									cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 8 but " << command.size() << " were given." << endl;
									continue;
								}

								FigureClicked = 3;
								isborder = true;
								isfill = false;
								isvertex = true;

								it++;
								int temp = stoi(*it);
								buffer[0]->Xs(temp);

								it++;
								temp = stoi(*it);
								buffer[0]->Ys(temp);

								it++;
								int rx = stoi(*it);
								it++;
								int ry = stoi(*it);

								buffer[1]->XY(buffer[0]->X()+rx, buffer[0]->Y());
								buffer[2]->XY(buffer[0]->X(), buffer[0]->Y()+ry);

								clicks_on_buffer = 3;

								it++;
								float temp2 = stof(*it);
								new_border_color.x = temp2;

								it++;
								temp2 = stof(*it);
								new_border_color.y = temp2;

								it++;
								temp2 = stof(*it);
								new_border_color.z = temp2;

								DrawSelectedFigure(FigureClicked, new_border_color, no_fill_color, buffer);
								clicks_on_buffer = 0;
							}
							else {
								if (it->compare("FILLED_ELLIPSE") == 0) {

									if (command.size() != 11) {
										cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 11 but " << command.size() << " were given." << endl;
										continue;
									}

									FigureClicked = 3;
									isborder = true;
									isfill = true;
									isvertex = true;

									it++;
									int temp = stoi(*it);
									buffer[0]->Xs(temp);

									it++;
									temp = stoi(*it);
									buffer[0]->Ys(temp);

									it++;
									int rx = stoi(*it);
									it++;
									int ry = stoi(*it);

									buffer[1]->XY(buffer[0]->X() + rx, buffer[0]->Y());
									buffer[2]->XY(buffer[0]->X(), buffer[0]->Y() + ry);

									clicks_on_buffer = 3;

									it++;
									float temp2 = stof(*it);
									new_border_color.x = temp2;

									it++;
									temp2 = stof(*it);
									new_border_color.y = temp2;

									it++;
									temp2 = stof(*it);
									new_border_color.z = temp2;

									it++;
									temp2 = stof(*it);
									new_fill_color.x = temp2;

									it++;
									temp2 = stof(*it);
									new_fill_color.y = temp2;

									it++;
									temp2 = stof(*it);
									new_fill_color.z = temp2;

									DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
									clicks_on_buffer = 0;
								}
								else {
									if (it->compare("RECTANGLE") == 0) {

										if (command.size() != 8) {
											cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 8 but " << command.size() << " were given." << endl;
											continue;
										}

										FigureClicked = 4;
										isborder = true;
										isfill = false;
										isvertex = true;

										it++;
										int temp = stoi(*it);
										buffer[0]->Xs(temp);

										it++;
										temp = stoi(*it);
										buffer[0]->Ys(temp);

										it++;
										temp = stoi(*it);
										buffer[1]->Xs(temp);

										it++;
										temp = stoi(*it);
										buffer[1]->Ys(temp);
										clicks_on_buffer = 2;

										it++;
										float temp2 = stof(*it);
										new_border_color.x = temp2;

										it++;
										temp2 = stof(*it);
										new_border_color.y = temp2;

										it++;
										temp2 = stof(*it);
										new_border_color.z = temp2;

										DrawSelectedFigure(FigureClicked, new_border_color, no_fill_color, buffer);
										clicks_on_buffer = 0;
									}
									else {
										if (it->compare("FILLED_RECTANGLE") == 0) {

											if (command.size() != 11) {
												cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 11 but " << command.size() << " were given." << endl;
												continue;
											}

											FigureClicked = 4;
											isborder = true;
											isfill = true;
											isvertex = true;

											it++;
											int temp = stoi(*it);
											buffer[0]->Xs(temp);

											it++;
											temp = stoi(*it);
											buffer[0]->Ys(temp);

											it++;
											temp = stoi(*it);
											buffer[1]->Xs(temp);

											it++;
											temp = stoi(*it);
											buffer[1]->Ys(temp);
											clicks_on_buffer = 2;

											it++;
											float temp2 = stof(*it);
											new_border_color.x = temp2;

											it++;
											temp2 = stof(*it);
											new_border_color.y = temp2;

											it++;
											temp2 = stof(*it);
											new_border_color.z = temp2;

											it++;
											temp2 = stof(*it);
											new_fill_color.x = temp2;

											it++;
											temp2 = stof(*it);
											new_fill_color.y = temp2;

											it++;
											temp2 = stof(*it);
											new_fill_color.z = temp2;

											DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
											clicks_on_buffer = 0;
										}
										else {
											if (it->compare("TRIANGLE") == 0) {

												if (command.size() != 10) {
													cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 10 but " << command.size() << " were given." << endl;
													continue;
												}

												FigureClicked = 5;
												isborder = true;
												isfill = false;
												isvertex = true;

												it++;
												int temp = stoi(*it);
												buffer[0]->Xs(temp);

												it++;
												temp = stoi(*it);
												buffer[0]->Ys(temp);

												it++;
												temp = stoi(*it);
												buffer[1]->Xs(temp);

												it++;
												temp = stoi(*it);
												buffer[1]->Ys(temp);

												it++;
												temp = stoi(*it);
												buffer[2]->Xs(temp);

												it++;
												temp = stoi(*it);
												buffer[2]->Ys(temp);
												clicks_on_buffer = 3;

												it++;
												float temp2 = stof(*it);
												new_border_color.x = temp2;

												it++;
												temp2 = stof(*it);
												new_border_color.y = temp2;

												it++;
												temp2 = stof(*it);
												new_border_color.z = temp2;

												DrawSelectedFigure(FigureClicked, new_border_color, no_fill_color, buffer);
												clicks_on_buffer = 0;
											}
											else {
												if (it->compare("FILLED_TRIANGLE") == 0) {

													if (command.size() != 13) {
														cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 13 but " << command.size() << " were given." << endl;
														continue;
													}

													FigureClicked = 5;
													isborder = true;
													isfill = true;
													isvertex = true;

													it++;
													int temp = stoi(*it);
													buffer[0]->Xs(temp);

													it++;
													temp = stoi(*it);
													buffer[0]->Ys(temp);

													it++;
													temp = stoi(*it);
													buffer[1]->Xs(temp);

													it++;
													temp = stoi(*it);
													buffer[1]->Ys(temp);

													it++;
													temp = stoi(*it);
													buffer[2]->Xs(temp);

													it++;
													temp = stoi(*it);
													buffer[2]->Ys(temp);
													clicks_on_buffer = 3;

													it++;
													float temp2 = stof(*it);
													new_border_color.x = temp2;

													it++;
													temp2 = stof(*it);
													new_border_color.y = temp2;

													it++;
													temp2 = stof(*it);
													new_border_color.z = temp2;

													it++;
													temp2 = stof(*it);
													new_fill_color.x = temp2;

													it++;
													temp2 = stof(*it);
													new_fill_color.y = temp2;

													it++;
													temp2 = stof(*it);
													new_fill_color.z = temp2;

													DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
													clicks_on_buffer = 0;
												}
												else {
													if ((it->substr(0, 6)).compare("BEZIER") == 0) {

														FigureClicked = 6;
														isborder = true;
														isfill = true;
														isvertex = true;

														string points = (it->substr(6, it->length() - 6));
														int temp = stoi(points);

														if (temp < 2 or temp > 20) {
															cout << "ERROR: INVALID AMOUNT OF POINTS FOR THIS COMMAND. Must be BETWEEN 2 AND 20 but " << temp << " were given." << endl;
															continue;
														}
														if (command.size() != 4+(temp*2)) {
															cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be " << 4 + (temp * 2) << " but " << command.size() << " were given." << endl;
															continue;
														}

														for (int p = 0; p < temp; p++) {
															it++;
															int temp2 = stoi(*it);
															buffer[p]->Xs(temp2);

															it++;
															temp2 = stoi(*it);
															buffer[p]->Ys(temp2);
														}
														clicks_on_buffer = temp;

														it++;
														float temp2 = stof(*it);
														new_border_color.x = temp2;

														it++;
														temp2 = stof(*it);
														new_border_color.y = temp2;

														it++;
														temp2 = stof(*it);
														new_border_color.z = temp2;

														DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
														clicks_on_buffer = 0;
													}
													else {
														if (it->compare("BACKGROUND") == 0) {
															if (command.size() != 4) {
																cout << "ERROR: INVALID AMOUNT OF ARGUMENTS FOR THIS COMMAND. Must be 4 but " << command.size() << " were given." << endl;
																continue;
															}

															it++;
															float temp1, temp2, temp3;
															temp1 = stof(*it);
															if (temp1 < 0.0 or temp1 > 1.0) {
																cout << "ERROR: \"" << temp1 << "\" is not a valid red BACKGROUND color component." << endl;
															}
															else{
																it++;
																temp2 = stof(*it);
																if (temp2 < 0.0 or temp2 > 1.0) {
																	cout << "ERROR: \"" << temp2 << "\" is not a valid blue BACKGROUND color component." << endl;
																}
																else {
																	it++;
																	temp3 = stof(*it);
																	if (temp3 < 0.0 or temp3 > 1.0) {
																		cout << "ERROR: \"" << temp3 << "\" is not a valid green BACKGROUND color component." << endl;
																	}
																	else {
																		FigureClicked = 0;
																		isborder = true;
																		isfill = true;
																		isvertex = true;
																		if (current_shape != NULL) {
																			lastClickedVertex = NULL;
																			current_shape->isSelected = false;
																			current_shape = NULL;
																		}

																		clear_color.x = temp1;
																		clear_color.y = temp2;
																		clear_color.z = temp3;

																		clicks_on_buffer = 0;
																	}
																}
															}
														}
														else {
															cout << "Line \"" << myText << "\" was not recognized." << endl;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else {
			cout << "ERROR: Arguments must be greater than 1." << endl;
			}
		}
		catch (const std::exception&)
		{
			cout << endl << "UNEXPECTED ERROR" << endl;
			continue;
		}
	}

	// Close the file
	MyReadFile.close();
	cout << "FILE LOADED" << endl;
}

// [SECTION] Display
void my_display_code()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_config_window)
		ImGui::ShowDemoWindow(&show_config_window, width, height);
	{
		ImGui::Begin("ToolBar!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is the useful toolbar!.");               // Display some text (you can use a format strings too)
		ImGui::Separator();
		ImGui::ColorEdit3("clear (background) color", (float*)&clear_color); // Edit 3 floats representing a color

		ImGui::Separator();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Mode Selector:");
		ImGui::SameLine();
		ImGui::RadioButton("Hardware", &DrawingMode, 0); ImGui::SameLine();
		ImGui::RadioButton("Software", &DrawingMode, 1);
		//cout << "Modo: " << FigureClicked << endl;

		ImGui::Separator();

		const char* figures[] = { "Select", "Line", "Circle", "Elipse", "Rectangle", "Triangle", "Bezier", "DELETE", "MOVE", "SELECT VERTEX", "MOVE VERTEX", NULL};
		ImGui::Text("Figure Selector: %s", figures[FigureClicked]);
		
		const char* figure;
		// Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
		for (int i = 0; i < 8; i++)
		{
			figure = *figures + i;
			if (i > 0)
				ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
			if (ImGui::Button(figures[i])) {
				FigureClicked = i;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		
		ImGui::Checkbox("Border", &isborder);
		ImGui::SameLine();
		ImGui::ColorEdit4("Border color", (float*)&new_border_color);
		

		ImGui::Checkbox("Fill", &isfill);
		ImGui::SameLine();
		ImGui::ColorEdit4("Fill color", (float*)&new_fill_color);

		ImGui::Checkbox("Vertex", &isvertex);
		ImGui::SameLine();
		ImGui::ColorEdit4("Vertex color", (float*)&vertex_color_unselected);

		ImGui::Separator();

		//cout << "current_shape: " << current_shape << endl;

		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

		if (current_shape != NULL) {
			
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Selected Figure:  %d", current_shape);
			ImGui::SameLine(0.0f, spacing);
			// Trying to give it color
			ImGui::PushID(11);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));
			ImGui::PopStyleColor(3);

			if (ImGui::Button("Delete"))
			{
				//Handle Delete
				erasepos(position, shapes);
				position = -1;//shapes.size();//-1;//
				current_shape = NULL;//*(shapes.end());//NULL;//
			}
			ImGui::PopID();

			// See 'Demo->Layout->Text Baseline Alignment' for details.
			ImGui::AlignTextToFramePadding();
			//ImGui::Text("Figure Position: %d", position);
			ImGui::Text("Figure Position:");
			ImGui::SameLine();

			// Arrow buttons with Repeater
			if (ImGui::Button("To Back"))
			{
				toBack(position, shapes);
				//position = 1;
			}
			ImGui::SameLine(0.0f, spacing);
			ImGui::PushButtonRepeat(true);
			if (ImGui::ArrowButton("##back", ImGuiDir_Left)) { decposition(position, shapes); }
			ImGui::SameLine(0.0f, spacing);
			ImGui::Text("%d", position);
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::ArrowButton("##front", ImGuiDir_Right)) { incposition(position, shapes);  }
			ImGui::PopButtonRepeat();
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::Button("To Front"))
			{
				toFront(position, shapes);
				//position = shapes.size();
			}

			ImGui::Checkbox("Border ", &current_shape->drawBorder);
			ImGui::SameLine();
			ImGui::ColorEdit4("Border color ", (float*)&current_shape->border_color);

			ImGui::Checkbox("Fill ", &current_shape->drawFill);
			ImGui::SameLine();
			ImGui::ColorEdit4("Fill color ", (float*)&current_shape->fill_color);

			ImGui::Checkbox("Vertex ", &current_shape->drawVertex);
			ImGui::SameLine();
			ImGui::ColorEdit4("Vertex color ", (float*)&current_shape->vertex_color_original);

			ImGui::Checkbox("Additional Settings:", &extraUI);

			if (extraUI) {
				if (DrawingMode == 0)
					ImGui::SliderFloat("Border Width", &current_shape->borderWidth, 1.0, 15.0);
				else
					ImGui::SliderFloat("Border Width", &current_shape->borderWidthS, 1.0, 15.0);
				ImGui::SliderFloat("Vertex Size", &current_shape->vertexSize, 1.0, 15.0);

				if (current_shape->ID == 6) {
					double step = 0.000001;
					double hardstep = 0.0001;
					// Bezier
					ImGui::AlignTextToFramePadding();
					//ImGui::Text("Figure Position: %d", position);
					ImGui::Text("Curve Precision:");
					ImGui::SameLine();
					ImGui::PushButtonRepeat(true);
					if (ImGui::ArrowButton("##--", ImGuiDir_Down)) { if (current_shape->PRECISION - hardstep > 0) current_shape->PRECISION = current_shape->PRECISION - hardstep; }
					ImGui::SameLine(0.0f, spacing);
					if (ImGui::ArrowButton("##-", ImGuiDir_Left)) { if (current_shape->PRECISION - step > 0) current_shape->PRECISION = current_shape->PRECISION - step; }

					ImGui::SameLine(0.0f, spacing);
					float fp = (float)current_shape->PRECISION;
					if (ImGui::SliderFloat("", &fp, 0.00001f, 1.0000f, "% .6f")) {
						double temp = current_shape->PRECISION;
						current_shape->PRECISION = (double)fp;
						if (current_shape->PRECISION == 0.0) {
							current_shape->PRECISION = temp;
						}
					}
					//ImGui::Text("%f", current_shape->PRECISION);
					ImGui::SameLine(0.0f, spacing);
					if (ImGui::ArrowButton("##+", ImGuiDir_Right)) { if (current_shape->PRECISION + step <= 1)current_shape->PRECISION = current_shape->PRECISION + step; }
					ImGui::SameLine(0.0f, spacing);
					if (ImGui::ArrowButton("##++", ImGuiDir_Up)) { if (current_shape->PRECISION + hardstep <= 1) current_shape->PRECISION = current_shape->PRECISION + hardstep; }
					ImGui::PopButtonRepeat();
				}
			}
			ImGui::Separator();
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Extras:");
		ImGui::SameLine();

		if (ImGui::Button("Select Figure"))
		{
			//Handle Select Figure
			FigureClicked = 0;
		}
		if (current_shape != NULL) {
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::Button("Move Selected Figure"))
			{
				//Handle Move Selected 
				FigureClicked = 8;
			}
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::Button("Select Vertex"))
			{
				//Handle Select Vertex
				FigureClicked = 9;
			}
			if (lastClickedVertex != NULL) {
				ImGui::SameLine(0.0f, spacing);
				if (ImGui::Button("Move Selected Vertex"))
				{
					//Handle Move Selected Vertex
					FigureClicked = 10;
				}
			}
		}
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Clear All"))
		{
			//Handle Clear All
			//printf("[%d] current_file_name: %s\n", current_file_mode, current_file_name.c_str());
			position = -1;//shapes.size();//-1;//
			current_shape = NULL;//*(shapes.end());//NULL;//
			clearALL(shapes);
		}

		//cout << "FigureClicked: " << FigureClicked << endl;
		// Check if we need to draw a figure
		if (FigureClicked == 0 or FigureClicked == 9) {
			// Select which figure to draw
			ImGui::SetMouseCursor(7); // HAND
		}
		else {
			if (FigureClicked == 7) {
				ImGui::SetMouseCursor(8); // DIAGONAL CROSS
			}
			else
			{
				if (FigureClicked == 8 or FigureClicked == 10) {
					ImGui::SetMouseCursor(2); // RESIZE ALL
				}
				else {
					if (FigureClicked > 0) {
						// Select which figure to draw
						ImGui::SetMouseCursor(0); // Arrow
					}
					else {
						// handle Deselect
						ImGui::SetMouseCursor(0); // Arrow
					}
				}
			}
		}
		ImGui::Separator();
		ImGui::Checkbox("Config Window", &show_config_window);
		ImGui::End();
	}

	//if (show_another_window) {
		//ShowExampleAppMainMenuBar();
	//}
}


void renderScene(void)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	my_display_code();
	
	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	//glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	// glClearColor(0, 0, 0, 1);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT); // For some unknown reason this clears all
	glColor3f(1.0f, 0.5f, 0.25f);

	// Aplication Code
	int i = 0;
	for (auto const& x : shapes) {
		x->render(DrawingMode);
		//cout << "render shape " << i << endl;
		i++;
	}

	// Draw clicked vertexs
	for(int i = 0; i < clicks_on_buffer; i++){
		aux->setColor4(vertex_color_unseted.x, vertex_color_unseted.y, vertex_color_unseted.z, vertex_color_unseted.w);
		aux->putPixel(buffer[i], aux->vertexSize);
	}

	if (vertex_per_figure[FigureClicked] <= clicks_on_buffer) {
		clicks_on_buffer = 0;
	}

	if (current_file_mode != 0) {
		if (current_file_mode == 1) {
			// LOAD FILE
			LoadFile();
			current_file_mode = 0; // Reset File Mode
		}
		else {
			if (current_file_mode == 2) {
				// SAVE FILE
				SaveFile();
				current_file_mode = 0; // Reset File Mode
			}
		}
	}
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();
}

// new Figures
shared_ptr<CLine> new_line;
shared_ptr<CCircle> new_c;
shared_ptr<CElipse> new_e;
shared_ptr<CRectangle> new_r;
shared_ptr<CTriangle> new_t;
shared_ptr<CBezier> new_b;

void DetermineWasClicked(shared_ptr<Vertex2D>* _v) {
	int i;
	bool wasClicked = false;
	list <shared_ptr<CShape>>::iterator x = shapes.end();
	i = shapes.size();
	if (i > 0) x--;
	for (; i > 0; i--) {
		if (!wasClicked) {
			if ((*x)->onClick(_v[0]->X(), _v[0]->Y()))
			{
				current_shape = (*x);
				position = i;
				wasClicked = true;
				//break;
			}
		}
		else {
			(*x)->isSelected = false;
		}
		if (i > 1) x--;
	}
	if (!wasClicked) {
		// If it cames here, then none figure where selected
		current_shape = NULL;
		position = -1;
	}
}

bool isFigureValid(int n, shared_ptr<Vertex2D>* _v, bool isBorder, ImVec4 &border_color, bool isFill, ImVec4 &fill_color) {
	bool isValid = true;

	for (int i = 0; i < n; i++) {
		if(_v[i]->X() < 0 or _v[i]->Y() < 0) {
			cout << "ERROR: \"" << _v[i]->X() << ", " << _v[i]->Y() << "\" is not a valid coordinate." << endl;
			isValid = false;
		}
	}

	if (isBorder) {
		if (border_color.x < 0.0 or border_color.x > 1.0) {
			cout << "ERROR: \"" << border_color.x << "\" is not a valid red border color component." << endl;
			border_color.x = 0.0;
			isValid = false;
		}
		if (border_color.y < 0.0 or border_color.y > 1.0) {
			cout << "ERROR: \"" << border_color.y << "\" is not a valid green border color component." << endl;
			border_color.y = 0.0;
			isValid = false;
		}
		if (border_color.z < 0.0 or border_color.z > 1.0) {
			cout << "ERROR: \"" << border_color.z << "\" is not a valid blue border color component." << endl;
			border_color.z = 0.0;
			isValid = false;
		}
		if (border_color.w < 0.0 or border_color.w > 1.0) {
			cout << "ERROR: \"" << border_color.w << "\" is not a valid alpha border color component." << endl;
			border_color.w = 0.0;
			isValid = false;
		}
	}

	if (isFill) {
		if (fill_color.x < 0.0 or fill_color.x > 1.0) {
			cout << "ERROR: \"" << fill_color.x << "\" is not a valid red fill color component." << endl;
			isValid = false;
			fill_color.x = 0.0;
		}
		if (fill_color.y < 0.0 or fill_color.y > 1.0) {
			cout << "ERROR: \"" << fill_color.y << "\" is not a valid green fill color component." << endl;
			isValid = false;
			fill_color.y = 0.0;
		}
		if (fill_color.z < 0.0 or fill_color.z > 1.0) {
			cout << "ERROR: \"" << fill_color.z << "\" is not a valid blue fill color component." << endl;
			isValid = false;
			fill_color.z = 0.0;
		}
		if (fill_color.w < 0.0 or fill_color.w > 1.0) {
			cout << "ERROR: \"" << fill_color.w << "\" is not a valid alpha fill color component." << endl;
			isValid = false;
			fill_color.w = 0.0;
		}
	}

	return isValid;

}

// [SECTION] Selectors Handlers
void DrawSelectedFigure(int figure, ImVec4 &border_color, ImVec4 &fill_color, shared_ptr<Vertex2D>* _v){

	if (current_shape != NULL) {
		current_shape->isSelected = false;
	}

	if (isFigureValid(clicks_on_buffer, _v, isborder, border_color, isfill, fill_color)) {
		switch (figure)
		{
		case 0: // Determine if a shape were clicked
			DetermineWasClicked(_v);
			break;
		case 1: // Line
			new_line = make_shared <CLine>(new_border_color);
			new_line->set(_v[0]->X(), _v[0]->Y(), _v[1]->X(), _v[1]->Y());

			new_line->drawBorder = isborder;
			new_line->drawFill = isfill;
			new_line->drawVertex = isvertex;

			shapes.push_back(new_line);
			current_shape = new_line;
			position = shapes.size();


			lastClickedVertex = current_shape->LastVertex();

			break;

		case 2: // Circle
			new_c = make_shared <CCircle>(new_border_color, new_fill_color);
			new_c->set(_v[0]->X(), _v[0]->Y(), _v[1]->X(), _v[1]->Y());

			new_c->drawBorder = isborder;
			new_c->drawFill = isfill;
			new_c->drawVertex = isvertex;

			shapes.push_back(new_c);
			current_shape = new_c;
			position = shapes.size();

			lastClickedVertex = current_shape->LastVertex();

			break;

		case 3: // Elipse
			new_e = make_shared <CElipse>(new_border_color, new_fill_color);
			new_e->set(_v[0]->X(), _v[0]->Y(), _v[1]->X(), _v[1]->Y(), _v[2]->X(), _v[2]->Y());

			new_e->drawBorder = isborder;
			new_e->drawFill = isfill;
			new_e->drawVertex = isvertex;

			shapes.push_back(new_e);
			current_shape = new_e;
			position = shapes.size();

			lastClickedVertex = current_shape->LastVertex();

			break;

		case 4: // Rectangle
			new_r = make_shared <CRectangle>(new_border_color, new_fill_color);
			//r1->set(500, 100, 100, 500, 100, 100, 500, 500);
			new_r->set(_v[0]->X(), _v[0]->Y(), _v[1]->X(), _v[1]->Y());

			new_r->drawBorder = isborder;
			new_r->drawFill = isfill;
			new_r->drawVertex = isvertex;

			shapes.push_back(new_r);
			current_shape = new_r;
			position = shapes.size();

			lastClickedVertex = current_shape->LastVertex();

			break;

		case 5: // Triangle
			new_t = make_shared <CTriangle>(new_border_color, new_fill_color);
			new_t->set(_v[0]->X(), _v[0]->Y(), _v[1]->X(), _v[1]->Y(), _v[2]->X(), _v[2]->Y());

			new_t->drawBorder = isborder;
			new_t->drawFill = isfill;
			new_t->drawVertex = isvertex;

			shapes.push_back(new_t);
			current_shape = new_t;
			position = shapes.size();

			lastClickedVertex = current_shape->LastVertex();

			break;

		case 6: // Bezier
			new_b = make_shared <CBezier>(new_border_color, new_fill_color);
			for (int i = 0; i < clicks_on_buffer; i++)
				new_b->addVertex(_v[i]->X(), _v[i]->Y());

			new_b->drawBorder = isborder;
			new_b->drawFill = isfill;
			new_b->drawVertex = isvertex;

			shapes.push_back(new_b);
			current_shape = new_b;
			position = shapes.size();

			lastClickedVertex = current_shape->LastVertex();

			break;
		case 7: // Delete
			DetermineWasClicked(_v);
			if (current_shape != NULL) {
				erasepos(position, shapes);
				position = -1;//shapes.size();//-1;//
				current_shape = NULL;//*(shapes.end());//NULL;//
			}

			lastClickedVertex = NULL;

			break;
		case 8: // Move Shape
			//DetermineWasClicked(_v);
			if (current_shape != NULL) {
				current_shape->onMove(_v[1]->X() - _v[0]->X(), _v[1]->Y() - _v[0]->Y());
				lastClickedVertex = current_shape->LastVertex();
			}
			break;
		case 9: // Select Vertex
			//DetermineWasClicked(_v);
			if (current_shape != NULL) {
				lastClickedVertex = current_shape->selectedVertex(_v[0]->X(), _v[0]->Y());
			}
			break;
		case 10: // Move Selected Vertex
			// DetermineWasClicked(_v);
			if (current_shape != NULL) {
				if (lastClickedVertex != NULL) {
					if (lastClickedVertex != NULL) {
						current_shape->MoveVertex(lastClickedVertex, _v[1]->X() - _v[0]->X(), _v[1]->Y() - _v[0]->Y());
					}
				}
			}
			break;
		default:
			break;
		}
	}
		if (current_shape != NULL) {
			current_shape->isSelected = true;
		}
}

// [SECTION] Input Event Handlers
void onClickCanvas(int button, int state, int x, int y) {
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (FigureClicked == 6) {
			// Bezier
			buffer[clicks_on_buffer]->XY(x, (height - y));
			clicks_on_buffer++;
			DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
			clicks_on_buffer = 0;
		}

	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Store where the user clicked, note Y is backwards.
		if (clicks_on_buffer > 19) {
			// Exceeds the maximum number of points allowed per figure
			DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
			clicks_on_buffer = 0;
		}
		else {
			buffer[clicks_on_buffer]->XY(x, (height - y)); 
			clicks_on_buffer++;
			if (vertex_per_figure[FigureClicked] == clicks_on_buffer) {
				DrawSelectedFigure(FigureClicked, new_border_color, new_fill_color, buffer);
				clicks_on_buffer = 0;
			}
		}
	}
	

}

void onClick(int button, int state, int x, int y)
{
	//Button: 0 left, 1 middle, 2 right
	//State: 0 press down, 1 up
	ImGuiIO& io = ImGui::GetIO();

	//printf("click button: %d, state: %d, x: %d, y: %d\n", button, state, x, y);

	if (io.WantCaptureMouse) {
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	}
	else {
		onClickCanvas(button, state, x, y);
	}
}

void onMotionCanvas(int x, int y) {
	//Do Somenting
	//printf("Motion x: %d, y: %d\n", x, y);
	/*
	if (isDrawingFigure) {
		lastClickedVertex = make_shared <Vertex2D>(x, y);
	}
	*/
}

void onMotion(int x, int y)
{

	ImGuiIO& io = ImGui::GetIO();
	const int maxRadius = 1; // in pixels

	//printf("Motion x: %d, y: %d\n", x, y);

	if (io.WantCaptureMouse)
	{
		ImGui_ImplGLUT_MotionFunc( x, y);
	}

	else {
		onMotionCanvas(x, y);
	}

}

void onPassiveMotionCanvas(int x, int y) {
	//Do Somenting
	/*
	if (isDrawingFigure) {
		lastClickedVertex = make_shared <Vertex2D>(x, y);
	}
	*/
}

void onPassiveMotion(int x, int y)
{

	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent((float)x, (float)y);
	const int maxRadius = 1; // in pixels

	//printf("Passive Motion x: %d, y: %d\n", x, y);

	if (io.WantCaptureMouse) //IDK which event correspond
	{
		//printf("Passive Motion x: %d, y: %d\n", x, y);
		ImGui_ImplGLUT_MotionFunc(x, y);
	}

	else {
		onPassiveMotionCanvas(x, y);
	}

}

void onKeyboardEntryCanvas(unsigned char key, int x, int y) {
	//Do Somenting
	if(key == (unsigned char)'0'){
		cout << "Change to Select Figure Tool" << endl;
		FigureClicked = 0;
	}
	else{
		if (key == (unsigned char)'1') {
			cout << "Change to Draw Line Tool" << endl;
			FigureClicked = 1;
		}
		else {
			if (key == (unsigned char)'2') {
				cout << "Change to Draw Circle Tool" << endl;
				FigureClicked = 2;
			}
			else {
				if (key == (unsigned char)'3') {
					cout << "Change to Draw Elipse Tool" << endl;
					FigureClicked = 3;
				}
				else {
					if (key == (unsigned char)'4') {
						cout << "Change to Draw Rectangle Tool" << endl;
						FigureClicked = 4;
					}
					else {
						if (key == (unsigned char)'5') {
							cout << "Change to Draw Triangle Tool" << endl;
							FigureClicked = 5;
						}
						else {
							if (key == (unsigned char)'6') {
								cout << "Change to Draw Bezier Tool" << endl;
								FigureClicked = 6;
							}
							else {
								if (key == (unsigned char)'7') {
									cout << "Change to Erase Figure Tool" << endl;
									FigureClicked = 7;
								}
								else {
									if (key == (unsigned char)'8') {
										if (current_shape != NULL) {
											cout << "Change to Move Figure Tool" << endl;
											FigureClicked = 8;
										}
									}
									else {
										if (key == (unsigned char)'9') {
											if (current_shape != NULL) {
												FigureClicked = 9;
												cout << "Change to Select Vertex Tool" << endl;
											}
										}
										else {
											if (key == (unsigned char)'*') {
												if (current_shape != NULL) {
													if (lastClickedVertex != NULL) {
														FigureClicked = 10;
														cout << "Change to Move Selected Vertex Tool" << endl;
													}
												}
											}
											else {
												if (key == (unsigned char)'c') {
													cout << "Change border color" << endl;
													/*if (current_shape != NULL) { //BUGGED
																											ImGui::Begin("Change border color!");
														ImGui::Checkbox("Border:  ", &current_shape->drawBorder);
														ImGui::SameLine();
														ImGui::ColorEdit4("Border color:  ", (float*)&current_shape->border_color);
														ImGui::End();
													}*/
												}
												else {
													if (key == (unsigned char)'f') {
														cout << "Change fill color" << endl;
														/*if (current_shape != NULL) {  //BUGGED
															ImGui::Begin("Change fill color!");
															ImGui::Checkbox("Fill:  ", &current_shape->drawFill);
															ImGui::SameLine();
															ImGui::ColorEdit4("Fill color:  ", (float*)&current_shape->fill_color);
															ImGui::End();
														}*/
													}
													else {
														if (key == (unsigned char)'h') {
															cout << "Change display mode" << endl;
															if (DrawingMode == 0)
																DrawingMode = 1;
															else
																DrawingMode = 0;
															//DrawingMode = ~DrawingMode;
														}
														else {
															if (key == (unsigned char)'F') {
																cout << "Selected Figure to Front" << endl;
																if (current_shape != NULL) {
																	toFront(position, shapes);
																}
															}
															else {
																if (key == (unsigned char)'B') {
																	cout << "Selected Figure to Back" << endl;
																	if (current_shape != NULL) {
																		toBack(position, shapes);
																	}
																}
																else {
																	if (key == (unsigned char)'+') {
																		cout << "Selected Figure Front + 1" << endl;
																		if (current_shape != NULL) {
																			incposition(position, shapes);
																		}
																	}
																	else {
																		if (key == (unsigned char)'-') {
																			cout << "Selected Figure Back - 1" << endl;
																			if (current_shape != NULL) {
																				decposition(position, shapes);
																			}
																		}
																		else {
																			if (key == (unsigned char)'u') {
																				cout << "Unselect Figure" << endl;
																				if (current_shape != NULL) {
																					current_shape->isSelected = false;
																				}
																				current_shape = NULL;
																				position = -1;
																			}
																			else {
																				if (key == (unsigned char)'d') {
																					cout << "Delete Figure" << endl;
																					if (current_shape != NULL) {
																						current_shape->isSelected = false;
																						// Handle delete
																						erasepos(position, shapes);
																						position = -1;//shapes.size();//-1;//
																						current_shape = NULL;//*(shapes.end());//NULL;//
																					}
																					//current_shape = NULL;
																					//position = -1;
																				}
																				else {
																					if (key == (unsigned char)'S') {
																						cout << "Save CANVAS" << endl;
																						// Handle Save
																						FileManager* fm = new FileManager();
																						//fm->save(true);
																						fm->save(false);
																						fm->~FileManager();
																						SaveFile();
																						current_file_mode = 0;
																					}
																					else {
																						if (key == (unsigned char)'L') {
																							cout << "Load CANVAS" << endl;
																							// Handle Load
																							FileManager* fm = new FileManager();
																							fm->open();
																							fm->~FileManager();
																							LoadFile();
																							current_file_mode = 0;
																						}
																						else {
																							if (key == (unsigned char)'x') {
																								cout << "Clear ALL" << endl;
																								// Handle Clear ALL
																								position = -1;//shapes.size();//-1;//
																								current_shape = NULL;//*(shapes.end());//NULL;//
																								clearALL(shapes);
																							}
																							else {
																								if (key == (unsigned char)'T') {
																									//TEST
																									printf("[%d] current_file_name: %s\n", current_file_mode, current_file_name.c_str());
																								}
																								else
																								{
																									if (key == (unsigned char)'t') {
																										// TEST CANVAS
																										// Clear ALL
																										position = -1;//shapes.size();//-1;//
																										current_shape = NULL;//*(shapes.end());//NULL;//
																										clearALL(shapes);
																										TEST();
																									}
																									else {
																										cout << "\"" << key << "\" is not a valid shortcut" << endl;
																									}
																								}
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void onKeyboardEntry(unsigned char key, int x, int y)
{

	ImGuiIO& io = ImGui::GetIO();
	const int maxRadius = 1; // in pixels

	//printf("char_down_func %d '%c'\n", key, key);

	if (io.WantCaptureKeyboard)
	{
		ImGui_ImplGLUT_KeyboardFunc(key, x, y);
		onKeyboardEntryCanvas(key, x, y);
	}

	else {
		onKeyboardEntryCanvas(key, x, y);
	}

}

void changeSize(int w, int h)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);

	if (h == 0)
		return;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	glOrtho(0, w, 0, h, -1, 1);

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	width = w;
	height = h;
}

void idle() {
	// force a redraw
	// so we get through as many frames as possible (needed for things like blinking of Move tool)
	glutPostRedisplay();
}

void TESTLINES() {
	shared_ptr<CLine> l1 = make_shared <CLine>(1, 1, 1);
	shared_ptr<CLine> l2 = make_shared <CLine>(1, 0, 0);

	l1->set(100, 100, 200, 200);
	l2->set(200, 200, 299, 149);

	shapes.push_back(l1);
	shapes.push_back(l2);

	shared_ptr<CLine> l3 = make_shared <CLine>(0, 0, 1);
	shared_ptr<CLine> l4 = make_shared <CLine>(0, 0, 0);

	l3->set(299, 149, 120, 54);
	l4->set(120, 54, 100, 100);

	shapes.push_back(l3);
	shapes.push_back(l4);
}

void TESTTRIANGLES() {
	//shared_ptr<CTriangle> t1 = make_shared <CTriangle>(1, 0, 1);
	shared_ptr<CTriangle> t1 = make_shared <CTriangle>(new_border_color, new_fill_color);
	t1->set(200, 200, 300, 300, 200, 300);
	shapes.push_back(t1);
	/*
	shared_ptr<CTriangle> t1b = make_shared <CTriangle>(new_border_color);
	t1b->set(200, 200, 300, 300, 200, 300);
	shapes.push_back(t1b);
	*/

	shared_ptr<CTriangle> t2 = make_shared <CTriangle>(0, 1, 1);
	t2->set(300, 300, 300, 400, 400, 300);
	shapes.push_back(t2);

	shared_ptr<CTriangle> t3 = make_shared <CTriangle>(new_border_color);
	t3->set(300, 300, 300, 400, 400, 400);
	shapes.push_back(t3);
}

void TESTRECTANGLES() {
	shared_ptr<CRectangle> r1 = make_shared <CRectangle>(new_border_color);
	//r1->set(500, 100, 100, 500, 100, 100, 500, 500);
	r1->set(100, 100, 500, 500);
	shapes.push_back(r1);

	//shared_ptr<CRectangle> r2 = make_shared <CRectangle>(new_border_color, new_fill_color);
	//r2->set(700, 500, 500, 700, 300, 700, 700, 300);
	//shapes.push_back(r2);

	shared_ptr<CRectangle> r3 = make_shared <CRectangle>(new_border_color, new_fill_color);
	//r3->set(400, 400, 400, 500, 500, 500, 500, 400);
	r3->set(300, 300, 350, 450);
	shapes.push_back(r3);

	/*
	shared_ptr<CRectangle> r3b = make_shared <CRectangle>(new_border_color);
	//r3->set(400, 400, 400, 500, 500, 500, 500, 400);
	r3b->set(300, 300, 350, 450);
	shapes.push_back(r3b);
	*/
}

void TESTCIRCLES() {
	shared_ptr<CCircle> c1 = make_shared <CCircle>(new_border_color);
	c1->set(500, 500, 600, 600);
	shapes.push_back(c1);

	shared_ptr<CCircle> c2 = make_shared <CCircle>(new_border_color, new_fill_color);
	c2->set(500, 500, 550, 500);
	shapes.push_back(c2);
}

void TESTELIPSES() {
	shared_ptr<CElipse> e1 = make_shared <CElipse>(new_border_color);
	e1->set(600, 600, 100, 50);
	shapes.push_back(e1);

	shared_ptr<CElipse> e2 = make_shared <CElipse>(new_border_color, new_fill_color);
	e2->set(550, 550, 25, 50);
	shapes.push_back(e2);
}

void TESTBEZIERS() {
	shared_ptr<CBezier> b1 = make_shared <CBezier>(new_border_color, new_fill_color);
	b1->addVertex(300, 100);
	b1->addVertex(300, 200);
	shapes.push_back(b1);

	shared_ptr<CBezier> b4b = make_shared <CBezier>(new_border_color, new_fill_color);
	b4b->addVertex(300, 400);
	b4b->addVertex(350, 555);
	b4b->addVertex(555, 500);
	b4b->addVertex(575, 415);
	b4b->addVertex(550, 395);
	shapes.push_back(b4b);

	shared_ptr<CBezier> b2 = make_shared <CBezier>(new_border_color, new_fill_color);
	b2->addVertex(400, 100);
	b2->addVertex(400, 200);
	b2->addVertex(425, 215);
	shapes.push_back(b2);

	shared_ptr<CBezier> b3 = make_shared <CBezier>(new_border_color, new_fill_color);
	b3->addVertex(515, 100);
	b3->addVertex(520, 200);
	b3->addVertex(525, 215);
	b3->addVertex(620, 415);
	shapes.push_back(b3);

	shared_ptr<CBezier> b4 = make_shared <CBezier>(new_border_color, new_fill_color);
	b4->addVertex(615, 100);
	b4->addVertex(620, 200);
	b4->addVertex(625, 215);
	b4->addVertex(720, 415);
	b4->addVertex(780, 515);
	shapes.push_back(b4);

	shared_ptr<CBezier> b5 = make_shared <CBezier>(new_border_color, new_fill_color);
	b5->addVertex(715, 100);
	b5->addVertex(720, 200);
	b5->addVertex(725, 215);
	b5->addVertex(820, 415);
	b5->addVertex(880, 515);
	b5->addVertex(900, 550);
	shapes.push_back(b5);

	shared_ptr<CBezier> b10 = make_shared <CBezier>(new_border_color, new_fill_color);
	b10->addVertex(1015, 115);
	b10->addVertex(1020, 225);
	b10->addVertex(1025, 230);
	b10->addVertex(1020, 305);
	b10->addVertex(915, 111);
	b10->addVertex(928, 210);
	b10->addVertex(960, 215);
	b10->addVertex(1002, 415);
	b10->addVertex(1030, 215);
	b10->addVertex(1035, 415);
	shapes.push_back(b10);
}

void TEST() {
	// Aplication Code
	TESTLINES();
	TESTTRIANGLES();
	TESTRECTANGLES();
	TESTCIRCLES();
	TESTELIPSES();
	TESTBEZIERS();
}

// Defines Mouse Style
void MouseStyle(bool state) {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDrawCursor = state;

	const char* mouse_cursors_names[] = { "Arrow", "TextInput", "ResizeAll", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand", "NotAllowed" };
	IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

	ImGuiMouseCursor current = ImGui::GetMouseCursor();
	ImGui::SetMouseCursor(0); //Arrow
}

int main(int argc, char** argv)
{
	// INIT STATUS
	INIT_STATUS();

	// init GLUT and create Window
	glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	//glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Better Paint - Proyecto 1 - Leonardo Mendoza");

	//cout << "Hi Hi :,D" << endl;
	cout << "Better Paint says Hi Hi :,D!" << endl;

	/*
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	*/

	// TEST FUNCTION
	// TEST(); // Call it with shortcut 't'.

	// register callbacks
	glutDisplayFunc(renderScene);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	// FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	//ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");
	//ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Here is our new entry in the main function
	glutMouseFunc(onClick);
	/*
		glutMotionFunc and glutPassiveMotionFunc set the motion and passive motion callback respectively 
		for the current window. The motion callback for a window is called when the mouse moves within the 
		window while one or more mouse buttons are pressed. The passive motion callback for a window is called
		when the mouse moves within the window while no mouse buttons are pressed.
	*/
	glutMotionFunc(onMotion);
	glutPassiveMotionFunc(onPassiveMotion); //IDK which event correspond

	/*
		glutKeyboardFunc sets the keyboard callback for the current window. When a user types into the window,
		each key press generating an ASCII character will generate a keyboard callback. The key callback 
		parameter is the generated ASCII character. The state of modifier keys such as Shift cannot be 
		determined directly; their only effect will be on the returned ASCII data. The x and y callback 
		parameters indicate the mouse location in window relative coordinates when the key was pressed. 
		When a new window is created, no keyboard callback is initially registered, and ASCII key strokes 
		in the window are ignored. Passing NULL to glutKeyboardFunc disables the generation of keyboard callbacks. 
	*/
	glutKeyboardFunc(onKeyboardEntry);
	glutReshapeFunc(changeSize);

	// define idle and init
	glutIdleFunc(idle);

	// Enable transparency (e.g. black semi-transparent cover over screen appears with dialogues)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MouseStyle(true);

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	cout << "Bye Bye..." << endl;
	return 0;
}