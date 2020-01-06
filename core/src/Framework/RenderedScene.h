#pragma once

#include <set>
#include <vector>
#include "../Math/Matrix44F.h"
#include "RenderedObject2D.h"

namespace altseed {

class CommandList;
class BatchRenderer;

class RenderedScene {
private:
    std::shared_ptr<BatchRenderer> batchRenderer_;
    std::set<std::shared_ptr<RenderedObject2D>> objects_;

public:
    RenderedScene();

    ~RenderedScene();

    void AddObject(std::shared_ptr<RenderedObject2D> obj);

    void RemoveObject(std::shared_ptr<RenderedObject2D> obj);

    void Render(
            std::shared_ptr<CommandList> commandList, const Matrix44F& cameraMatrix, const Matrix44F& projectionaMatrix, int32_t layerMask);
};

}  // namespace altseed