/* Aravis - Digital camera library
 *
 * Copyright © 2009-2012 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Author: Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef ARV_GC_REGISTER_H
#define ARV_GC_REGISTER_H

#include <arvtypes.h>

G_BEGIN_DECLS

#define ARV_TYPE_GC_REGISTER             	(arv_gc_register_get_type ())
#define ARV_GC_REGISTER(obj)             	(G_TYPE_CHECK_INSTANCE_CAST ((obj), ARV_TYPE_GC_REGISTER, ArvGcRegister))
#define ARV_IS_GC_REGISTER(obj)          	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARV_TYPE_GC_REGISTER))
#define ARV_GC_REGISTER_GET_INTERFACE(obj)   	(G_TYPE_INSTANCE_GET_INTERFACE((obj), ARV_TYPE_GC_REGISTER, ArvGcRegisterInterface))

typedef struct _ArvGcRegisterInterface ArvGcRegisterInterface;

struct _ArvGcRegisterInterface {
	GTypeInterface parent;

	void 		(*get)			(ArvGcRegister *gc_register, void *buffer, guint64 length, GError **error);
	void 		(*set)			(ArvGcRegister *gc_register, void *buffer, guint64 length, GError **error);
	guint64		(*get_address) 		(ArvGcRegister *gc_register, GError **error);
	guint64 	(*get_length)		(ArvGcRegister *gc_register, GError **error);
};

GType arv_gc_register_get_type (void);

void 		arv_gc_register_get			(ArvGcRegister *gc_register, void *buffer, guint64 length, GError **error);
void 		arv_gc_register_set			(ArvGcRegister *gc_register, void *buffer, guint64 length, GError **error);
guint64 	arv_gc_register_get_address 		(ArvGcRegister *gc_register, GError **error);
guint64 	arv_gc_register_get_length		(ArvGcRegister *gc_register, GError **error);

G_END_DECLS

#endif
