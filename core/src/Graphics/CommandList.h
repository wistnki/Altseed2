
#pragma once

#include "../BaseObject.h"
#include <Utils/LLGI.CommandListPool.h>
#include <LLGI.Graphics.h>

namespace altseed {

class Graphics;



class CommandList : public BaseObject {
private:

    LLGI::CommandList* currentCommandList_ = nullptr;
    std::shared_ptr<LLGI::SingleFrameMemoryPool> memoryPool_;
    std::shared_ptr<LLGI::CommandListPool> commandListPool_;

public:
    static std::shared_ptr<CommandList> Create();

	void Newframe();

    // BeginRenderPass (cache renderpass class)

    // EndRenderPass

    // SetViewport

    // SetXXX

    // Draw

    LLGI::CommandList* GetLL() const;
};

}  // namespace altseed