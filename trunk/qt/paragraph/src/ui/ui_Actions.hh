/*
 *	qt:paragraph:ui:Actions.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__PARAGRAPH_UI_ACTIONS_HH__
#define KU_QT__PARAGRAPH_UI_ACTIONS_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

class QAction;

namespace ku2 {
namespace paragraph {
namespace ui {

enum ActionId {
	ACT_NEW_GRAPH,
	ACT_VISUALISE_GRAPH,

	ACT_SAVE_DOCUMENT,
	ACT_OPEN_DOCUMENT,

	ACT_EXIT_APP,
	ACT_LAST_ENUM
};

QAction *action( ActionId actionId );

}}}

#endif // KU_QT__PARAGRAPH_UI_ACTIONS_HH__
