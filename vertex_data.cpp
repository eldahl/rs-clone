#include "vertex_data.h"
#include "shader.h"

VertexData::VertexData() {
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
}

void VertexData::bind(float* vertices, unsigned long size) {
    //Bind Vertex Array Object first, then VBOs
    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexData::setVertexAttrib(unsigned int index, int numBytes, int stride, int offset) {
    glVertexAttribPointer(index, numBytes, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(index);
}

void VertexData::unbind() {
    glBindVertexArray(0);
}
