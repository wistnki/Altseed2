#include "ShaderCompiler.h"
#include <glslang/Include/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include "ResourceLimits.h"

#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_hlsl.hpp>
#include <spirv_cross/spirv_msl.hpp>
#include <spirv_cross/spirv_reflect.hpp>

#include <iostream>

const char* simpleVS = R"(

struct VS_INPUT{
    float3 Position : POSITION0;
	float2 UV : UV0;
    float4 Color : COLOR0;
};
struct VS_OUTPUT{
    float4 Position : SV_POSITION;
	float2 UV : UV0;
    float4 Color : COLOR0;
};
   
cbuffer CB : register(b0)
{
  float4 offset;
};
VS_OUTPUT main(VS_INPUT input){
    VS_OUTPUT output;
        
    output.Position = float4(input.Position, 1.0f) + offset;
	output.UV = input.UV;
    output.Color = input.Color;
        
    return output;
}

)";

const char* simplePS = R"(

cbuffer CB : register(b1)
{
  float4 offset;
};
struct PS_INPUT
{
    float4  Position : SV_POSITION;
	float2  UV : UV0;
    float4  Color    : COLOR0;
};
float4 main(PS_INPUT input) : SV_TARGET 
{ 
	float4 c;
	c = input.Color + offset;
	c.a = 1.0f;
	return c;
}

)";

const char* textureVS = R"(

)";

const char* texturePS = R"(

Texture2D txt : register(t8);
SamplerState smp : register(s8);
struct PS_INPUT
{
    float4  Position : SV_POSITION;
	float2  UV : UV0;
    float4  Color    : COLOR0;
};
float4 main(PS_INPUT input) : SV_TARGET 
{ 
	float4 c;
	c = input.Color * txt.Sample(smp, input.UV);
	return c;
}

)";

namespace altseed {

enum class ShaderStageType {
    Vertex,
    Pixel,
};

EShLanguage GetGlslangShaderStage(ShaderStageType type) {
    if (type == ShaderStageType::Vertex) return EShLanguage::EShLangVertex;
    if (type == ShaderStageType::Pixel) return EShLanguage::EShLangFragment;
    throw std::string("Unimplemented ShaderStage");
}

class SPIRV {
private:
    std::vector<uint32_t> data_;
    std::string error_;

public:
    SPIRV(const std::vector<uint32_t>& data) : data_(data) {}

    SPIRV(const std::string& error) : error_(error) {}

    std::vector<uint32_t> GetData() const { return data_; }
};

class SPIRVTranspiler {
protected:
    std::string code_;
    std::string errorCode_;

public:
    virtual bool Transpile(const std::shared_ptr<SPIRV>& spirv) { return false; }
    std::string GetErrorCode() const { return errorCode_; }
    std::string GetCode() const { return code_; }
};

class SPIRVToHLSLTranspiler : public SPIRVTranspiler {
public:
    bool Transpile(const std::shared_ptr<SPIRV>& spirv) override {
        spirv_cross::CompilerHLSL compiler(spirv->GetData());

        // compiler.build_combined_image_samplers();

        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        // Get all sampled images in the shader.
        for (auto& resource : resources.separate_images) {
            // compiler.unset_decoration(resource.id, spv::decorationseparate);
        }

        for (auto& resource : resources.sampled_images) {
            unsigned set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            unsigned binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

            // Modify the decoration to prepare it for GLSL.
            compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);

            // Some arbitrary remapping if we want.
            compiler.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
        }

        spirv_cross::CompilerGLSL::Options options;
        options.separate_shader_objects = true;
        compiler.set_common_options(options);

        spirv_cross::CompilerHLSL::Options targetOptions;
        compiler.set_hlsl_options(targetOptions);

        code_ = compiler.compile();

        return true;
    }
};

class SPIRVToMSLTranspiler : public SPIRVTranspiler {
public:
    bool Transpile(const std::shared_ptr<SPIRV>& spirv) override {
        spirv_cross::CompilerMSL compiler(spirv->GetData());

        // compiler.build_combined_image_samplers();

        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        spirv_cross::CompilerGLSL::Options options;
        compiler.set_common_options(options);

        spirv_cross::CompilerMSL::Options targetOptions;
        compiler.set_msl_options(targetOptions);

        code_ = compiler.compile();

        return true;
    }
};

