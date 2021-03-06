#include "CommandList.h"
#include "../Graphics/Graphics.h"
#include "RenderTexture.h"

namespace Altseed {

std::shared_ptr<CommandList> CommandList::Create() {
    auto g = Graphics::GetInstance()->GetGraphicsLLGI();
    auto memoryPool = LLGI::CreateSharedPtr(g->CreateSingleFrameMemoryPool(1024 * 1024 * 16, 128));  // TODO : fix DX12 bug
    auto commandListPool = std::make_shared<LLGI::CommandListPool>(g, memoryPool.get(), 3);
    auto ret = CreateSharedPtr(new CommandList());

    ret->memoryPool_ = memoryPool;
    ret->commandListPool_ = commandListPool;

    return ret;
}

void CommandList::StartFrame() {
    memoryPool_->NewFrame();
    currentCommandList_ = commandListPool_->Get();
    currentCommandList_->Begin();

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
        currentCommandList_->EndRenderPass();
        isInRenderPass_ = false;
        currentRenderPass_ = nullptr;
    }
    currentCommandList_->End();
}

void CommandList::SetScissor(const RectI& scissor) { currentCommandList_->SetScissor(scissor.X, scissor.Y, scissor.Width, scissor.Height); }

void CommandList::SetRenderTargetWithScreen() {
    auto g = Graphics::GetInstance()->GetGraphicsLLGI();

    if (isInRenderPass_) {
        currentCommandList_->EndRenderPass();
    }

    auto r = LLGI::CreateSharedPtr(Graphics::GetInstance()->GetCurrentScreen(LLGI::Color8(255, 0, 0, 255), true, true));
    r->AddRef();

    currentCommandList_->BeginRenderPass(r.get());
    currentRenderPass_ = r;
    isInRenderPass_ = true;
}

void CommandList::SetRenderTarget(std::shared_ptr<RenderTexture> target, const RectI& viewport) {
    auto it = renderPassCaches_.find(target);

    if (it == renderPassCaches_.end()) {
        auto g = Graphics::GetInstance()->GetGraphicsLLGI();

        LLGI::Texture* texture = target->GetNativeTexture().get();
        auto renderPass = LLGI::CreateSharedPtr(g->CreateRenderPass((const LLGI::Texture**)&texture, 1, nullptr));
        RenderPassCache cache;
        cache.Life = 5;
        cache.Stored = renderPass;
        renderPassCaches_[target] = cache;
    } else {
        // extend life to avoid to remove
        it->second.Life = 5;
    }

    if (isInRenderPass_) {
        currentCommandList_->EndRenderPass();
    }

    currentCommandList_->BeginRenderPass(renderPassCaches_[target].Stored.get());
    currentRenderPass_ = renderPassCaches_[target].Stored;
    isInRenderPass_ = true;
}

void CommandList::BlitScreenToTexture(std::shared_ptr<RenderTexture> target, std::shared_ptr<Material> material) {
    SetRenderTarget(target, RectI(0, 0, target->GetSize().X, target->GetSize().Y));

    // VB, IB
    currentCommandList_->SetVertexBuffer(blitVB_.get(), sizeof(BatchVertex), 0);
    currentCommandList_->SetIndexBuffer(blitIB_.get(), 0);

    // pipeline state
    currentCommandList_->SetPipelineState(material->GetPipelineState(GetCurrentRenderPass()).get());

    // draw
    currentCommandList_->Draw(2);
}

LLGI::SingleFrameMemoryPool* CommandList::GetMemoryPool() const { return memoryPool_.get(); }

LLGI::RenderPass* CommandList::GetCurrentRenderPass() const { return currentRenderPass_.get(); }

LLGI::CommandList* CommandList::GetLL() const { return currentCommandList_; }

}  // namespace Altseed