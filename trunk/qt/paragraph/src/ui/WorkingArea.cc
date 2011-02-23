/*
 *	qt:paragraph:ui:WorkingArea.cc
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

// Self-include:
#include "WorkingArea.hh"

using namespace ku2::paragraph::ui;

WorkingArea::WorkingArea():
		m_dummyWidget(NULL)
{
	setObjectName("WorkingArea");
	createDummyTab();
}

void WorkingArea::createDummyTab()
{
	m_dummyWidget = new QWidget(this);

	//% "(no tabs are currently open)"
	//: This is the name of the dummy tab, when no other tabs are open.
	addTab(m_dummyWidget, qtTrId("qtn_no_tabs_open"));
}
