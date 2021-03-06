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

#ifndef _IMPORT_H_
#define _IMPORT_H_

#include <glib.h>
#include <glib/gi18n.h>

// All these functions return 0 if the file is not recognized as the given format
//                            1 if the file is correctly recognized and imported
//                           <0 if the file is recognized, but it is not imported

gint import_csr (guchar *file_contents, gsize file_contents_size, gchar **csr_dn, guint64 *id);
gint import_certlist (guchar *file_contents, gsize file_contents_size, gchar **cert_dn, guint64 *id);
gint import_pkey_wo_passwd (guchar *file_contents, gsize file_contents_size);
gint import_crl (guchar *file_contents, gsize file_contents_size);

/* PKCS#7 importing was removed in libgnutls 2.6.0 */
/* gint import_pkcs7 (guchar *file_contents, gsize file_contents_size); */

gint import_pkcs8 (guchar *file_contents, gsize file_contents_size);

gint import_pkcs12 (guchar *file_contents, gsize file_contents_size);

gint import_openssl_private_key (const gchar *filename, gchar **last_password, gchar *file_description);
gboolean import_single_file (gchar *filename, gchar **dn, guint64 *id);
gchar * import_whole_dir (gchar *dirname);

#endif
