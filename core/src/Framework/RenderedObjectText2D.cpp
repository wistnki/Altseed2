#include "RenderedObjectText2D.h"

namespace altseed {

void RenderedObjectText2D::Draw(RenderedObjectDrawingContext* context) {}

void RenderedObjectText2D::SetFont(const std::shared_ptr<Font>& font) { font_ = font; }

const char16_t* RenderedObjectText2D::GetText() const { return text_.c_str(); }

void RenderedObjectText2D::SetText(const char16_t* text) { text_ = text; }

}  // namespace altseed