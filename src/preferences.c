//  gnoMint: a graphical interface for managing a certification authority
//  Copyright (C) 2006-2009 David Marín Carreño <davefx@gmail.com>
//
//  This file is part of gnoMint.
//
//  gnoMint is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or   
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <libintl.h>
#include <gconf/gconf-client.h>

#include <glib/gi18n.h>

#include "preferences.h"


static GConfEngine * preferences_engine;

void preferences_init (int argc, char **argv)
{
        gconf_init (argc, argv, NULL);
        preferences_engine = gconf_engine_get_default ();
}


gboolean preferences_get_gnome_keyring_export ()
{
        return gconf_engine_get_bool (preferences_engine, "/apps/gnomint/gnome_keyring_export", NULL);
}

void preferences_set_gnome_keyring_export (gboolean new_value)
{
        gconf_engine_set_bool (preferences_engine, "/apps/gnomint/gnome_keyring_export", new_value, NULL);
}


void preferences_deinit ()
{
        gconf_engine_unref (preferences_engine);
        preferences_engine = NULL;
}

