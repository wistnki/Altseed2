#pragma once

#include "RenderedObject2D.h"

namespace altseed {

class RenderedObjectText2D : public RenderedObject2D {
private:

	std::u16string text_;


public:
    const char16_t* GetText() const;

    void SetText(const char16_t* text);
};

}  // namespace altseed