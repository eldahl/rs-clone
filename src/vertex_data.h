#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

class VertexData {
public:
    unsigned int vaoID, vboID;
    VertexData();
    void setVertexAttrib(unsigned int index, int numBytes, int stride, int offset);
    void bind(float* vertices, unsigned long size);
    void unbind();
private:

};

#endif
