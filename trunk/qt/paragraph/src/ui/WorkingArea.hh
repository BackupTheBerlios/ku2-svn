/*
 *	qt:paragraph:ui:WorkingArea.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__PARAGRAPH_UI_WORKING_AREA_HH__
#define KU_QT__PARAGRAPH_UI_WORKING_AREA_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// External includes:
#include <QTabWidget>

class QTableView;

namespace ku2 {
namespace paragraph {
namespace ui {

class WorkingArea: public QTabWidget
{
	Q_OBJECT

public:
	WorkingArea();

private:
	void createDummyTab();
	void closeDummyTab();

	void createGraphViewTab();
	void createGraphVisualTab();

private slots:
	void onGraphOpened( bool opened );
	void onVisualise();

private:
	QWidget *m_dummyWidget;
	QTableView *m_graphView;
};

}}}

#endif // KU_QT__PARAGRAPH_UI_WORKING_AREA_HH__
