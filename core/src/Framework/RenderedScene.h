#pragma once

#include "../Math/Matrix44F.h"
#include "RenderedObject2D.h"

namespace altseed {

class CommandList;

class RenderedScene {
private:
public:
    void AddObject(std::shared_ptr<RenderedObject2D> obj);

    void RemoveObject(std::shared_ptr<RenderedObject2D> obj);

    void Render(std::shared_ptr<CommandList> commandList, const Matrix44& cameraMat, const Matrix44& projectionaMat, int32_t layerMask);
};

}  // namespace altseed