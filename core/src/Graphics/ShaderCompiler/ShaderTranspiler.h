
#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>

namespace altseed {

enum class ShaderStageType {
    Vertex,
    Pixel,
};

class SPIRV {
private:
    std::vector<uint32_t> data_;
    std::string error_;
    ShaderStageType shaderStage_;

public:
    SPIRV(const std::vector<uint32_t>& data, ShaderStageType shaderStage);

    SPIRV(const std::string& error);

    ShaderStageType GetStage() const;

    const std::vector<uint32_t>& GetData() const;
};

class SPIRVTranspiler {
protected:
    std::string code_;
    std::string errorCode_;

public:
    SPIRVTranspiler() = default;
    virtual ~SPIRVTranspiler() = default;

    virtual bool Transpile(const std::shared_ptr<SPIRV>& spirv);
    std::string GetErrorCode() const;
    std::string GetCode() const;
};

/**
    @brief it doesn't work currently
*/

class SPIRVToHLSLTranspiler : public SPIRVTranspiler {
public:
    bool Transpile(const std::shared_ptr<SPIRV>& spirv) override;
};

class SPIRVToMSLTranspiler : public SPIRVTranspiler {
public:
    bool Transpile(const std::shared_ptr<SPIRV>& spirv) override;
};

class SPIRVToGLSLTranspiler : public SPIRVTranspiler {
private:
    bool isVulkanMode_ = false;

public:
    SPIRVToGLSLTranspiler(bool isVulkanMode) : isVulkanMode_(isVulkanMode) {}

    bool Transpile(const std::shared_ptr<SPIRV>& spirv) override;
};

struct SPIRVReflectionUniform {
    std::string Name;
    int32_t Offset = 0;
    int32_t Size = 0;
};

struct SPIRVReflectionTexture {
    std::string Name;
    int32_t Offset = 0;
};

class SPIRVReflection : public SPIRVTranspiler {
public:
    bool Transpile(const std::shared_ptr<SPIRV>& spirv) override;

    std::vector<SPIRVReflectionUniform> Uniforms;

    std::vector<SPIRVReflectionTexture> Textures;
};

class SPIRVGenerator {
private:
public:
    bool Initialize();

    void Terminate();

    std::shared_ptr<SPIRV> Generate(const char* code, ShaderStageType shaderStageType);
};

}  // namespace altseed
