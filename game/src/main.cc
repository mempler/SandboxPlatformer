#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"

#include <glm/gtc/matrix_transform.hpp>

#include <freetype-gl.h>

#include <iostream>

class SandboxGame : public BaseApp {
protected:
    void Init() override {
        const char *cache = " !\"#$%&'()*+,-./0123456789:;<=>?"
                            "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                            "`abcdefghijklmnopqrstuvwxyz{|}~";

        atlas = ftgl::texture_atlas_new(512, 512, 4);
        ftgl::texture_font_t *font = ftgl::texture_font_new_from_file(atlas, 32.f, "arial.ttf");
        ftgl::texture_font_load_glyphs(font, cache);

        texture = m_pEngine->GetTextureManager().CreateTextureFromMemory(
            "engine://font-atlas-1", 512, 512, bgfx::TextureFormat::RGBA8, (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT), atlas->data, atlas->width * atlas->height * atlas->depth);

        // m_pSoundEffectChannel = m_pEngine->GetAudioSystem().CreateChannel("audio://sound_effects");
        // m_pAudio = m_pEngine->GetAudioSystem().LoadMonoAudio(m_pSoundEffectChannel, "file://audio.wav");

        // m_pAudio->SetPitch(1.25f);
        // m_pAudio->SetVolume(0.5f);
        // m_pAudio->Play();
    }

    void Tick(float fDelta) override {
    }

    void Draw(float fDelta) override {
        // TESTS

        // draw normal 100x100 rect
        m_pEngine->GetBatcher().SubmitRectangle(texture, glm::translate(glm::mat4(1.f), { 0.f, 0.f, 1.f }) * glm::scale(glm::mat4(1.f), { 512.f, 512.f, 1.f }));
    }

private:
    // AudioChannel *m_pSoundEffectChannel;
    // Audio *m_pAudio;

    glm::vec3 m_v3AudioPosition;

    ftgl::texture_atlas_t *atlas;
    Texture2D *texture;
};

static BaseApp *app = nullptr;

int main() {
    app = new SandboxGame;

    app->Run();

    return 0;
}

BaseApp *GetApp() {
    return app;
}
