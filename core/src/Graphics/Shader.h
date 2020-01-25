﻿#pragma once

#include <memory>

#include "../BaseObject.h"
#include "Graphics.h"

namespace altseed {

struct ShaderReflectionUniform {
    std::string Name;
    int32_t Offset = 0;
    int32_t Size = 0;
};

struct ShaderReflectionTexture {
    std::string Name;
    int32_t Offset = 0;
};

class Shader : public BaseObject {
private:
    bool isValid_ = false;
    std::string code_;
    std::string errorMessage_;

    std::vector<ShaderReflectionTexture> textures_;
    std::vector<ShaderReflectionUniform> uniforms_;

    LLGI::Shader* shader_ = nullptr;

public:
    ~Shader() { LLGI::SafeRelease(shader_); }

    Shader(std::string code, std::string errorMessage) : code_(code), errorMessage_(errorMessage), isValid_(false) {}

    Shader(std::string code,
           const std::vector<ShaderReflectionTexture>& textures,
           const std::vector<ShaderReflectionUniform>& uniforms,
           LLGI::Shader* shader)
        : isValid_(true), textures_(textures), uniforms_(uniforms) {
        LLGI::SafeAssign(shader_, shader);
    }

    const std::vector<ShaderReflectionTexture>& GetReflectionTextures() const { return textures_; }
    const std::vector<ShaderReflectionUniform>& GetReflectionUniforms() const { return uniforms_; }

    bool GetIsValid() const { return isValid_; }

    LLGI::Shader* Get() const { return shader_; }
};

}  // namespace altseed
