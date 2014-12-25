#pragma once

#ifndef GLC_TEXTURE_H
#define GLC_TEXTURE_H

#include <GL/glew.h>
#include <FreeImagePlus.h>
#include <string>

namespace glc {
    class TexLoader {
        public:
            TexLoader();
            GLuint load(std::string path);
        private:
            fipImage m_container;
    };
}

#endif