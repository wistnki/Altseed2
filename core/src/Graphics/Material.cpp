#include "Material.h"

// hogehoge
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

	piplineState->SetShader(LLGI::ShaderStageType::Vertex, shader_->Get());

	piplineState->Compile();

	pipelineStates_[renderPassPipelineState] = piplineState;

	return piplineState;
}

}  // namespace altseed