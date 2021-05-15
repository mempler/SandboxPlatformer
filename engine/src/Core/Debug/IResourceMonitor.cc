#include "pch.hh"

#include "IResourceMonitor.hh"

#include "Core/Engine.hh"

void IResourceMonitor::Draw() {
#if ENGINE_DEBUG
    ImGui::Begin("IResource Monitor", &m_bIsShowing);

    // Currently we only support TextureManager as that one uses the newer IResource class

    if (ImGui::CollapsingHeader("TextureManager")) {
        if (ImGui::BeginTable("##texture_manager", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders)) {
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

            ImGui::TableNextColumn();
            ImGui::Text("Identifier");

            ImGui::TableNextColumn();
            ImGui::Text("Memory Usage");

            for (auto &resource : m_pEngine->GetTextureManager()) {
                ImGui::TableNextColumn();

                // NOTE: this is very expensive, lots of CPU time wasted. but it should be fine.
                //       as this is a debugging utility anyway
                ImGui::Text("%s", (const char *)resource.first);

                ImGui::TableNextColumn();
                ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "%s", Memory::SizeInText((double)resource.second.GetDataSize()).c_str()); // TODO: Implement
            }

            ImGui::EndTable();
        }
    }

    ImGui::End();
#endif
}
