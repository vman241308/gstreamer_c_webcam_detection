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
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef ARV_GC_INDEX_NODE_H
#define ARV_GC_INDEX_NODE_H

#include <arvtypes.h>
#include <arvgcpropertynode.h>

G_BEGIN_DECLS

#define ARV_TYPE_GC_INDEX_NODE             (arv_gc_index_node_get_type ())
#define ARV_GC_INDEX_NODE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ARV_TYPE_GC_INDEX_NODE, ArvGcIndexNode))
#define ARV_GC_INDEX_NODE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ARV_TYPE_GC_INDEX_NODE, ArvGcIndexNodeClass))
#define ARV_IS_GC_INDEX_NODE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARV_TYPE_GC_INDEX_NODE))
#define ARV_IS_GC_INDEX_NODE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ARV_TYPE_GC_INDEX_NODE))
#define ARV_GC_INDEX_NODE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), ARV_TYPE_GC_INDEX_NODE, ArvGcIndexNodeClass))

typedef struct _ArvGcIndexNodeClass ArvGcIndexNodeClass;

struct _ArvGcIndexNode {
	ArvGcPropertyNode	base;

	char *offset;
	gboolean is_p_offset;
};

struct _ArvGcIndexNodeClass {
	ArvGcPropertyNodeClass parent_class;
};

GType 		arv_gc_index_node_get_type 	(void);
ArvGcNode * 	arv_gc_index_node_new 		(void);
gint64		arv_gc_index_node_get_index	(ArvGcIndexNode *index_node, gint64 default_offset, GError **error);

G_END_DECLS

#endif
