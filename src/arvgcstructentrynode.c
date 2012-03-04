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

/**
 * SECTION: arvgcstructentrynode
 * @short_description: Class for StructEntry nodes
 */

#include <arvgcstructentrynode.h>
#include <arvgcregister.h>
#include <arvgcinteger.h>
#include <arvgcport.h>
#include <arvgc.h>
#include <arvmisc.h>
#include <arvdebug.h>
#include <stdlib.h>
#include <string.h>

/**
 * SECTION: arvgcstructentrynode
 * @short_description: Base class for StrucEntry nodes
 *
 * #ArvGcStructEntryNode provides a base class for the implementation of the different
 * types of Genicam node.
 */

static GObjectClass *parent_class = NULL;

/* ArvDomNode implementation */

static const char *
arv_gc_struct_entry_node_get_node_name (ArvDomNode *node)
{
	return "StructEntry";
}

static void
arv_gc_struct_entry_node_post_new_child (ArvDomNode *self, ArvDomNode *child)
{
	ArvGcStructEntryNode *node = ARV_GC_STRUCT_ENTRY_NODE (self);

	if (ARV_IS_GC_PROPERTY_NODE (child)) {
		ArvGcPropertyNode *property_node = ARV_GC_PROPERTY_NODE (child);

		switch (arv_gc_property_node_get_node_type (property_node)) {
			case ARV_GC_PROPERTY_NODE_TYPE_SIGN:
				/* TODO */
				node->sign = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_LSB:
				node->lsb = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_MSB:
				node->msb = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_BIT:
				node->msb = property_node;
				node->lsb = property_node;
				break;
			default:
				ARV_DOM_NODE_CLASS (parent_class)->post_new_child (self, child);
				break;
		}
	}
}

static void
arv_gc_struct_entry_node_pre_remove_child (ArvDomNode *self, ArvDomNode *child)
{
	g_assert_not_reached ();
}

/* ArvGcFeatureNode implementation */

static GType
arv_gc_struct_entry_node_get_value_type (ArvGcFeatureNode *node)
{
	return G_TYPE_INT64;
}

static void
arv_gc_struct_entry_node_set_value_from_string (ArvGcFeatureNode *node, const char *string)
{
	arv_gc_integer_set_value (ARV_GC_INTEGER (node), g_ascii_strtoll (string, NULL, 0));
}

static const char *
arv_gc_struct_entry_node_get_value_as_string (ArvGcFeatureNode *node)
{
	ArvGcStructEntryNode *gc_struct_entry_node = ARV_GC_STRUCT_ENTRY_NODE (node);

	g_snprintf (gc_struct_entry_node->v_string, G_ASCII_DTOSTR_BUF_SIZE,
		    "0x%08" G_GINT64_MODIFIER "x", arv_gc_integer_get_value (ARV_GC_INTEGER (node)));
	return gc_struct_entry_node->v_string;
}

/* ArvGcStructEntryNode implementation */

/* Set default to read only 32 bits little endian integer struct_entry_node */

static ArvGcCachable
_get_lsb (ArvGcStructEntryNode *gc_struct_entry_node)
{
	if (gc_struct_entry_node->lsb == NULL)
		return 0;

	return arv_gc_property_node_get_int64 (gc_struct_entry_node->lsb);
}

static ArvGcCachable
_get_msb (ArvGcStructEntryNode *gc_struct_entry_node)
{
	if (gc_struct_entry_node->msb == NULL)
		return 31;

	return arv_gc_property_node_get_int64 (gc_struct_entry_node->msb);
}

ArvGcNode *
arv_gc_struct_entry_node_new (void)
{
	ArvGcStructEntryNode *gc_struct_entry_node;

	gc_struct_entry_node = g_object_new (ARV_TYPE_GC_STRUCT_ENTRY_NODE, NULL);

	return ARV_GC_NODE (gc_struct_entry_node);
}

static void
arv_gc_struct_entry_node_init (ArvGcStructEntryNode *gc_struct_entry_node)
{
}

static void
arv_gc_struct_entry_node_finalize (GObject *object)
{
	parent_class->finalize (object);
}

static void
arv_gc_struct_entry_node_class_init (ArvGcStructEntryNodeClass *this_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (this_class);
	ArvDomNodeClass *dom_node_class = ARV_DOM_NODE_CLASS (this_class);
	ArvGcFeatureNodeClass *gc_feature_node_class = ARV_GC_FEATURE_NODE_CLASS (this_class);

	parent_class = g_type_class_peek_parent (this_class);

	object_class->finalize = arv_gc_struct_entry_node_finalize;
	dom_node_class->get_node_name = arv_gc_struct_entry_node_get_node_name;
	dom_node_class->post_new_child = arv_gc_struct_entry_node_post_new_child;
	dom_node_class->pre_remove_child = arv_gc_struct_entry_node_pre_remove_child;
	gc_feature_node_class->get_value_type = arv_gc_struct_entry_node_get_value_type;
	gc_feature_node_class->set_value_from_string = arv_gc_struct_entry_node_set_value_from_string;
	gc_feature_node_class->get_value_as_string = arv_gc_struct_entry_node_get_value_as_string;
}

/* ArvGcInteger interface implementation */

static gint64
arv_gc_struct_entry_node_get_integer_value (ArvGcInteger *gc_integer)
{
	ArvGcStructEntryNode *struct_entry = ARV_GC_STRUCT_ENTRY_NODE (gc_integer);
	ArvDomNode *struct_register;
	guint lsb;
	guint msb;

	struct_register = arv_dom_node_get_parent_node (ARV_DOM_NODE (gc_integer));
	if (!ARV_IS_GC_REGISTER (struct_register))
		return 0;

	lsb = _get_lsb (struct_entry);
	msb = _get_msb (struct_entry);

	return arv_gc_register_get_masked_integer_value (ARV_GC_REGISTER (struct_register), lsb, msb);
}

static void
arv_gc_struct_entry_node_set_integer_value (ArvGcInteger *gc_integer, gint64 value)
{
	ArvGcStructEntryNode *struct_entry = ARV_GC_STRUCT_ENTRY_NODE (gc_integer);
	ArvDomNode *struct_register;
	guint lsb;
	guint msb;

	struct_register = arv_dom_node_get_parent_node (ARV_DOM_NODE (gc_integer));
	if (!ARV_IS_GC_REGISTER (struct_register))
		return;

	lsb = _get_lsb (struct_entry);
	msb = _get_msb (struct_entry);

	arv_gc_register_set_masked_integer_value (ARV_GC_REGISTER (struct_register), lsb, msb, value);
}

static void
arv_gc_struct_entry_node_integer_interface_init (ArvGcIntegerInterface *interface)
{
	interface->get_value = arv_gc_struct_entry_node_get_integer_value;
	interface->set_value = arv_gc_struct_entry_node_set_integer_value;
}

G_DEFINE_TYPE_WITH_CODE (ArvGcStructEntryNode, arv_gc_struct_entry_node, ARV_TYPE_GC_FEATURE_NODE,
			 G_IMPLEMENT_INTERFACE (ARV_TYPE_GC_INTEGER, arv_gc_struct_entry_node_integer_interface_init))
