#pragma once 

#ifndef GUI_SYSTEM

#include "Glen/Globals.h"
#include "Glen/Core/Window.h"
#include "Glen/core/System.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

#include "SceneHeirarchyPanel.h"
#include "LogWindowPanel.h"

class GuiSystem : public ISystem{
private:
	SceneHeirarchyPanel sceneHeirarchyPanel;
	LogWindowPanel logWindowPanel;
public:
	GuiSystem() = default;

	virtual void update(float deltaTime) override;
	virtual void shutdown() override;
	virtual void startup() override;
};
#endif // !GUI_SYSTEM
