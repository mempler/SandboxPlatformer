#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"
#include "Core/Graphics/Font/Label.hh"

#include "Game/Game.hh"

#if GAME_SERVER
    #include "Server/Server.hh"
#endif
class Application : public BaseApp
{
  protected:
    void Init() override
    {
        m_pEngine->GetWindow().OnResize.connect<&Application::OnWindowResize>( this );

        m_pFont = m_pEngine->GetFontManager().LoadFromFile( "file://Roboto-Regular.ttf",
                                                            256, 256, 22.f );
        m_lWatermark.SetText( { 0, 0, 999.f }, "ICESDK DEMO", m_pFont );
        m_lWatermark.SetColor( { 1, 1, 1, .2 } );

        glm::vec3 center = { m_pEngine->GetWindow().Width() / 2.f
                                 - m_lWatermark.GetSize().x / 2,
                             m_pEngine->GetWindow().Height() - 100.f, 999.f };

        m_lWatermark.SetPosition( center );

        m_Game.Init();
    }

    void Tick( float fDelta ) override
    {
        m_Game.Tick( fDelta );
    }

    void Draw( float fDelta ) override
    {
        m_Game.Draw();

        m_lWatermark.Render();
    }

  public:
    Game &GetGame()
    {
        return m_Game;
    }

  private:
    void OnWindowResize( GameWindow *pGameWindow, uint32_t iWidth, uint32_t iHeight )
    {
        glm::vec3 center = { m_pEngine->GetWindow().Width() / 2.f
                                 - m_lWatermark.GetSize().x / 2.f,
                             m_pEngine->GetWindow().Height() - 100.f, 999.f };
        m_lWatermark.SetPosition( center );
    }

  private:
    Game m_Game;

    Font *m_pFont;
    Label m_lWatermark;
};

static Application *app = nullptr;

#if GAME_SERVER
static Server *g_pServer = nullptr;
#endif

int main()
{
#if GAME_SERVER
    g_pServer = new Server;

    g_pServer->Run();
#else
    app = new Application;

    app->Run();
#endif

    return 0;
}

BaseApp *GetApp()
{
    return app;
}

Game *GetGame()
{
    return &app->GetGame();
}

#if GAME_SERVER
Server *GetServer()
{
    return g_pServer;
}
#endif
