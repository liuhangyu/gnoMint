//  gnoMint: a graphical interface for managing a certification authority
//  Copyright (C) 2006 David Marín Carreño <davefx@gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or   
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

#include "csr_creation.h"
#include "ca_file.h"
#include "tls.h"

#include <stdio.h>
#include <gnutls/gnutls.h>
#include <sqlite3.h>

#include <libintl.h>
#define _(x) gettext(x)
#define N_(x) (x) gettext_noop(x)


gint csr_creation_is_launched = -1;

static GStaticMutex csr_creation_thread_status_mutex = G_STATIC_MUTEX_INIT;
gint csr_creation_thread_status = 0;
gchar * csr_creation_message = "";

gchar * csr_creation_database_save (CaCreationData * creation_data, 
				   gchar * private_key, 
				   gchar * root_certificate);

gpointer csr_creation_thread (gpointer data)
{
	CaCreationData *creation_data = (CaCreationData *) data;
	
	gchar * private_key = NULL;
	gnutls_x509_privkey_t * csr_key = NULL;
	gchar * certificate_sign_request = NULL;
	gchar * error_message = NULL;

	switch (creation_data->key_type){
	case 0: /* RSA */
		g_static_mutex_lock (&csr_creation_thread_status_mutex);
		csr_creation_message =  _("Generating new RSA key pair");
		g_static_mutex_unlock (&csr_creation_thread_status_mutex);

		error_message = tls_generate_rsa_keys (creation_data, &private_key, &csr_key);		
		if (error_message) {
			printf ("%s\n\n", error_message);

			g_static_mutex_lock (&csr_creation_thread_status_mutex);

			csr_creation_message = g_strdup_printf ("%s:\n%s",_("Key generation failed"), error_message); 
			csr_creation_thread_status = -1;

			g_static_mutex_unlock (&csr_creation_thread_status_mutex);

			return NULL;
			// return error_message;
		}

		break;

	case 1: /* DSA */
		g_static_mutex_lock (&csr_creation_thread_status_mutex);
		csr_creation_message =  _("Generating new DSA key pair");
		g_static_mutex_unlock (&csr_creation_thread_status_mutex);

 		error_message = tls_generate_dsa_keys (creation_data, &private_key, &csr_key);

		if (error_message) { 
			printf ("%s\n\n", error_message);

 			g_static_mutex_lock (&csr_creation_thread_status_mutex); 

			csr_creation_message = g_strdup_printf ("%s:\n%s",_("Key generation failed"), error_message); 
 			csr_creation_thread_status = -1; 

 			g_static_mutex_unlock (&csr_creation_thread_status_mutex); 


 			//return error_message; 
			return NULL;
 		} 

		break;
	}

	g_static_mutex_lock (&csr_creation_thread_status_mutex);
	csr_creation_message =  _("Generating CSR");
	g_static_mutex_unlock (&csr_creation_thread_status_mutex);	

	error_message = tls_generate_csr (creation_data, csr_key, &certificate_sign_request);
	
 	if (error_message) {
		printf ("%s\n\n", error_message);
 		g_static_mutex_lock (&csr_creation_thread_status_mutex); 
		
 		csr_creation_message = g_strdup_printf ("%s:\n%s",_("CSR generation failed"), error_message); 
 		csr_creation_thread_status = -1; 
		
 		g_static_mutex_unlock (&csr_creation_thread_status_mutex);

		g_free (error_message);
 		//return error_message; 
		return NULL;
 	} 

	g_static_mutex_lock (&csr_creation_thread_status_mutex);
	csr_creation_message =  _("Saving CSR in database");
	g_static_mutex_unlock (&csr_creation_thread_status_mutex);

	error_message = csr_creation_database_save (creation_data, private_key, certificate_sign_request);
	if (error_message) {
		g_static_mutex_lock (&csr_creation_thread_status_mutex);
		
 		csr_creation_message = g_strdup_printf ("%s:\n%s",_("CSR couldn't be saved"), error_message); 
		csr_creation_thread_status = -1;
		
		g_static_mutex_unlock (&csr_creation_thread_status_mutex);
		
		g_free (error_message);

		return NULL;
	}

	printf ("\n\n%s\n\n", certificate_sign_request);

	g_static_mutex_lock (&csr_creation_thread_status_mutex);
	csr_creation_message =  _("CSR generated successfully");
	csr_creation_thread_status = 1;
	g_static_mutex_unlock (&csr_creation_thread_status_mutex);
	
	if (csr_key) {
		gnutls_x509_privkey_deinit ((* csr_key));
		g_free (csr_key);
	}

	return NULL;
	

}



GThread * csr_creation_launch_thread (CaCreationData *creation_data)
{
	return g_thread_create (csr_creation_thread,
				creation_data,
				TRUE,
				NULL);
}

void csr_creation_lock_status_mutex ()
{
	g_static_mutex_lock(&csr_creation_thread_status_mutex);
}

void csr_creation_unlock_status_mutex ()
{
	g_static_mutex_unlock (&csr_creation_thread_status_mutex);
}


gint csr_creation_get_thread_status ()
{	
	return csr_creation_thread_status;
}

gchar * csr_creation_get_thread_message()
{
	return csr_creation_message;
}

gchar * csr_creation_database_save (CaCreationData * creation_data, 
				    gchar * private_key, 
				    gchar * certificate_sign_request)
{
	return ca_file_insert_csr (creation_data, 
				   private_key,
				   certificate_sign_request);
}
