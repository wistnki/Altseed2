
#pragma once

#include <stdint.h>
#include <stdio.h>
#include "../BaseObject.h"
#include "../Math/Matrix44F.h"
#include "../Math/Vector2DF.h"

namespace altseed {

class RenderedTransform {
    Vector2DF position_;
    float rotation_ = 0.0f;
    Vector2DF scale_ = Vector2DF();

public:
    Matrix44 GetGlobalMatrix();
};

class RenderedObject2D : public BaseObject {
public:

    void SetTransformMatrix(const Matrix44& matrix);


};

}  // namespace altseed