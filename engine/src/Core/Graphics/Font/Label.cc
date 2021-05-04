#include "Label.hh"

#include "Core/Engine.hh"

void Label::SetText(const glm::vec2 &v2Pos, const std::string &sText, Font *pFont) {
    m_v2Pos = v2Pos;
    m_sText = sText;

    SetText(sText, pFont);
}

void Label::SetText(const std::string &sText, Font *pFont) {
    m_sText = sText;

    if (m_sText == "")
        return;

    float linex = m_v2Pos.x;

    for (int i = 0; i < sText.length(); i++) {
        char c = sText[i];

        float kerning = 0.f;
        if (i > 0)
            pFont->GetKerning(sText[i - 1], c);

        linex += kerning;

        RenderableChar r;
        r.color = { 1, 1, 1, 1 };
        r.texture = pFont->GetTexture();

        ftgl::texture_glyph_t *g = pFont->GetGlyph(c);
        r.uvs = { g->s0, g->t0, g->s1, g->t1 };
        r.transform = glm::translate(glm::mat4(1.f), { linex + g->offset_x, m_v2Pos.y - g->offset_y, 1.f }) * glm::scale(glm::mat4(1.f), { g->width, g->height, 1.f });

        m_vChars.push_back(r);

        linex += g->advance_x;
    }
}

void Label::SetPosition(const glm::vec2 &v2Pos) {
    m_v2Pos = v2Pos;

    CalculateTransform();
}

void Label::Render() {
    for (auto &&r : m_vChars) GetEngine()->GetBatcher().SubmitWithRawUV(r.texture, r.transform, r.uvs, r.color);
}

void Label::CalculateTransform() {
    for (auto &&r : m_vChars) {
        // TODO
        //r.transform = glm::translate(glm::mat4(1.f), { m_v2Pos.x, m_v2Pos.y + g->offset_y, 1.f }) * glm::scale(glm::mat4(1.f), { g->advance_x, g->height, 1.f });
    }
}