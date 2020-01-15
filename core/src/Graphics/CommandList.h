
#pragma once

#include "../BaseObject.h"
#include <Utils/LLGI.CommandListPool.h>
#include <LLGI.Graphics.h>
#include "../Math/RectI.h"
#include "../Graphics/Color.h"
#include "../Math/Vector2DF.h"
#include "../Math/Vector3DF.h"
#include "../Framework/BatchRenderer.h"

namespace altseed {

class Graphics;
class RenderTexture;

struct BatchVertex {
    Vector3DF Pos;
    Color Col;
    Vector2DF UV1;
    Vector2DF UV2;
};


class CommandList : public BaseObject {
private:

    LLGI::CommandList* currentCommandList_ = nullptr;
    std::shared_ptr<LLGI::SingleFrameMemoryPool> memoryPool_;
    std::shared_ptr<LLGI::CommandListPool> commandListPool_;
    std::shared_ptr<BatchRenderer> batchRenderer_;
    bool isBatchRenderDirtied_ = false;

public:
    static std::shared_ptr<CommandList> Create();

	void Newframe();

	void SetScissor(const RectI& scissor);

	void SetRenderTarget(std::shared_ptr<RenderTexture> target, const RectI& viewport);

	void Draw(const BatchVertex* vb, const int32_t* ib, int32_t vbCount, int32_t ibCount, const std::shared_ptr<Texture2D>& texture);

	void Flush();

    LLGI::CommandList* GetLL() const;
};

}  // namespace altseed