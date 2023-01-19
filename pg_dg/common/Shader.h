//
// Created by part4 on 28.01.2022.
//

#ifndef GRAFIKA_SHADER_H
#define GRAFIKA_SHADER_H

#include <sstream>
#include <string>

using namespace std;

class shader {
    public:
        // the program ID
        unsigned int id{};
        shader();
        // constructor reads and builds the shader
        shader(const char* vertex_path, const char* fragment_path);
        // activate/deactivate the shader
        void activate() const;
        void deactivate() const;
        // utility uniform functions
        void set_bool(const string &name, bool value) const;
        void set_int(const string &name, int value) const;
        void set_float(const string &name, float value) const;
};


#endif //GRAFIKA_SHADER_H
