#pragma once

#include <memory>

#include <map>
#include "../BaseObject.h"
#include "Shader.h"

namespace altseed {

class Shader;

class Material : public BaseObject {
private:
    std::shared_ptr<Shader> vertexShader_;
    std::shared_ptr<Shader> shader_;
    std::map<std::shared_ptr<LLGI::RenderPassPipelineState>, std::shared_ptr<LLGI::PipelineState>> pipelineStates_;

public:

    std::shared_ptr<Shader> GetShader() const { return shader_; }
    void SetShader(std::shared_ptr<Shader>& shader) { shader_ = shader; }

	std::shared_ptr<LLGI::PipelineState> GetPipelineState(LLGI::RenderPass* renderPass);
};

}  // namespace altseed
