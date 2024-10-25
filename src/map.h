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
    Map(std::string filePath);
    void render(Shader *mapShader);
private:
    // Map file file path
    std::string file_path;

    // Location of textures on the map cells
    MapTextureIndex map_textures_catalog[16][16];
    
    // Textures of the map
    std::vector<Texture> textures;

    void loadMapFromFile();
};

#endif
