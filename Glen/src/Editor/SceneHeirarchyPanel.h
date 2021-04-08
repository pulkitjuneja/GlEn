#pragma once

#ifndef SCENE_HEIRARCHY_PANEL_H

#include "../Scene.h"
#include <imgui.h>


// TODO: Abstract this into a separate panel class
class SceneHeirarchyPanel {
private:
	Scene* sceneRef;
public:
	void setSceneRef(Scene* scene);
	void update();
};

#endif // !SCENE_HEIRARCHY_PANEL_H
