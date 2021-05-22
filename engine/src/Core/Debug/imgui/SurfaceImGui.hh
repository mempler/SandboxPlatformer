#pragma once
#include "imgui.h"  // IMGUI_IMPL_API

#include <Core/Graphics/Surface/Surface.hh>

IMGUI_IMPL_API bool ImGui_ImplSurface_Init( BaseSurface *pSurface );
IMGUI_IMPL_API void ImGui_ImplSurface_Shutdown();
IMGUI_IMPL_API void ImGui_ImplSurface_NewFrame();
