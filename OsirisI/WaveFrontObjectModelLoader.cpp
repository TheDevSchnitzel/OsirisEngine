#include "stdafx.h"

using namespace OsirisI;
using namespace OsirisI::Graphics;
using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Graphics::Shader;
using namespace OsirisI::Utilities;
using namespace OsirisI::Manager;

namespace OsirisI {
	namespace Resources {
		namespace Loader {
			WaveFrontObjectModelLoader::WaveFrontObjectModelLoader() {
			
			}

			WaveFrontObjectModelLoader::~WaveFrontObjectModelLoader() {
			}

            OsirisI::Resources::Resource* WaveFrontObjectModelLoader::Load(char* data, long size, OsirisI::Utilities::HashedString resourceName) {
                return nullptr;
            }
			/*Resource* WaveFrontObjectModelLoader::Load(std::string modelName, char* rawData, unsigned int dataSize) {
				ResourcePtr<char>* resource = (ResourcePtr<char>*)ResourceManager::LoadWithoutCache<char>(rawData, dataSize);
				
				std::vector<std::string> objFile;
				std::vector<std::string> mtlFile;
				if(resource == NULL) return NULL;

				objFile = HelperFunctions::split(resource->GetValue().get(), '\n');
				resource->Release();
				return Load(modelName, objFile, mtlFile);
			}*/
			Resource* WaveFrontObjectModelLoader::Load(HashedString path, HashedString resourceName) {
				std::vector<std::string> objFile;
				std::vector<std::string> mtlFile;
				
				//if(fromFileSystem) { 
				///*	std::string* objFileStrArray = FileIOHelper::GetTextLinesFromFile(filePath);
				//	for(unsigned int i = 0; i < sizeof(objFileStrArray)/sizeof(std::string); i++) objFile[i] = objFileStrArray[i];*/
				//	
				//	std::fstream fin;
				//	fin.open(filePath, std::ios::in);
				//	if(fin.fail() || fin.bad()) return nullptr;
				//	std::string text = "";		
				//	while(std::getline(fin, text)) objFile.push_back(text);
				//	fin.close();


				//	HelperFunctions::replace(modelName, ".obj", ".mtl");
				//	/*std::string* mtlFileStrArray = FileIOHelper::GetTextLinesFromFile(filePath);
				//	for(unsigned int i = 0; i < sizeof(mtlFileStrArray)/sizeof(std::string); i++) mtlFile[i] = mtlFileStrArray[i];*/
				//	fin.open(filePath, std::ios::in);
				//	if(fin.fail() || fin.bad()) return nullptr;
				//	while(std::getline(fin, text)) mtlFile.push_back(text);
				//	fin.close();
				//}
				//else {				
                ResourcePtr<char*>* resourceObj = (ResourcePtr<char*>*)ResourceManager::GetResourceDirectly(path.GetString());

				std::string tmpPath = path.GetString();
				HelperFunctions::replace(tmpPath, ".obj", ".mtl");
				ResourcePtr<char*>* resourceMtl = (ResourcePtr<char*>*)ResourceManager::GetResourceDirectly(tmpPath);
				if(resourceObj == NULL) return NULL;

				objFile = HelperFunctions::split(resourceObj->GetValue(), '\n');
				mtlFile = HelperFunctions::split(resourceMtl->GetValue(), '\n');
				resourceObj->Release();
				resourceMtl->Release();
				//}
				return Load(path, resourceName, objFile, mtlFile);
			}

