#ifndef MAP_H
#define MAP_H

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
    Map(const char* filePath);

private:
    const char* file_path;
    std::vector<std::vector<MapTextureIndex>> map_textures_catalog;

    void loadMapFromFile();
};

#endif
