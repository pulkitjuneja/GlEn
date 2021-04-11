#pragma once 

#ifndef EDITOR_GUI

#include "Glen/Globals.h"
#include "Glen/Core/Window.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

#include "SceneHeirarchyPanel.h"
#include "LogWindowPanel.h"

class EditorGui {
private:
	SceneHeirarchyPanel sceneHeirarchyPanel;
	LogWindowPanel logWindowPanel;
public:
	EditorGui(Scene* scene);
	void update();
	void shutDown();
	void render();
};
#endif // !EDITOR_GUI
