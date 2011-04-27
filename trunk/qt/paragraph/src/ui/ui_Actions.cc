//


// Self-include:
#include "ui_Actions.hh"

// Ku2 includes:
#include <ku2/debug.h>

// External includes:
#include <QString>
#include <QAction>

namespace ku2 {
namespace paragraph {
namespace ui {

struct Action {
	inline Action() {}
	inline Action( const QString &id, const QString &icon, const QString &text, QObject *parent ):
		action(new QAction(QIcon(icon), text, parent)), id(id) {
		pdebug("Creating action '%s` ('%s`, '%s`)\n",
		       qPrintable(id), qPrintable(icon), qPrintable(text));
	}
	QAction *action;
	QString id;
};

class ActionsHelper: public QObject {
public:
	ActionsHelper();
	QVector<Action> actions;
};
Q_GLOBAL_STATIC(ActionsHelper, actions);

}}}

using namespace ku2::paragraph::ui;

ActionsHelper::ActionsHelper()
{
	pstart();
	actions
	//% "New graph"
	<< Action("new-graph", ":/ui/graph-new.png", qtTrId("qtn_new_graph_action"), this)
	//% "Visualise a graph"
	<< Action("visualise-graph", ":/ui/graph-visualise.png", qtTrId("qtn_visualise_graph_action"), this)
	//% "Close the document"
	<< Action("save-document", ":/ui/document-save.png", qtTrId("qtn_save_document_action"), this)
	//% "Open a document"
	<< Action("open-document", ":/ui/document-open.png", qtTrId("qtn_open_document_action"), this)
	//% "Exit application"
	<< Action("exit-app", ":/ui/application-exit.png", qtTrId("qtn_exit_app_action"), this);
	pstop();
}

QAction *ku2::paragraph::ui::action( ActionId actionId )
{
	pstartp("actionId: %d", actionId);
	preturn actions()->actions.at(actionId).action;
}
