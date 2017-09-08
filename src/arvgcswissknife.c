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

/**
 * SECTION: arvgcswissknife
 * @short_description: Class for SwissKnife and IntSwissKnife nodes
 */

#include <arvgcswissknife.h>
#include <arvevaluator.h>
#include <arvgcinteger.h>
#include <arvgcfloat.h>
#include <arvgcport.h>
#include <arvgc.h>
#include <arvdebug.h>
#include <string.h>

static GObjectClass *parent_class = NULL;

/* ArvDomNode implementation */

static const char *
arv_gc_swiss_knife_get_node_name (ArvDomNode *node)
{
	ArvGcSwissKnife *gc_swiss_knife = ARV_GC_SWISS_KNIFE (node);

	if (gc_swiss_knife->value_type == G_TYPE_DOUBLE)
		return "SwissKnife";

	return "IntSwissKnife";
}

static void
arv_gc_swiss_knife_post_new_child (ArvDomNode *self, ArvDomNode *child)
{
	ArvGcSwissKnife *node = ARV_GC_SWISS_KNIFE (self);

	if (ARV_IS_GC_PROPERTY_NODE (child)) {
		ArvGcPropertyNode *property_node = ARV_GC_PROPERTY_NODE (child);

		switch (arv_gc_property_node_get_node_type (property_node)) {
			case ARV_GC_PROPERTY_NODE_TYPE_P_VARIABLE:
				node->variables = g_slist_prepend (node->variables, property_node);
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_FORMULA:
				node->formula_node = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_EXPRESSION:
				node->expressions = g_slist_prepend (node->expressions, property_node);
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_CONSTANT:
				node->constants = g_slist_prepend (node->constants, property_node);
				break;
			default:
				ARV_DOM_NODE_CLASS (parent_class)->post_new_child (self, child);
				break;
		}
	}
}

static void
arv_gc_swiss_knife_pre_remove_child (ArvDomNode *self, ArvDomNode *child)
{
	g_assert_not_reached ();
}

/* ArvGcFeatureNode implementation */

static GType
arv_gc_swiss_knife_node_get_value_type (ArvGcFeatureNode *node)
{
	ArvGcSwissKnife *gc_swiss_knife = ARV_GC_SWISS_KNIFE (node);

	return gc_swiss_knife->value_type;
}

/* ArvGcSwissKnife implementation */

ArvGcNode *
arv_gc_swiss_knife_new (void)
{
	ArvGcSwissKnife *swiss_knife;

	swiss_knife = g_object_new (ARV_TYPE_GC_SWISS_KNIFE, NULL);
	swiss_knife->value_type = G_TYPE_DOUBLE;

	return ARV_GC_NODE (swiss_knife);
}

ArvGcNode *
arv_gc_swiss_knife_new_integer (void)
{
	ArvGcSwissKnife *swiss_knife;

	swiss_knife = g_object_new (ARV_TYPE_GC_SWISS_KNIFE, NULL);
	swiss_knife->value_type = G_TYPE_INT64;

	return ARV_GC_NODE (swiss_knife);
}

static void
arv_gc_swiss_knife_init (ArvGcSwissKnife *gc_swiss_knife)
{
	gc_swiss_knife->formula = arv_evaluator_new (NULL);
}

static void
arv_gc_swiss_knife_finalize (GObject *object)
{
	ArvGcSwissKnife *gc_swiss_knife = ARV_GC_SWISS_KNIFE (object);

	g_slist_free (gc_swiss_knife->variables);
	g_slist_free (gc_swiss_knife->expressions);
	g_slist_free (gc_swiss_knife->constants);

	g_clear_object (&gc_swiss_knife->formula);

	parent_class->finalize (object);
}

static void
arv_gc_swiss_knife_class_init (ArvGcSwissKnifeClass *this_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (this_class);
	ArvDomNodeClass *dom_node_class = ARV_DOM_NODE_CLASS (this_class);
	ArvGcFeatureNodeClass *gc_feature_node_class = ARV_GC_FEATURE_NODE_CLASS (this_class);

	parent_class = g_type_class_peek_parent (this_class);

	object_class->finalize = arv_gc_swiss_knife_finalize;
	dom_node_class->get_node_name = arv_gc_swiss_knife_get_node_name;
	dom_node_class->post_new_child = arv_gc_swiss_knife_post_new_child;
	dom_node_class->pre_remove_child = arv_gc_swiss_knife_pre_remove_child;
	gc_feature_node_class->get_value_type = arv_gc_swiss_knife_node_get_value_type;
}

/* ArvGcInteger interface implementation */

