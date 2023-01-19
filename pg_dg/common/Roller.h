#ifndef GRAFIKA_ROLLER_H
#define GRAFIKA_ROLLER_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include "vector"

using namespace glm;
using namespace std;

class Roller {
private:
    mat4 model;
    unsigned int modelUniformLoc;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    vec3 position;
    vec4 color;

    vector<float> Vertices;
    vector<int> Indices;
    unsigned int numberOfIndices;
    float height;
    int stackCount;
    int sectorsCount;
    float diameter;

    double sectorAngle;
    double stackAngle;
public:
    void calculateVerticesAndIndices( );
    Roller( ) = default;
    Roller(int stackCount, int sectorsCount, float diameter, float height, vec3 position, unsigned int modelUniformLoc, vec4 color);
    void render( );
};

#endif //GRAFIKA_ROLLER_H