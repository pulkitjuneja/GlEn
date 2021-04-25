#pragma once

#ifndef LOGOWINDOW_PANEL_H

#include <imgui.h>
#include "Glen/Core/Logger.h"

class LogWindowPanel {
	ImGuiTextFilter     filter;
	ImVec4 getColorFromLogType(std::string& logType);
	void renderLogMessage(LogMessage& logMessage);
public:
	void update();
};

#endif // !LOGOWINDOW_PANEL_H
