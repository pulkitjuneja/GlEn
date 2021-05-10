#pragma once

#ifndef LOGOWINDOW_PANEL_H

#include <imgui.h>
#include "Glen/Core/Logger.h"
#include "Panel.h"

class LogWindowPanel : public IPanel {
	ImGuiTextFilter     filter;
	ImVec4 getColorFromLogType(std::string& logType);
	void renderLogMessage(LogMessage& logMessage);
public:
	void update() override;
};

#endif // !LOGOWINDOW_PANEL_H
