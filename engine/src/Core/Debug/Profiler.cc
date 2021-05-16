#include "pch.hh"

#include "Profiler.hh"

#include "Core/Engine.hh"

void Profiler::Draw()
{
    if ( timer.elapsed() > 1 )
    {  // Shift every second
        // Shift the whoel thing to the left and cut off the first entry.
        for ( std::size_t i = 0; i < m_vFrames.size() - 1; ++i )
        {
            m_vFrames [ i ] = m_vFrames [ i + 1 ];
        }
        m_vFrames [ m_vFrames.size() - 1 ] =
            ImGui::GetIO().Framerate;  // Add a new entry

        if ( ImGui::GetIO().Framerate > m_fHighestFPS )
            m_fHighestFPS = ImGui::GetIO().Framerate;

        timer.reset();
    }

    ImGui::Begin( "Profiler", &m_bIsShowing );

    ImGui::Text( "FPS: %.2f, Max: %.2f", m_vFrames [ m_vFrames.size() - 1 ],
                 m_fHighestFPS );

    ImPlot::FitNextPlotAxes();
    if ( ImPlot::BeginPlot(
             "Profiler", NULL, NULL, { -1, 200 },
             ImPlotFlags_CanvasOnly | ImPlotFlags_AntiAliased,
             ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit ) )
    {
        ImPlot::PlotLine( "FPS", m_vFrames.data(), m_vFrames.size() );
        ImPlot::EndPlot();
    }

    ImGui::End();
}
