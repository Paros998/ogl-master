#include "Roller.h"

#define M_PI 3.141592

void Roller::calculateVerticesAndIndices( ) {
    float sectorStep = 2 * M_PI / sectorsCount;
    float stackStep = M_PI / stackCount;
    float alpha = 2 * M_PI * (sectorStep / sectorsCount);

    float x, y, z;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;
       
        z = height /2;                                 

        for (int j = 0; j <= sectorsCount; ++j)
        {
            sectorAngle = j * sectorStep;              // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = diameter / 2 * cosf(sectorAngle);                // r * cos(u) * cos(v)
            y = diameter / 2 * sinf(sectorAngle);                // r * cos(u) * sin(v)
            Vertices.push_back(x + position.x);
            Vertices.push_back(y + position.y);
            Vertices.push_back(z + position.z);

            Vertices.push_back(color.r);
            Vertices.push_back(color.g);
            Vertices.push_back(color.b);
            Vertices.push_back(color.a);

        }

        for (int j = 0; j <= sectorsCount; ++j)
        {
            sectorAngle = j * sectorStep;              // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = diameter / 2 * cosf(sectorAngle);                // r * cos(u) * cos(v)
            y = diameter / 2 * sinf(sectorAngle);                // r * cos(u) * sin(v)

            Vertices.push_back(x + position.x);
            Vertices.push_back(y + position.y);
            Vertices.push_back(z - position.z);

            Vertices.push_back(color.r);
            Vertices.push_back(color.g);
            Vertices.push_back(color.b);
            Vertices.push_back(color.a);
        }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorsCount + 1);     // beginning of current stack
        k2 = k1 + sectorsCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorsCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                Indices.push_back(k1);
                Indices.push_back(k2);
                Indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                Indices.push_back(k1 + 1);
                Indices.push_back(k2);
                Indices.push_back(k2 + 1);
            }

        }
    }


    for (int i = 0, k = 0 + 1; i < sectorsCount; ++i, ++k)
    {
        if (i < sectorsCount - 1)
        {
            Indices.push_back(0);
            Indices.push_back(k + 1);
            Indices.push_back(k);
        }
        else // last triangle
        {
            Indices.push_back(0);
            Indices.push_back(0 + 1);
            Indices.push_back(k);
        }
    }

    // indices for the top surface
    for (int i = 0, k = Indices.size() / 2 + 1; i < sectorsCount; ++i, ++k)
    {
        if (i < sectorsCount - 1)
        {
            Indices.push_back(Indices.size( ) / 2);
            Indices.push_back(k);
            Indices.push_back(k + 1);
        }
        else // last triangle
        {
            Indices.push_back(Indices.size( ) / 2);
            Indices.push_back(k);
            Indices.push_back(Indices.size( ) / 2 + 1);
        }
    }

};

Roller::Roller(int stackCount, int sectorsCount, float diameter, float height, vec3 position, unsigned int modelUniformLoc, vec4 color ) {
	this->modelUniformLoc = modelUniformLoc;
    this->height = height;
    this->stackCount = stackCount;
    this->sectorsCount = sectorsCount;
    this->diameter = diameter;
    this->position = position;
    this->color = color;

    calculateVerticesAndIndices( );

    numberOfIndices = Indices.size( );

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Vertices.size( ), Vertices.data( ), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * Indices.size( ), Indices.data( ), GL_STATIC_DRAW);


    ///2-Setting vertex linking for openGL (call once in prepare)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    /// color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
};

void Roller::render( ) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, nullptr);
}