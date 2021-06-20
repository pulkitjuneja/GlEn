#include  "LogWindowPanel.h"

ImVec4 LogWindowPanel::getColorFromLogType(std::string& logType)
{
	ImVec4 color;
	if (logType.compare("INFO") == 0)
		color = ImVec4(0, 1, 0, 1);
	else if (logType.compare("WARN") == 0)
		color = ImVec4(0, 1, 1, 1);
	else if (logType.compare("DEBUG") == 0)
		color = ImVec4(0, 0, 1, 1);
	else if (logType.compare("ERROR") == 0)
		color = ImVec4(1, 0, 0, 1);

	return color;
}

void LogWindowPanel::renderLogMessage(LogMessage& logMessage)
{
	ImVec4 color = getColorFromLogType(logMessage.logType);

	ImGui::Text(logMessage.time.c_str());
	ImGui::SameLine(0, 10);
	ImGui::TextColored(color, logMessage.logType.c_str());
	ImGui::SameLine(0, 10);
	ImGui::Text(logMessage.message.c_str());
}

void LogWindowPanel::update()
{

	std::string windowTitle = "Log";
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	bool show = true;
	ImGui::Begin(windowTitle.c_str() , &show);
	ImGui::End();
	if (!ImGui::Begin(windowTitle.c_str(), &show))
	{
		ImGui::End();
		return;
	}
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	filter.Draw("Filter", -100.0f);
	ImGui::Separator();

	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear) {
		Logger::clearLogBuffer();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	if (filter.IsActive()) {
		auto it = Logger::logBuffer.begin();
		for (; it != Logger::logBuffer.end(); it++) {
			bool messageFilter = filter.PassFilter((*it).message.c_str());
			bool typeFilter = filter.PassFilter((*it).logType.c_str());

			if (typeFilter || messageFilter) {
				renderLogMessage((*it));
			}
		}
	}

	else {
		ImGuiListClipper clipper;
		clipper.Begin(Logger::logBuffer.size());

		while (clipper.Step()) {

			auto it = Logger::logBuffer.begin();
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				auto it = Logger::logBuffer.begin();
				it += line_no;

				renderLogMessage((*it));
			}
		}
	}
	ImGui::PopStyleVar();

	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();

	// TODO : probably move this to another class
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 160, 40));
	ImGui::Begin("Stats");
	ImGui::TextColored(ImVec4(1, 1, 1, 1), "Frame time :%f", EngineContext::get()->stats.lastFrameTime);
	ImGui::End();
}
