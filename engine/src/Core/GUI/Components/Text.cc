#include "Text.hh"

#include "Core/Engine.hh"
#include "Core/Managers/FontManager.hh"

Text::Text(Font *pFont) {
    if (pFont == nullptr) {
        //GetEngine()->GetFontManager().Load("engine://fonts/default.ttf", 512, 512, 24);
    }
}
