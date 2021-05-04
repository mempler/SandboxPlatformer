#pragma once

#include "pch.hh"

#include "Core/Graphics/Texture2D.hh"

#include "Font.hh"

class Label {
public:
    Label(){};

    void SetText(const glm::vec2 &v2Pos, const std::string &sText, Font *pFont);
    void SetText(const std::string &sText, Font *pFont);

    void SetPosition(const glm::vec2 &v2Pos);

    void Render();

public:
    const glm::vec2 &GetPosition() const {
        return m_v2Pos;
    }

    const glm::vec2 &GetScale() const {
        return m_v2Scale;
    }

private:
    void CalculateTransform();
    void ProcessText();

private:
    struct RenderableChar {
        glm::vec4 uvs;
        glm::mat4x4 transform;
        glm::vec4 color;
        Texture2D *texture;
    };

    std::vector<RenderableChar> m_vChars; // - in cache

    std::string m_sText = "";

    glm::vec2 m_v2Pos{};
    glm::vec2 m_v2Scale{};
};