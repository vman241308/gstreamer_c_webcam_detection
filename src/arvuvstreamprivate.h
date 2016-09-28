/* Aravis - Digital camera library
 *
 * Copyright © 2009-2016 Emmanuel Pacaud
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

#ifndef ARV_UV_STREAM_PRIVATE_H
#define ARV_UV_STREAM_PRIVATE_H

#include <arvuvstream.h>
#include <arvstream.h>
#include <arvuvdeviceprivate.h>

G_BEGIN_DECLS

typedef struct _ArvUvStreamPrivate ArvUvStreamPrivate;
typedef struct _ArvUvStreamClass ArvUvStreamClass;

struct _ArvUvStream {
	ArvStream	stream;

	ArvUvStreamPrivate *priv;
};

struct _ArvUvStreamClass {
	ArvStreamClass parent_class;
};

ArvStream * 	arv_uv_stream_new	(ArvUvDevice *uv_device, ArvStreamCallback callback, void *user_data);

G_END_DECLS

#endif
