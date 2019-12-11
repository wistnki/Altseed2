#include "RenderedScene.h"

namespace altseed {

void RenderedScene::AddObject(std::shared_ptr<RenderedObject2D> obj) {}

void RenderedScene::RemoveObject(std::shared_ptr<RenderedObject2D> obj) {}

void RenderedScene::Render(
        std::shared_ptr<CommandList> commandList, const Matrix44& cameraMat, const Matrix44& projectionaMat, int32_t layerMask) {}

}  // namespace altseed