static void
_update_variables (ArvGcSwissKnife *gc_swiss_knife, GError **error)
{
	ArvGcNode *node;
	GError *local_error = NULL;
	GSList *iter;
	const char *expression;

	if (gc_swiss_knife->formula_node != NULL)
		expression = arv_gc_property_node_get_string (gc_swiss_knife->formula_node, &local_error);
	else
		expression = "";

	if (local_error != NULL) {
		g_propagate_error (error, local_error);
		return;
	}

	arv_evaluator_set_expression (gc_swiss_knife->formula, expression);

	for (iter = gc_swiss_knife->expressions; iter != NULL; iter = iter->next) {
		const char *expression;
		const char *name;

		expression = arv_gc_property_node_get_string (ARV_GC_PROPERTY_NODE (iter->data), &local_error);
		if (local_error != NULL) {
			g_propagate_error (error, local_error);
			return;
		}

		name = arv_gc_property_node_get_name (iter->data);

		arv_evaluator_set_sub_expression (gc_swiss_knife->formula, name, expression);
	}

	for (iter = gc_swiss_knife->constants; iter != NULL; iter = iter->next) {
		const char *constant;
		const char *name;

		constant = arv_gc_property_node_get_string (ARV_GC_PROPERTY_NODE (iter->data), &local_error);
		if (local_error != NULL) {
			g_propagate_error (error, local_error);
			return;
		}

		name = arv_gc_property_node_get_name (iter->data);

		arv_evaluator_set_constant (gc_swiss_knife->formula, name, constant);
	}

	for (iter = gc_swiss_knife->variables; iter != NULL; iter = iter->next) {
		ArvGcPropertyNode *variable_node = iter->data;

		node = arv_gc_property_node_get_linked_node (ARV_GC_PROPERTY_NODE (variable_node));
		if (arv_gc_feature_node_get_value_type (ARV_GC_FEATURE_NODE (node)) == G_TYPE_INT64) {
			gint64 value;

			value = arv_gc_integer_get_value (ARV_GC_INTEGER (node), &local_error);

			if (local_error != NULL) {
				g_propagate_error (error, local_error);
				return;
			}

			arv_evaluator_set_int64_variable (gc_swiss_knife->formula,
							  arv_gc_property_node_get_name (variable_node),
							  value);
		} else if (arv_gc_feature_node_get_value_type (ARV_GC_FEATURE_NODE (node)) == G_TYPE_DOUBLE) {
			double value;

			value = arv_gc_float_get_value (ARV_GC_FLOAT (node), &local_error);

			if (local_error != NULL) {
				g_propagate_error (error, local_error);
				return;
			}

			arv_evaluator_set_double_variable (gc_swiss_knife->formula,
							  arv_gc_property_node_get_name (variable_node),
							  value);
		}
	}

}

static gint64
arv_gc_swiss_knife_get_integer_value (ArvGcInteger *gc_integer, GError **error)
{
	ArvGcSwissKnife *gc_swiss_knife = ARV_GC_SWISS_KNIFE (gc_integer);
	GError *local_error = NULL;

	_update_variables (gc_swiss_knife, &local_error);

	if (local_error != NULL) {
		g_propagate_error (error, local_error);
		return 0;
	}

	return arv_evaluator_evaluate_as_int64 (gc_swiss_knife->formula, NULL);
}

static void
arv_gc_swiss_knife_set_integer_value (ArvGcInteger *gc_integer, gint64 value, GError **error)
{
}

static void
arv_gc_swiss_knife_integer_interface_init (ArvGcIntegerInterface *interface)
{
	interface->get_value = arv_gc_swiss_knife_get_integer_value;
	interface->set_value = arv_gc_swiss_knife_set_integer_value;
}

static double
arv_gc_swiss_knife_get_float_value (ArvGcFloat *gc_float, GError **error)
{
	ArvGcSwissKnife *gc_swiss_knife = ARV_GC_SWISS_KNIFE (gc_float);
	GError *local_error = NULL;

	_update_variables (gc_swiss_knife, &local_error);

	if (local_error != NULL) {
		g_propagate_error (error, local_error);
		return 0.0;
	}

	return arv_evaluator_evaluate_as_double (gc_swiss_knife->formula, NULL);
}

static void
arv_gc_swiss_knife_set_float_value (ArvGcFloat *gc_float, double value, GError **error)
{
}

static void
arv_gc_swiss_knife_float_interface_init (ArvGcFloatInterface *interface)
{
	interface->get_value = arv_gc_swiss_knife_get_float_value;
	interface->set_value = arv_gc_swiss_knife_set_float_value;
}

G_DEFINE_TYPE_WITH_CODE (ArvGcSwissKnife, arv_gc_swiss_knife, ARV_TYPE_GC_FEATURE_NODE,
			 G_IMPLEMENT_INTERFACE (ARV_TYPE_GC_INTEGER, arv_gc_swiss_knife_integer_interface_init)
			 G_IMPLEMENT_INTERFACE (ARV_TYPE_GC_FLOAT,   arv_gc_swiss_knife_float_interface_init))
