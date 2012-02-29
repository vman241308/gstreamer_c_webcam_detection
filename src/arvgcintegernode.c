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
 * SECTION: arvgcintegernode
 * @short_description: Class for Integer nodes
 */

#include <arvgcintegernode.h>
#include <arvgcinteger.h>
#include <arvgc.h>
#include <arvmisc.h>
#include <string.h>
#include <stdlib.h>

static GObjectClass *parent_class = NULL;

/* ArvDomNode implementation */

static const char *
arv_gc_integer_node_get_node_name (ArvDomNode *node)
{
	return "Integer";
}

static void
arv_gc_integer_node_post_new_child (ArvDomNode *self, ArvDomNode *child)
{
	ArvGcIntegerNode *node = ARV_GC_INTEGER_NODE (self);

	if (ARV_IS_GC_PROPERTY_NODE (child)) {
		ArvGcPropertyNode *property_node = ARV_GC_PROPERTY_NODE (child);

		switch (arv_gc_property_node_get_node_type (property_node)) {
			case ARV_GC_PROPERTY_NODE_TYPE_VALUE:
			case ARV_GC_PROPERTY_NODE_TYPE_P_VALUE:
				node->value = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_MINIMUM:
			case ARV_GC_PROPERTY_NODE_TYPE_P_MINIMUM:
				node->maximum = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_MAXIMUM:
			case ARV_GC_PROPERTY_NODE_TYPE_P_MAXIMUM:
				node->minimum = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_INCREMENT:
			case ARV_GC_PROPERTY_NODE_TYPE_P_INCREMENT:
				node->increment = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_UNIT:
				node->unit = property_node;
				break;
			default:
				ARV_DOM_NODE_CLASS (parent_class)->post_new_child (self, child);
				break;
		}
	}
}

static void
arv_gc_integer_node_pre_remove_child (ArvDomNode *self, ArvDomNode *child)
{
	g_assert_not_reached ();
}

/* ArvGcFeatureNode implementation */

static GType
arv_gc_integer_node_get_value_type (ArvGcFeatureNode *node)
{
	return G_TYPE_INT64;
}

static void
arv_gc_integer_node_set_value_from_string (ArvGcFeatureNode *node, const char *string)
{
	arv_gc_integer_set_value (ARV_GC_INTEGER (node), g_ascii_strtoll (string, NULL, 0));
}

static const char *
arv_gc_integer_node_get_value_as_string (ArvGcFeatureNode *node)
{
	ArvGcIntegerNode *integer_node = ARV_GC_INTEGER_NODE (node);

	if (integer_node->value != NULL)
		return arv_gc_property_node_get_string (ARV_GC_PROPERTY_NODE (integer_node->value));

	return NULL;
}

/* ArvGcIntegerNode implementation */

ArvGcNode *
arv_gc_integer_node_new (void)
{
	ArvGcNode *node;

	node = g_object_new (ARV_TYPE_GC_INTEGER_NODE, NULL);

	return node;
}

static void
arv_gc_integer_node_init (ArvGcIntegerNode *gc_integer_node)
{
}

static void
arv_gc_integer_node_finalize (GObject *object)
{
	parent_class->finalize (object);
}

static void
arv_gc_integer_node_class_init (ArvGcIntegerNodeClass *this_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (this_class);
	ArvDomNodeClass *dom_node_class = ARV_DOM_NODE_CLASS (this_class);
	ArvGcFeatureNodeClass *gc_feature_node_class = ARV_GC_FEATURE_NODE_CLASS (this_class);

	parent_class = g_type_class_peek_parent (this_class);

	object_class->finalize = arv_gc_integer_node_finalize;
	dom_node_class->get_node_name = arv_gc_integer_node_get_node_name;
	dom_node_class->post_new_child = arv_gc_integer_node_post_new_child;
	dom_node_class->pre_remove_child = arv_gc_integer_node_pre_remove_child;
	gc_feature_node_class->get_value_type = arv_gc_integer_node_get_value_type;
	gc_feature_node_class->set_value_from_string = arv_gc_integer_node_set_value_from_string;
	gc_feature_node_class->get_value_as_string = arv_gc_integer_node_get_value_as_string;
}

/* ArvGcInteger interface implementation */