			Resource* WaveFrontObjectModelLoader::Load(HashedString path, HashedString resourceName, std::vector<std::string> objFile, std::vector<std::string> mtlFile) {
				#pragma region .obj Parser
				std::string mtlLibName = "";
				bool hasTextureCoordinates = false, hasNormal = false;
				unsigned int OVertexCounter = 0, textureCounter = 0, normalCounter = 0;
				std::vector<OFace> tmpFaces;
				std::vector<OVector3> tmpOVertexPositions;
				std::vector<OVector2> tmpTexturePositions;
                std::vector<unsigned long> tmpIndicesVector;
                std::vector<OVertex> tmpVerticesVector;
                std::vector<OVector3> tmpVertexNormalsVector;
				HashedString meshName;

				for(unsigned int i = 0; i < objFile.size(); i++) {
					std::vector<std::string> splitted = HelperFunctions::split(objFile[i], ' ');

					if(splitted.size() > 0){
						if(splitted[0] == "#") { //Comments
						}
						else if(splitted[0] == "v") { //OVertex Positions
							tmpOVertexPositions.push_back(OVector3(Converter::StringToFloat(splitted[1]), Converter::StringToFloat(splitted[2]), Converter::StringToFloat(splitted[3])));
						}
						else if(splitted[0] == "vt") {	//Texture Coordinates
							tmpTexturePositions.push_back(OVector2(Converter::StringToFloat(splitted[1]), Converter::StringToFloat(splitted[2])));
							hasTextureCoordinates = true;
						}
						else if(splitted[0] == "vn") {	//Normals
                            tmpVertexNormalsVector.push_back(OVector3(Converter::StringToFloat(splitted[1]), Converter::StringToFloat(splitted[2]), Converter::StringToFloat(splitted[3])));
							normalCounter++;
							hasNormal= true;
						}
						else if(splitted[0] == "o") { //Object Name
							meshName = HashedString(splitted[1]);
						}
						else if(splitted[0] == "g") { //Groups	(New group (Subset))
						}
						else if(splitted[0] == "f") { //Faces
							OFace face = OFace();
							for (unsigned int i = 1; i < splitted.size(); i++) {
								unsigned int vertPosIndexTemp = 0, vertNormIndexTemp = 0, vertTCIndexTemp = 0;
								std::vector<std::string> faceSplitted = HelperFunctions::split(splitted[i], '/');
								if(faceSplitted.size() == 2) faceSplitted.push_back(""); //Falls vPos/tPos/NULL -> fügt NULL ein

								if(i < 4) { //Die ersten drei Vertices
									if(faceSplitted.size() == 1) { //OVertex Position
										vertPosIndexTemp = Converter::StringToInt(faceSplitted[0]) - 1; //Subtract one since c++ arrays start with 0, and obj start with 1
										vertNormIndexTemp = 0;
										vertTCIndexTemp = 0;
									}
									else if(faceSplitted.size() > 1) {
										if(faceSplitted[0] != "") { //OVertex Position
											vertPosIndexTemp = Converter::StringToInt(faceSplitted[0]) - 1;
										}
										if(faceSplitted[1] != "") { //Texture Position
											vertTCIndexTemp = Converter::StringToInt(faceSplitted[1]) - 1;
										}
										else vertTCIndexTemp = 0;
										if(faceSplitted[2] != "") { //Normal
											vertNormIndexTemp = Converter::StringToInt(faceSplitted[2]) - 1;
										}
										else vertNormIndexTemp = 0;
									}
									if(i == 1) face.SetData1(OVector3(static_cast<float>(vertPosIndexTemp), static_cast<float>(vertTCIndexTemp), static_cast<float>(vertNormIndexTemp)));
									else if(i == 2) face.SetData2(OVector3(static_cast<float>(vertPosIndexTemp), static_cast<float>(vertTCIndexTemp), static_cast<float>(vertNormIndexTemp)));
									else if(i == 3) {
										face.SetData3(OVector3(static_cast<float>(vertPosIndexTemp), static_cast<float>(vertTCIndexTemp), static_cast<float>(vertNormIndexTemp)));
										tmpFaces.push_back(face);
									}
								}
								else {						
									if(faceSplitted.size() == 1) { //OVertex Position
										vertPosIndexTemp = Converter::StringToInt(faceSplitted[0]) - 1; //Subtract one since c++ arrays start with 0, and obj start with 1
										vertNormIndexTemp = 0;
										vertTCIndexTemp = 0;
									}
									else if(faceSplitted.size() > 1) {
										if(faceSplitted[0] != "") { //OVertex Position
											vertPosIndexTemp = Converter::StringToInt(faceSplitted[0]) - 1;
										}
										if(faceSplitted[1] != "") { //Texture Position
											vertTCIndexTemp = Converter::StringToInt(faceSplitted[1]) - 1;
										}
										else vertTCIndexTemp = 0;
										if(faceSplitted[2] != "") { //Normal
											vertNormIndexTemp = Converter::StringToInt(faceSplitted[2]) - 1;
										}
										else vertNormIndexTemp = 0;
									}
									tmpFaces.push_back(OFace(OVector3(face.GetData1()), OVector3(face.GetData3()), OVector3(static_cast<float>(vertPosIndexTemp), static_cast<float>(vertTCIndexTemp), static_cast<float>(vertNormIndexTemp))));
									face = tmpFaces[tmpFaces.size()-1];
								}
							}
						}
						else if(splitted[0] == "mtllib") { //Material Lib
							if(splitted.size() > 1) mtlLibName = splitted[1];
						}
						else if(splitted[0] == "s") { // Smoothing Groups
						}
						else if(splitted[0] == "usemtl") { //Group Materials
						}
					}
				}
                DX11Mesh* iMesh = (new DX11Mesh(meshName));/*, [](IMesh* mesh) {
					ResourceManager::GetCache<IMesh>()->ReleaseResource(mesh->GetName()); 
				});
*/
				//BUILD VERTICES FROM FACES
				unsigned int verticesCount = 0, maxVerticesCount = tmpFaces.size()*3;
				for(unsigned int i = 0; i < tmpFaces.size(); i++) {
					if(tmpTexturePositions.size() > 0) {
                        tmpVerticesVector.push_back(OVertex(tmpOVertexPositions[static_cast<int>(tmpFaces[i].GetData1().x)], tmpTexturePositions[static_cast<int>(tmpFaces[i].GetData1().y)], ((hasNormal) ? tmpVertexNormalsVector[static_cast<int>(tmpFaces[i].GetData1().z)] : OVector3(0.f, 0.f, -1.f))));
						tmpIndicesVector.push_back(verticesCount);
						verticesCount++;
                        tmpVerticesVector.push_back(OVertex(tmpOVertexPositions[static_cast<int>(tmpFaces[i].GetData2().x)], tmpTexturePositions[static_cast<int>(tmpFaces[i].GetData2().y)], ((hasNormal) ? tmpVertexNormalsVector[static_cast<int>(tmpFaces[i].GetData1().z)] : OVector3(0.f, 0.f, -1.f))));
						tmpIndicesVector.push_back(verticesCount);
						verticesCount++;
                        tmpVerticesVector.push_back(OVertex(tmpOVertexPositions[static_cast<int>(tmpFaces[i].GetData3().x)], tmpTexturePositions[static_cast<int>(tmpFaces[i].GetData3().y)], ((hasNormal) ? tmpVertexNormalsVector[static_cast<int>(tmpFaces[i].GetData1().z)] : OVector3(0.f, 0.f, -1.f))));
						tmpIndicesVector.push_back(verticesCount);
						verticesCount++;
					}
					else {
                        tmpVerticesVector.push_back(OVertex(tmpOVertexPositions[static_cast<int>(tmpFaces[i].GetData1().x)], OVector2(0.f,0.f), ((hasNormal) ? tmpVertexNormalsVector[static_cast<int>(tmpFaces[i].GetData1().z)] : OVector3(0.f, 0.f, -1.f))));
						tmpIndicesVector.push_back(verticesCount);
						verticesCount++;
                        tmpVerticesVector.push_back(OVertex(tmpOVertexPositions[static_cast<int>(tmpFaces[i].GetData2().x)], OVector2(1.f,0.f), ((hasNormal) ? tmpVertexNormalsVector[static_cast<int>(tmpFaces[i].GetData2().z)] : OVector3(0.f, 0.f, -1.f))));
						tmpIndicesVector.push_back(verticesCount);
						verticesCount++;
                        tmpVerticesVector.push_back(OVertex(tmpOVertexPositions[static_cast<int>(tmpFaces[i].GetData3().x)], OVector2(0.f,1.f), ((hasNormal) ? tmpVertexNormalsVector[static_cast<int>(tmpFaces[i].GetData3().z)] : OVector3(0.f, 0.f, -1.f))));
						tmpIndicesVector.push_back(verticesCount);
						verticesCount++;
					}
				}
				tmpFaces.clear();
				tmpOVertexPositions.clear();
				tmpTexturePositions.clear();
				#pragma endregion

                ResourcePtr<ITexture*>* res = nullptr;
				#pragma region .mtl Parser
				if(mtlFile.size() > 0) {
					for(unsigned int i = 0; i < mtlFile.size(); i++) {
						std::vector<std::string> splitted = HelperFunctions::split(mtlFile[i], ' ');

						if(splitted.size() > 0){
							if(splitted[0] == "#") { //Comments
							}
							else if(splitted[0] == "newmtl") {
							}
							else if(splitted[0] == "Ns") {
							}
							else if(splitted[0] == "Ka") {
							}
							else if(splitted[0] == "Kd") {
							}
							else if(splitted[0] == "Ks") {
							}
							else if(splitted[0] == "Ni") {
							}
							else if(splitted[0] == "d") {
							}
							else if(splitted[0] == "illum") {
							}
							else if(splitted[0] == "map_Kd") {                                								
                                res = ((ResourcePtr<ITexture*>*)ResourceManager::GetResource<ITexture>(splitted[1]));
                                if(res == nullptr) {
                                    res = ((ResourcePtr<ITexture*>*)ResourceManager::LoadAndCache<ITexture>(HashedString(splitted[1]), HashedString(splitted[1])));
                                }
                                if(res == nullptr) {
                                    LOG_WARNING("[" << OS_OBJECT_NOT_FOUND << "] The resource(ITexture) " << splitted[1] << "could not be found...");
                                    return nullptr;
                                }
							}
						}
					}
				}
				#pragma endregion
                
                if (res == nullptr) {
                    LOG_WARNING("WaveFrontObjectModelLoader - " << resourceName << " no texture found!");
                    return nullptr;
                }

                /*for (unsigned int i = 0; i < tmpVerticesVector.size(); i++) {
                    OVector3 a = tmpVerticesVector[i].GetPosition();
                    OVector3 b = tmpVerticesVector[i].GetNormal();
                    OVector2 c = tmpVerticesVector[i].GetTexture();
                    std::cout << "{" << a.x << ", " << a.y << ", " << a.z << "}, {" << c.x << ", " << c.y << "} , { " << b.x << ", " << b.y << ", " << b.z << " }" << std::endl;
                }*/

                //for (unsigned int i = 0; i < tmpVerticesVector.size(); i++) {
                //    std::cout << "vertices[" << i << "].SetPosition(OVector3(" << tmpVerticesVector[i].GetPosition().x << "f, " << tmpVerticesVector[i].GetPosition().y << "f, " << tmpVerticesVector[i].GetPosition().z << "f));" << std::endl;
                //    std::cout << "vertices[" << i << "].SetTexture(OVector3(" << tmpVerticesVector[i].GetTexture().x << "f, " << tmpVerticesVector[i].GetTexture().y << "f));" << std::endl;
                //    std::cout << "vertices[" << i << "].SetNormal(OVector3(" << tmpVerticesVector[i].GetNormal().x << "f, " << tmpVerticesVector[i].GetNormal().y << "f, " << tmpVerticesVector[i].GetNormal().z << "f));" << std::endl;
                //}

                iMesh->CreateVertexAndIndexBuffer(tmpVerticesVector.data(), tmpIndicesVector.data(), tmpVerticesVector.size(), tmpIndicesVector.size());
				unsigned int size = (tmpVerticesVector.size() * sizeof(OVertex)) + (tmpIndicesVector.size() * sizeof(unsigned long));
                DX11GameModel* model = new DX11GameModel(OVector3(0.f, 0.f, 0.f), HashedString(resourceName));
                model->SetTexture(res->GetValue());
				model->AddMesh(iMesh);

				return new ResourcePtr<IGameModel*>(size, model, path);
			}

			ReturnState WaveFrontObjectModelLoader::Release() {
				return OS_OK;
			}
		}
	}
}
