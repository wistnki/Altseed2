#pragma once

#include "../Math/RectF.h"
#include "RenderedObject2D.h"

namespace altseed {

class Material;
class Texture2D;

class RenderedObjectSprite2D : public RenderedObject2D {
private:
    std::shared_ptr<Texture2D> texture_;
    std::shared_ptr<Material> material_;
    RectF renderingSize_;

public:
    void Draw(RenderedObjectDrawingContext* context) override;

    void SetTexture(const std::shared_ptr<Texture2D>& texture) { texture_ = texture; }

    void SetMaterial(const std::shared_ptr<Material>& material) { material_ = material; }
};

}  // namespace altseed