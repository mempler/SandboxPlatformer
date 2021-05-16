#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "Core/Engine.hh"
#include "Core/Graphics/Surface/Surface.hh"

inline void DefaultImGuiLayout( Engine *pEngine, ImGuiID dock_main_id )
{
    BaseSurface *surface = pEngine->GetSurface();

    ImGui::DockBuilderRemoveNode( dock_main_id );  // Yeet away our old layout
    ImGui::DockBuilderAddNode( dock_main_id,
                               ImGuiDockNodeFlags_DockSpace );  // Add empty node
    ImGui::DockBuilderSetNodeSize( dock_main_id,
                                   ImVec2( surface->GetWidth(), surface->GetHeight() ) );

    ImGuiID dock_id_left = ImGui::DockBuilderSplitNode( dock_main_id, ImGuiDir_Left, 0.3f,
                                                        NULL, &dock_main_id );
    // ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id,
    // ImGuiDir_Right, 1.0f, NULL, &dock_main_id);

    ImGui::DockBuilderDockWindow( "IResource Monitor", dock_id_left );
    ImGui::DockBuilderDockWindow( "Game View", dock_main_id );
}
