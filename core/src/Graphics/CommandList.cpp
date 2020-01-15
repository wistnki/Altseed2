#include "CommandList.h"
#include "../Graphics/Graphics.h"

namespace altseed {

std::shared_ptr<CommandList> CommandList::Create() {
    auto g = Graphics::GetInstance()->GetGraphicsLLGI();
    auto memoryPool = LLGI::CreateSharedPtr(g->CreateSingleFrameMemoryPool(1024 * 1024 * 16, 1024));
    auto commandListPool = std::make_shared<LLGI::CommandListPool>(g, memoryPool.get(), 3);
    auto ret = CreateSharedPtr(new CommandList());

    ret->memoryPool_ = memoryPool;
    ret->commandListPool_ = commandListPool;

    return ret;
}

void CommandList::Newframe() { currentCommandList_ = commandListPool_->Get(); }

void CommandList::SetScissor(const RectI& scissor) { currentCommandList_->SetScissor(scissor.X, scissor.Y, scissor.Width, scissor.Height); }

void CommandList::SetRenderTarget(std::shared_ptr<RenderTexture> target, const RectI& viewport) {}

void CommandList::Draw(
        const BatchVertex* vb, const int32_t* ib, int32_t vbCount, int32_t ibCount, const std::shared_ptr<Texture2D>& texture) {}

void CommandList::Flush() {
    if (isBatchRenderDirtied_) {
        batchRenderer_->Render(this);
        batchRenderer_->ResetCache();
    }
}

LLGI::CommandList* CommandList::GetLL() const { return currentCommandList_; }

}  // namespace altseed