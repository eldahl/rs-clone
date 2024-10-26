#include "map.h"
#include "shader.h"
#include "texture.h"

#include <fstream>
#include <iostream>

#include <glm/glm.hpp>

Map::Map(std::string filePath) {
    file_path = filePath;
    

    loadMapFromFile();
}

void Map::initializeMapTextureCatalog() {
    map_textures_catalog = new MapTextureIndex*[map_width];
    for(int x = 0; x < map_width; x++) {
        map_textures_catalog[x] = new MapTextureIndex[map_height];
        for(int y = 0; y < map_height; y++) {
            map_textures_catalog[x][y] = static_cast<MapTextureIndex>(-1);
        }
    }

}

// (Super unsafe!!!) TODO: handle errors
void Map::loadMapFromFile() {
    std::string mapContent;
    std::ifstream mapFile (file_path.c_str());
    mapFile >> mapContent;
    
    std::cout << file_path << "\n" << mapContent << std::endl;
    
    // Get width and height of map in grid cells
    int width, height;
    mapFile >> width;
    mapFile >> height;

    std::cout << "Size: " << width << "x" << height << std::endl;

    map_width = width;
    map_height = height;

    // Ready the map
    initializeMapTextureCatalog();

    // Read textures
    int numOfTextures;
    std::vector<std::string> texturePaths;
    mapFile >> numOfTextures;
    std::cout << "Number of textures: " << numOfTextures << std::endl;
    for(int i = 0; i < numOfTextures; i++) {
        std::string textureStr;
        mapFile >> textureStr;
        texturePaths.push_back(textureStr);
        std::cout << textureStr << std::endl;
    }

    // Load textures
    loadTexturesAndBind(texturePaths);

    // Read map data file 
    while(!mapFile.eof()) {
        int x, y, textureIndex;

        mapFile >> x;
        mapFile >> y;
        mapFile >> textureIndex;
        map_textures_catalog[y][x] = static_cast<MapTextureIndex>(textureIndex);
        std::cout << "Wrote tex index " << textureIndex << " to x: " << x << " y: " << y << std::endl; 
    }
}

void Map::loadTexturesAndBind(std::vector<std::string> paths) {
    for(int i = 0; i < paths.size(); i++) {
        std::cout << "Loading and binding: " << paths.at(i) << std::endl;
        Texture tex = Texture(paths.at(i).c_str(), false);
        textures.push_back(tex);

        // Texture units binding with offset of 50 plus amount of textures loaded
        glActiveTexture(GL_TEXTURE0 + 50 + textures.size());
        glBindTexture(GL_TEXTURE_2D, tex.ID);
    }
}

void Map::render(Shader *mapShader) {
    
    for(int x = 0; x < map_width; x++) {
        for(int y = 0; y < map_height; y++) {
            MapTextureIndex mti = map_textures_catalog[x][y];
            if (mti == 1) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)x + 0.5f, (float)y + 0.5f, 0.0f));
                mapShader->setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
    }
}
