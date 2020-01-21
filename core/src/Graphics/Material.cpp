#include "Material.h"
#include "BuildinShader.h"

#include <glslang/Public/ShaderLang.h>
#include <spirv_cross/spirv.hpp>

namespace altseed {

std::shared_ptr<LLGI::PipelineState> Material::GetPipelineState(LLGI::RenderPass* renderPass) {
    auto g = Graphics::GetInstance()->GetGraphicsLLGI();

    auto renderPassPipelineState = LLGI::CreateSharedPtr(g->CreateRenderPassPipelineState(renderPass));
    auto it = pipelineStates_.find(renderPassPipelineState);
    if (it != pipelineStates_.end()) {
        return it->second;
    }

	if (vertexShader_ == nullptr)
	{
        vertexShader_ = Graphics::GetInstance()->GetBuildinShader()->Create(BuildinShaderType::SpriteUnlitVS);
	}

    auto piplineState = LLGI::CreateSharedPtr(g->CreatePiplineState());
    piplineState->SetShader(LLGI::ShaderStageType::Vertex, vertexShader_->Get());
    piplineState->SetShader(LLGI::ShaderStageType::Pixel, shader_->Get());
    piplineState->Culling = LLGI::CullingMode::DoubleSide;
    piplineState->SetRenderPassPipelineState(renderPassPipelineState.get());

    piplineState->VertexLayouts[0] = LLGI::VertexLayoutFormat::R32G32B32_FLOAT;
    piplineState->VertexLayouts[1] = LLGI::VertexLayoutFormat::R8G8B8A8_UNORM;
    piplineState->VertexLayouts[2] = LLGI::VertexLayoutFormat::R32G32_FLOAT;
    piplineState->VertexLayouts[3] = LLGI::VertexLayoutFormat::R32G32_FLOAT;
    piplineState->VertexLayoutNames[0] = "POSITION";
    piplineState->VertexLayoutNames[1] = "COLOR";
    piplineState->VertexLayoutNames[2] = "UV";
    piplineState->VertexLayoutNames[3] = "UV";
    piplineState->VertexLayoutSemantics[0] = 0;
    piplineState->VertexLayoutSemantics[1] = 0;
    piplineState->VertexLayoutSemantics[2] = 0;
    piplineState->VertexLayoutSemantics[3] = 1;
    piplineState->VertexLayoutCount = 4;

    piplineState->Compile();

    pipelineStates_[renderPassPipelineState] = piplineState;

    return piplineState;
}

}  // namespace altseed