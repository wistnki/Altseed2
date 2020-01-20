#include "Material.h"

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

    auto piplineState = LLGI::CreateSharedPtr(g->CreatePiplineState());
    piplineState->SetShader(LLGI::ShaderStageType::Vertex, vertexShader_->Get());
    piplineState->SetShader(LLGI::ShaderStageType::Pixel, shader_->Get());
    piplineState->Culling = LLGI::CullingMode::DoubleSide;
    piplineState->SetRenderPassPipelineState(renderPassPipelineState.get());
    // TODO
    /*
    pip->VertexLayouts[0] = LLGI::VertexLayoutFormat::R32G32B32_FLOAT;
    pip->VertexLayouts[1] = LLGI::VertexLayoutFormat::R32G32_FLOAT;
    pip->VertexLayouts[2] = LLGI::VertexLayoutFormat::R8G8B8A8_UNORM;
    pip->VertexLayoutNames[0] = "POSITION";
    pip->VertexLayoutNames[1] = "UV";
    pip->VertexLayoutNames[2] = "COLOR";
    pip->VertexLayoutCount = 3;
    */

    piplineState->Compile();

    pipelineStates_[renderPassPipelineState] = piplineState;

    return piplineState;
}

}  // namespace altseed