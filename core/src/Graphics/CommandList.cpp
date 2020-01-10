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

LLGI::CommandList* CommandList::GetLL() const { return currentCommandList_; }

}  // namespace altseed