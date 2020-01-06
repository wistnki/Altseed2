#pragma once

#include "../Graphics/Color.h"
#include "RenderedObject2D.h"

namespace altseed {

class Font;

class RenderedObjectText2D : public RenderedObject2D {
private:
    std::u16string text_;

    std::shared_ptr<Font> font_;

public:
    void Draw(RenderedObjectDrawingContext* context) override;

    void SetFont(const std::shared_ptr<Font>& font);

    const char16_t* GetText() const;

    void SetText(const char16_t* text);
};

}  // namespace altseed