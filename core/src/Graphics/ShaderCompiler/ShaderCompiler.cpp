#include "ShaderCompiler.h"
#include <glslang/Include/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include "ResourceLimits.h"

#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

const char* shader_ = R"(

#version 440 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;
    
out gl_PerVertex
{
    vec4 gl_Position;
};
    
layout(location = 0) out vec2 v_uv;
layout(location = 1) out vec4 v_color;

layout(binding = 0, set=0) uniform Block 
{
	vec4 u_offset;
};

void main()
{
	gl_Position.x  = a_position.x;
	gl_Position.y  = a_position.y;
	gl_Position.z  = a_position.z;
	gl_Position.w  = 1.0f;
	gl_Position = gl_Position + u_offset;
	v_uv = a_uv;
	v_color = a_color;
}

)";

const char* hlslShader_ = R"(
struct VS_INPUT{
    float3 g_position : POSITION0;
	float2 g_uv : UV0;
    float4 g_color : COLOR0;
};
struct VS_OUTPUT{
    float4 g_position : SV_POSITION;
    float4 g_color : COLOR0;
};

VS_OUTPUT main(VS_INPUT input){
    VS_OUTPUT output;
    
    output.g_position = float4(input.g_position, 1.0f);
    output.g_color = input.g_color;
    
    return output;
}
)";

bool compilerTest() {
    glslang::InitializeProcess();
    glslang::TProgram& program = *new glslang::TProgram;
    const char* shaderStrings[1];
    TBuiltInResource Resources = glslang::DefaultTBuiltInResource;

    EShLanguage stage = EShLangVertex;
    glslang::TShader* shader = new glslang::TShader(stage);
    shader->setEnvInput(glslang::EShSourceHlsl, stage, glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
    shader->setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
    shader->setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
    shaderStrings[0] = hlslShader_;
    shader->setEntryPoint("main");
	shader->setAutoMapBindings(true);
    shader->setAutoMapLocations(true);

	shader->setStrings(shaderStrings, 1);
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
        messages = (EShMessages)(messages | EShMsgReadHlsl | EShMsgHlslDX9Compatible);

    int defaultVersion = 110;
    if (!shader->parse(&Resources, defaultVersion, false, messages)) {
        printf("%s\n", shader->getInfoLog());
        printf("%s\n", shader->getInfoDebugLog());

        delete &program;
        delete shader;
        return false;
    }

    program.addShader(shader);
    
    if (!program.link(messages)) {
        delete &program;
        delete shader;
        return false;
    }

    std::vector<unsigned int> spirv;
    glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
    glslang::FinalizeProcess();

    delete &program;
    delete shader;

	{
        spirv_cross::CompilerGLSL glsl(std::move(spirv));

        // The SPIR-V is now parsed, and we can perform reflection on it.
        spirv_cross::ShaderResources resources = glsl.get_shader_resources();

        // Get all sampled images in the shader.
        for (auto& resource : resources.sampled_images) {
            unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
            printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

            // Modify the decoration to prepare it for GLSL.
            glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

            // Some arbitrary remapping if we want.
            glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
        }

        // Set some options.
        spirv_cross::CompilerGLSL::Options options;
        options.version = 310;
        //options.es = true;
        glsl.set_common_options(options);

        // Compile to GLSL, ready to give to GL driver.
        std::string source = glsl.compile();
        std::printf("Shader %s\n", source.c_str());
	}
    return true;
}
