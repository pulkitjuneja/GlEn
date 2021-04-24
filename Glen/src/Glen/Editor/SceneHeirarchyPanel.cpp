#include "SceneHeirarchyPanel.h"

void SceneHeirarchyPanel::update()
{
	ImGui::SetNextWindowPos(ImVec2(20, 40));
	ImGui::Begin("Scene Heirarchy");

	std::vector<Entity*> entities = EngineContext::get()->sceneManager->getEntities();

	for (Entity* entity : entities) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), entity->name.c_str());
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Position");
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "x: %f", entity->getTransform()->getPosition().x);
		ImGui::SameLine(0);
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "y: %f", entity->getTransform()->getPosition().y);
		ImGui::SameLine(0);
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "z: %f", entity->getTransform()->getPosition().z);
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Rotation");
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "x: %f", entity->getTransform()->getEulerAngles().x * 57.2958);
		ImGui::SameLine(0);
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "y: %f", entity->getTransform()->getEulerAngles().y * 57.2958);
		ImGui::SameLine(0);
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "z: %f", entity->getTransform()->getEulerAngles().z * 57.2958);
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "Scale");
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "x: %f", entity->getTransform()->getScale().x);
		ImGui::SameLine(0);
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "y: %f", entity->getTransform()->getScale().y);
		ImGui::SameLine(0);
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "z: %f", entity->getTransform()->getScale().z);
	}

	ImGui::End();
}


