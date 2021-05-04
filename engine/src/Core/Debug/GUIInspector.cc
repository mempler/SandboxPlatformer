#include "GUIInspector.hh"

#include "imgui.h"

#include "Core/Engine.hh"
#include "Core/GUI/GUIComponent.hh"

void DrawChildAndItsChildren(GUIComponent *pComponent) {
    if (ImGui::TreeNode(pComponent->GetIdentifier().Raw().data())) {
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

            { // Identifier
                ImGui::TextUnformatted("Identifier:");
                ImGui::SameLine(150);
                ImGui::Text("%s", pComponent->GetIdentifier().Raw().data());
            }

            { // Color
                ImGui::TextUnformatted("Color:");
                ImGui::SameLine(150);
                ImGui::ColorEdit4("##Color", (float *)&pComponent->m_v4Color.r, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            }

            { // Position
                ImGui::TextUnformatted("Position:");
                ImGui::SameLine(150);
                ImGui::Text("Vec2(x: %.2f, y: %.2f)", pComponent->m_v2Position.x, pComponent->m_v2Position.y);
            }

            { // Size
                ImGui::TextUnformatted("Size:");
                ImGui::SameLine(150);
                ImGui::Text("Vec2(x: %.2f, y: %.2f)", pComponent->m_v2Size.x, pComponent->m_v2Size.y);
            }

            { // Scale
                ImGui::TextUnformatted("Scale:");
                ImGui::SameLine(150);
                ImGui::Text("Vec2(x: %.2f, y: %.2f)", pComponent->m_v2Scale.x, pComponent->m_v2Scale.y);
            }

            { // Rotation
                ImGui::TextUnformatted("Rotation:");
                ImGui::SameLine(150);
                ImGui::Text("%f", pComponent->m_fRotation);
            }

            ImGui::EndTooltip();
        }

        for (auto *child : *pComponent) {
            DrawChildAndItsChildren(child);
        }

        ImGui::TreePop();
    }
}

void GUIInspector::Draw() {
    ImGui::Begin("GUI Inspector");

    DrawChildAndItsChildren(m_pEngine->GetGUI());

    ImGui::End();
}
