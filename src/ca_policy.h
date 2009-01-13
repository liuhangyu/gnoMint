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

#ifndef _CA_POLICY_H_
#define _CA_POLICY_H_

void ca_policy_populate (guint64 ca_id);
guint ca_policy_get (guint64 ca_id, gchar *property_name);
void ca_policy_set (guint64 ca_id, gchar *property_name, guint value);

void ca_policy_expiration_spin_button_change (gpointer spin_button, gpointer userdata);
void ca_policy_crl_update_spin_button_change (gpointer spin_button, gpointer userdata);
void ca_policy_toggle_button_toggled (gpointer button, gpointer userdata);

#endif
