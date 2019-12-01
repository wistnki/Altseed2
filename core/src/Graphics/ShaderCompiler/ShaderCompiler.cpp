#include "ShaderCompiler.h"
#include "../Graphics.h"

namespace altseed {

std::shared_ptr<ShaderCompiler> ShaderCompiler::instance_ = nullptr;

std::shared_ptr<ShaderCompiler>& ShaderCompiler::GetInstance() { return instance_; }

bool ShaderCompiler::Initialize(std::shared_ptr<Graphics>& graphics) {
    instance_ = CreateAndAddSharedPtr(new ShaderCompiler(graphics));
    instance_->Release();
    return true;
}

void ShaderCompiler::Terminate() { instance_ = nullptr; }

ShaderCompiler::ShaderCompiler(std::shared_ptr<Graphics>& graphics) {
    spirvGenerator_ = std::make_shared<SPIRVGenerator>();

// TODO : change with graphics
#ifdef _WIN32
    // spirvTranspiler_ = std::make_shared<SPIRVToHLSLTranspiler>();
#elif __APPLE__
    spirvTranspiler_ = std::make_shared<SPIRVToMSLTranspiler>();
#else
    spirvTranspiler_ = std::make_shared<SPIRVToGLSLTranspiler>(true);
#endif
}

std::shared_ptr<Shader> ShaderCompiler::Compile(char* code, ShaderStageType shaderStage) {
    auto spirv = spirvGenerator_->Generate(code, shaderStage);
    if (!spirvTranspiler_->Transpile(spirv)) {
    }

	return nullptr;
}

}  // namespace altseed