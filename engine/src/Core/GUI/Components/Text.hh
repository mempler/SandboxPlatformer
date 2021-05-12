#pragma once

#include "Core/GUI/GUIComponent.hh"
#include "Core/Graphics/Font/Font.hh"
#include "Core/Graphics/Font/Label.hh"
#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

enum class TextAlign { Left, Center, Right };

class Text : public GUIComponent {
public:
    Text(Font *pFont = nullptr);

    void SetText(std::string_view const &svText);
    void SetTextAlignment(TextAlign eAlignment);

private:
    TextAlign m_eAlignment;

    std::string m_sOriginalText;
    std::vector<Label> m_vLabels;

    Font *m_sFont;

    void SetLabels();

    void Draw(float fDelta);
};
