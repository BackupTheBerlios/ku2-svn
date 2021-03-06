/*
 *	qt:paragraph:misc:Settings.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__PARAGRAPH_MISC_SETTINGS_HH__
#define KU_QT__PARAGRAPH_MISC_SETTINGS_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// External includes:
#include <QSettings>

KU_BEGIN_DECLS
namespace paragraph {

class Settings: public QSettings
{
public:
	Settings();
};

}
KU_END_DECLS

#endif // KU_QT__PARAGRAPH_MISC_SETTINGS_HH__
