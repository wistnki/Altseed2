#include "RenderedScene.h"
#include "../Graphics/Graphics.h"
#include "BatchRenderer.h"

namespace altseed {

RenderedScene::RenderedScene() {}

RenderedScene ::~RenderedScene() {}

void RenderedScene::AddObject(std::shared_ptr<RenderedObject2D> obj) { objects_.insert(obj); }

void RenderedScene::RemoveObject(std::shared_ptr<RenderedObject2D> obj) { objects_.erase(obj); }

void RenderedScene::Render(
        std::shared_ptr<CommandList> commandList, const Matrix44F& cameraMatrix, const Matrix44F& projectionaMatrix, int32_t layerMask) {
    RenderedObjectDrawingContext context;

    batchRenderer_->ResetCache();

    context.batchRenderer = batchRenderer_;
    context.cameraMattrix = cameraMatrix;
    context.projectionaMatrix = projectionaMatrix;
    context.commandList = commandList;

    for (auto& obj : objects_) {
        if (((1 << obj->GetLayer()) & layerMask) == 0) continue;

        obj->Draw(&context);
    }

    batchRenderer_->Render(commandList);
}

}  // namespace altseed