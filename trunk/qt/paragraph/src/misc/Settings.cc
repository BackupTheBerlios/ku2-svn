/*
 *	qt:paragraph:misc:Settings.cc
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
#include "Settings.hh"

using namespace ku2::paragraph;

Settings::Settings():
		QSettings("kaneApp", "paragraph")
{
}
