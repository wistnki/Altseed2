
#pragma once

#include <stdint.h>
#include <stdio.h>
#include "../BaseObject.h"
#include "../Math/Matrix44F.h"
#include "../Math/Vector2DF.h"
#include "BatchRenderer.h"

namespace altseed {

class RenderedTransform {
    Vector2DF position_;
    float rotation_ = 0.0f;
    Vector2DF scale_ = Vector2DF();

public:
    Matrix44F GetGlobalMatrix();
};

class RenderedObjectDrawingContext {
public:
    Matrix44F cameraMattrix;
    Matrix44F projectionaMatrix;
    std::shared_ptr<CommandList> commandList;
    std::shared_ptr<BatchRenderer> batchRenderer;
};

class RenderedObject2D : public BaseObject {
private:
    Matrix44F transformMatrix_;
    int32_t layer_ = 0;

public:
    virtual void Draw(RenderedObjectDrawingContext* context) {}

	const Matrix44F& GetTransformMatrix() const;
    void SetTransformMatrix(const Matrix44F& matrix);

    int32_t GetLayer() const;
};

}  // namespace altseed