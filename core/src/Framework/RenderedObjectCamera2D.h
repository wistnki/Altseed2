#pragma once

#include "RenderedObject2D.h"

namespace altseed {

class RenderedObjectCamera2D : public RenderedObject2D {
private:
public:

	void Draw(RenderedObjectDrawingContext* context) override;

	void CalculateCameraProjectionMatrix(Matrix44F& cameraMatrix, Matrix44F& projectionMatrix, const Vector2DI& screenSize);
};

}  // namespace altseed