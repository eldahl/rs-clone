#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    unsigned int ID;

    Texture(const char* filePath, bool withAlpha);
    unsigned int genAndBindAndLoad();
private:
    const char* filepath;
    bool loadWithAlpha;
    int width, height, nrChannels;
};

#endif
