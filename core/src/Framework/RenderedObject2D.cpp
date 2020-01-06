#include "RenderedObject2D.h"

namespace altseed {

const Matrix44F& RenderedObject2D::GetTransformMatrix() const { return transformMatrix_; }

void RenderedObject2D::SetTransformMatrix(const Matrix44F& matrix) { transformMatrix_ = matrix; }

int32_t RenderedObject2D::GetLayer() const { return layer_; }

}  // namespace altseed