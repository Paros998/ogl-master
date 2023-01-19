//
// Created by part4 on 28.01.2022.
//

#ifndef GRAFIKA_TEXTURE_H
#define GRAFIKA_TEXTURE_H

#include <GL/glew.h>
#include "iostream"

using namespace std;

class texture {
private:
    unsigned int texture_{};

public:
    texture();
    explicit texture(const char* texture_source);
    [[nodiscard]] unsigned int get_texture() const;
};

#endif //GRAFIKA_TEXTURE_H
