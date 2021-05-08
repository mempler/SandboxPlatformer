#pragma once

#include "pch.hh"

#include "Core/Graphics/Texture2D.hh"

#include "Font.hh"

class Label {
public:
    Label(){};

    void SetText(const glm::vec3 &v2Pos, const std::string &sText, Font *pFont);
    void SetText(const std::string &sText, Font *pFont);
    void SetPosition(const glm::vec3 &v2Pos);
    void SetColor(const glm::vec4 &v4Color);

    void Render();

    static glm::vec2 CalculateTextSize(const std::string &sText, Font *pFont);

public:
    const glm::vec3 &GetPosition() const {
        return m_v3Pos;
    }

    const glm::vec2 &GetSize() const {
        return m_v2Size;
    }

private:
    void CalculateTransform();

private:
    struct RenderableChar {
        glm::vec4 uvs;
        glm::vec2 glyphPos;
        glm::vec2 size;
        glm::mat4x4 transform;
        glm::vec4 color;
        Texture2D *texture;
    };

    std::vector<RenderableChar> m_vChars; // - in cache

    std::string m_sText = "";

    Font *m_pUsingFont = 0;

    glm::vec3 m_v3Pos{};
    glm::vec2 m_v2Size{};
};