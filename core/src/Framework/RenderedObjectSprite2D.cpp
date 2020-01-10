#include "RenderedObjectSprite2D.h"

namespace altseed {

void RenderedObjectSprite2D::Draw(RenderedObjectDrawingContext* context) {
    std::array<BatchVertex, 4> vb;
    std::array<int32_t, 6> ib;

    vb[0].Pos.X = 0.0f;
    vb[0].Pos.Y = 0.0f;

    vb[0].UV1.X = 0.0f;
    vb[0].UV1.Y = 0.0f;

    vb[1].Pos.X = renderingSize_.Width;
    vb[1].Pos.Y = 0.0f;

    vb[1].UV1.X = 1.0f;
    vb[1].UV1.Y = 0.0f;

    vb[2].Pos.X = 0.0f;
    vb[2].Pos.Y = renderingSize_.Height;

    vb[2].UV1.X = 0.0f;
    vb[2].UV1.Y = 1.0f;

    vb[3].Pos.X = renderingSize_.Width;
    vb[3].Pos.Y = renderingSize_.Height;

    vb[3].UV1.X = 1.0f;
    vb[3].UV1.Y = 1.0f;

    for (size_t i = 0; i < 4; i++) {
        vb[i].Pos.Z = 0.5f;
        vb[i].UV2 = vb[1].UV1;
        vb[i].Col = Color(255, 255, 255, 255);
    }

    ib[0] = 0;
    ib[1] = 1;
    ib[2] = 3;
    ib[3] = 0;
    ib[4] = 3;
    ib[5] = 2;

    auto mat = context->projectionMatrix * context->cameraMattrix * this->GetTransformMatrix();

    for (size_t i = 0; i < 4; i++) {
        vb[i].Pos = mat.Transform3D(vb[i].Pos);
    }

	context->batchRenderer->Draw(vb.data(), ib.data(), 4, 6, nullptr);
}
}  // namespace altseed