class SPIRVToGLSLTranspiler : public SPIRVTranspiler {
public:
    bool Transpile(const std::shared_ptr<SPIRV>& spirv) override {
        spirv_cross::CompilerGLSL compiler(spirv->GetData());

        // to combine a sampler and a texture
        compiler.build_combined_image_samplers();

        spirv_cross::ShaderResources resources = compiler.get_shader_resources();


		int32_t binding = 0;
        for (auto& resource : resources.sampled_images) 
		{
            auto i = compiler.get_decoration(resource.id, spv::DecorationLocation);
            compiler.set_decoration(resource.id, spv::DecorationBinding, binding);
            compiler.set_decoration(resource.id, spv::DecorationDescriptorSet, binding);
            binding += 1;
        }

        spirv_cross::CompilerGLSL::Options options;
        options.version = 420;
        options.enable_420pack_extension = true;
        compiler.set_common_options(options);

        code_ = compiler.compile();

        return true;
    }
};

class SPIRVGenerator {
private:
public:
    bool Initialize() {
        glslang::InitializeProcess();

        return true;
    }

    void Terminate() { glslang::FinalizeProcess(); }

    std::shared_ptr<SPIRV> Generate(const char* code, ShaderStageType shaderStageType) {
        std::string codeStr(code);
        std::shared_ptr<glslang::TProgram> program = std::make_shared<glslang::TProgram>();
        TBuiltInResource resources = glslang::DefaultTBuiltInResource;
        auto shaderStage = GetGlslangShaderStage(shaderStageType);

        std::shared_ptr<glslang::TShader> shader = std::make_shared<glslang::TShader>(shaderStage);
        shader->setEnvInput(glslang::EShSourceHlsl, shaderStage, glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
        shader->setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
        shader->setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

        const char* shaderStrings[1];
        shaderStrings[0] = codeStr.c_str();
        shader->setEntryPoint("main");
        // shader->setAutoMapBindings(true);
        // shader->setAutoMapLocations(true);

        shader->setStrings(shaderStrings, 1);
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
        messages = (EShMessages)(messages | EShMsgReadHlsl);
        messages = (EShMessages)(messages | EShOptFull);

        int defaultVersion = 110;
        if (!shader->parse(&resources, defaultVersion, false, messages)) {
            return std::make_shared<SPIRV>(program->getInfoLog());
        }

        program->addShader(shader.get());

        if (!program->link(messages)) {
            return std::make_shared<SPIRV>(program->getInfoLog());
        }

        std::vector<unsigned int> spirv;
        glslang::GlslangToSpv(*program->getIntermediate(shaderStage), spirv);

        return std::make_shared<SPIRV>(spirv);
    }
};

}  // namespace altseed

bool compilerTest() {
    altseed::SPIRVGenerator generator;
    generator.Initialize();
    /*
    {
        auto spirv = generator.Generate(simpleVS, altseed::ShaderStageType::Vertex);

        {
            auto transpiler = altseed::SPIRVToGLSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== GLSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }

        {
            auto transpiler = altseed::SPIRVToHLSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== HLSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }

        {
            auto transpiler = altseed::SPIRVToMSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== MSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }
    }

    {
        auto spirv = generator.Generate(simplePS, altseed::ShaderStageType::Pixel);

        {
            auto transpiler = altseed::SPIRVToGLSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== GLSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }

        {
            auto transpiler = altseed::SPIRVToHLSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== HLSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }

        {
            auto transpiler = altseed::SPIRVToMSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== MSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }
    }
        */
    {
        auto spirv = generator.Generate(texturePS, altseed::ShaderStageType::Pixel);

        {
            auto transpiler = altseed::SPIRVToGLSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== GLSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }

        {
            auto transpiler = altseed::SPIRVToMSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== MSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }

        {
            auto transpiler = altseed::SPIRVToHLSLTranspiler();
            transpiler.Transpile(spirv);
            std::cout << "== HLSL ==" << std::endl;
            std::cout << transpiler.GetCode() << std::endl;
        }
    }

    generator.Terminate();

    return true;
}
