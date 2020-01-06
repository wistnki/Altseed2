#include "RenderedObjectCamera2D.h"

namespace altseed {

void RenderedObjectCamera2D::Draw(RenderedObjectDrawingContext* context) {}

void RenderedObjectCamera2D::CalculateCameraProjectionMatrix(
        Matrix44F& cameraMatrix, Matrix44F& projectionMatrix, const Vector2DI& screenSize) {
    cameraMatrix = GetTransformMatrix().GetInverted();

    projectionMatrix.SetIdentity();

    projectionMatrix.Values[0][0] = 2.0f / static_cast<float>(screenSize.X);
    projectionMatrix.Values[1][1] = 2.0f / static_cast<float>(screenSize.Y);
}

}  // namespace altseed