#pragma once

#ifndef SCENE_HEIRARCHY_PANEL_H

#include "Glen/Scene/Scene.h"
#include <imgui.h>
#include "Glen/Core/EngineContext.h"
#include "Panel.h"


// TODO: Abstract this into a separate panel class
class SceneHeirarchyPanel : public IPanel {
private:
public:
	void update() override;
};

#endif // !SCENE_HEIRARCHY_PANEL_H
