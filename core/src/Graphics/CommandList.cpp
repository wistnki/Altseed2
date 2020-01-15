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

void CommandList::StartFrame() {
    currentCommandList_ = commandListPool_->Get();

    for (auto& c : renderPassCaches_) {
        c.second.Life--;
    }

    for (auto it = renderPassCaches_.begin(); it != renderPassCaches_.end();) {
        if (it->second.Life == 0) {
            it = renderPassCaches_.erase(it);
        } else {
            it++;
        }
    }
}

void CommandList::EndFrame() {
    if (isInRenderPass_) {
        Flush();
        currentCommandList_->EndRenderPass();
        isInRenderPass_ = false;
    }
}

void CommandList::SetScissor(const RectI& scissor) { currentCommandList_->SetScissor(scissor.X, scissor.Y, scissor.Width, scissor.Height); }

void CommandList::SetRenderTarget(std::shared_ptr<RenderTexture> target, const RectI& viewport) {
    auto it = renderPassCaches_.find(target);

    if (it == renderPassCaches_.end()) {
        auto g = Graphics::GetInstance()->GetGraphicsLLGI();

        LLGI::Texture* texture = nullptr;
        auto renderPass = LLGI::CreateSharedPtr(g->CreateRenderPass(&texture, 1, nullptr));
        RenderPassCache cache;
        cache.Life = 5;
        cache.Stored = renderPass;
        renderPassCaches_[target] = cache;
    } else {
        // extend life to avoid to remove
        it->second.Life = 5;
    }

    if (isInRenderPass_) {
        Flush();
        currentCommandList_->EndRenderPass();
    }

    // TODO BeginRenderPass

    isInRenderPass_ = true;
}

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