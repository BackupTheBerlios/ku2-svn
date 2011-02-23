/*
 *	qt:paragraph:ui:MainWindow.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__PARAGRAPH_UI_MAIN_WINDOW_HH__
#define KU_QT__PARAGRAPH_UI_MAIN_WINDOW_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// External includes:
#include <QMainWindow>
#include <QMap>

class QAction;

namespace ku2 {
namespace paragraph {
namespace ui {

class WorkingArea;

class MainWindow: public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private:
	void createActions();
	void createMenus();
	void createToolBars();

private:
	void saveWindow();
	void restoreWindow();

protected:
	/*!
	 * \overload?
	 */
	void closeEvent(QCloseEvent *event);

private slots:
	void onNewGraphAction();

private:
	//! List of actions.
	QMap<QString,QAction*> m_actions;

	//! Working area (central widget).
	WorkingArea *m_workingArea;
};

}}}
//Q_DECLARE_OPERATORS_FOR_FLAGS(ku2::graphviz::Graph::Types)

#endif // KU_QT__PARAGRAPH_UI_MAIN_WINDOW_HH__
