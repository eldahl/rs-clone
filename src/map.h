#ifndef MAP_H
#define MAP_H

#include <string>

#include "texture.h"
#include "shader.h"

#include <vector>
enum MapTextureIndex {
    grass,
    gravel,
    sand,
    dirt,
    stone
};

class Map {
public:
    Map(std::string filePath, Shader *shader);
    void render();
private:
    // Map file file path
    std::string file_path;
    int map_width, map_height;
        
    Shader *mapShader = nullptr;

    // Location of textures on the map cells
    MapTextureIndex **map_textures_catalog;
    
    // Textures of the map
    std::vector<Texture> textures;

    void loadMapFromFile();
    void initializeMapTextureCatalog();
    void loadTexturesAndBind(std::vector<std::string> paths);
};

#endif