static gint64
arv_gc_integer_node_get_integer_value (ArvGcInteger *gc_integer)
{
	ArvGcIntegerNode *gc_integer_node = ARV_GC_INTEGER_NODE (gc_integer);

	if (gc_integer_node->value != NULL)
		return arv_gc_property_node_get_int64 (ARV_GC_PROPERTY_NODE (gc_integer_node->value));

	return 0;
}

static void
arv_gc_integer_node_set_integer_value (ArvGcInteger *gc_integer, gint64 value)
{
	ArvGcIntegerNode *gc_integer_node = ARV_GC_INTEGER_NODE (gc_integer);

	if (gc_integer_node->value != NULL)
		arv_gc_property_node_set_int64 (ARV_GC_PROPERTY_NODE (gc_integer_node->value), value);
}

static gint64
arv_gc_integer_node_get_min (ArvGcInteger *gc_integer)
{
	ArvGcIntegerNode *gc_integer_node = ARV_GC_INTEGER_NODE (gc_integer);

	if (gc_integer_node->minimum != NULL)
		return arv_gc_property_node_get_int64 (ARV_GC_PROPERTY_NODE (gc_integer_node->minimum));

	return G_MININT64;
}

static gint64
arv_gc_integer_node_get_max (ArvGcInteger *gc_integer)
{
	ArvGcIntegerNode *gc_integer_node = ARV_GC_INTEGER_NODE (gc_integer);

	if (gc_integer_node->maximum != NULL)
		return arv_gc_property_node_get_int64 (ARV_GC_PROPERTY_NODE (gc_integer_node->maximum));

	return G_MAXINT64;
}

static gint64
arv_gc_integer_node_get_inc (ArvGcInteger *gc_integer)
{
	ArvGcIntegerNode *gc_integer_node = ARV_GC_INTEGER_NODE (gc_integer);

	if (gc_integer_node->increment != NULL)
		return arv_gc_property_node_get_int64 (ARV_GC_PROPERTY_NODE (gc_integer_node->increment));

	return 1;
}

static const char *
arv_gc_integer_node_get_unit (ArvGcInteger *gc_integer)
{
	ArvGcIntegerNode *gc_integer_node = ARV_GC_INTEGER_NODE (gc_integer);

	if (gc_integer_node->unit != NULL)
		return arv_gc_property_node_get_string (ARV_GC_PROPERTY_NODE (gc_integer_node->unit));

	return NULL;
}

static void
arv_gc_integer_node_impose_min (ArvGcInteger *gc_integer, gint64 minimum)
{
	ArvGcIntegerNode *gc_integer_node = ARV_GC_INTEGER_NODE (gc_integer);

	if (gc_integer_node->minimum != NULL)
		arv_gc_property_node_set_int64 (ARV_GC_PROPERTY_NODE (gc_integer_node->minimum), minimum);
}

static void
arv_gc_integer_node_impose_max (ArvGcInteger *gc_integer, gint64 maximum)
{
	ArvGcIntegerNode *gc_integer_node = ARV_GC_INTEGER_NODE (gc_integer);

	if (gc_integer_node->maximum != NULL)
		arv_gc_property_node_set_int64 (ARV_GC_PROPERTY_NODE (gc_integer_node->maximum), maximum);
}

static void
arv_gc_integer_node_integer_interface_init (ArvGcIntegerInterface *interface)
{
	interface->get_value = arv_gc_integer_node_get_integer_value;
	interface->set_value = arv_gc_integer_node_set_integer_value;
	interface->get_min = arv_gc_integer_node_get_min;
	interface->get_max = arv_gc_integer_node_get_max;
	interface->get_inc = arv_gc_integer_node_get_inc;
	interface->get_unit = arv_gc_integer_node_get_unit;
	interface->impose_min = arv_gc_integer_node_impose_min;
	interface->impose_max = arv_gc_integer_node_impose_max;
}

G_DEFINE_TYPE_WITH_CODE (ArvGcIntegerNode, arv_gc_integer_node, ARV_TYPE_GC_FEATURE_NODE,
			 G_IMPLEMENT_INTERFACE (ARV_TYPE_GC_INTEGER, arv_gc_integer_node_integer_interface_init))
