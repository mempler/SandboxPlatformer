#include "Label.hh"

#include "Core/Engine.hh"
#include "Core/Utils/Math.hh"

void Label::SetText( const glm::vec3 &v3Pos, const std::string &sText,
                     Font *pFont )
{
    ZoneScoped;

    m_v3Pos = v3Pos;
    m_sText = sText;
    m_pUsingFont = pFont;

    SetText( sText, pFont );
}

void Label::SetText( const std::string &sText, Font *pFont )
{
    ZoneScoped;

    m_sText = sText;
    m_pUsingFont = pFont;

    m_vChars.clear();

    if ( m_sText == "" ) return;

    float linex = 0.f;

    for ( int i = 0; i < sText.length(); i++ )
    {
        char c = sText [ i ];

        m_v2Size.y =
            pFont->GetHandle()->height;  // setting this at the top to make sure
                                         // the text has at least one char

        float kerning = 0.f;
        if ( i > 0 ) pFont->GetKerning( sText [ i - 1 ], c );

        linex += kerning;

        RenderableChar r;
        r.color = { 1, 1, 1, 1 };
        r.texture = pFont->GetTexture();

        ftgl::texture_glyph_t *g = pFont->GetGlyph( c );
        r.uvs = { g->s0, g->t0, g->s1, g->t1 };

        r.size = { g->width, g->height };
        r.glyphPos = { linex + g->offset_x,
                       g->offset_y + pFont->GetHandle()->ascender };

        r.transform = Math::CalcTransform(
            { m_v3Pos.x + linex + g->offset_x,
              m_v3Pos.y - g->offset_y + pFont->GetHandle()->ascender,
              m_v3Pos.z },
            { g->width, g->height } );

        m_vChars.push_back( r );

        linex += g->advance_x;
        m_v2Size.x = linex;
    }
}

void Label::SetPosition( const glm::vec3 &v3Pos )
{
    ZoneScoped;

    m_v3Pos = v3Pos;

    CalculateTransform();
}

void Label::SetColor( const glm::vec4 &v4Color )
{
    ZoneScoped;

    for ( auto &&r : m_vChars ) r.color = v4Color;
}

void Label::Render()
{
    ZoneScoped;

    for ( auto &&r : m_vChars )
        GetEngine()->GetBatcher().SubmitWithRawUV( r.texture, r.transform,
                                                   r.uvs, r.color );
}

glm::vec2 Label::CalculateTextSize( const std::string &sText, Font *pFont )
{
    ZoneScoped;

    glm::vec2 size { 0.f, 0.f };

    for ( int i = 0; i < sText.length(); i++ )
    {
        char c = sText [ i ];

        size.y =
            pFont->GetHandle()->height;  // setting this at the top to make sure
                                         // the text has at least one char

        float kerning = 0.f;
        if ( i > 0 ) pFont->GetKerning( sText [ i - 1 ], c );

        ftgl::texture_glyph_t *g = pFont->GetGlyph( c );

        size.x += g->advance_x + kerning;
    }

    return size;
}

void Label::CalculateTransform()
{
    ZoneScoped;

    for ( auto &r : m_vChars )
        r.transform = Math::CalcTransform(
            { m_v3Pos.x + r.glyphPos.x, m_v3Pos.y - r.glyphPos.y, m_v3Pos.z },
            r.size );
}