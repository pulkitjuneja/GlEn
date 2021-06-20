#pragma once

#ifndef PANEHL_H
#define PANEL_H

class IPanel {
	virtual void update() = 0;
};

//class PanelList {
//private:
//	std::unordered_map<std::string, IPanel*> UIPanels;
//public:
//	template<typename t> void add();
//	void remove(IPanel* panel);
//};

#endif // !PANEHL_H
