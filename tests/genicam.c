#include <glib.h>
#include <arv.h>
#include <string.h>

typedef struct {
	const char *name;
	GType value_type;
} NodeTypes;

NodeTypes node_value_types[] = {
	{"RWFloat",			G_TYPE_DOUBLE},
	{"P_RWFloat_Min",		G_TYPE_DOUBLE},
	{"P_RWFloat_Max",		G_TYPE_DOUBLE},
	{"P_RWFloat_Inc",		G_TYPE_DOUBLE},
	{"P_RWFloat",			G_TYPE_DOUBLE},
	{"RWBoolean",			G_TYPE_BOOLEAN},
	{"P_RWBoolean",			G_TYPE_BOOLEAN},
	{"RWInteger",			G_TYPE_INT64},
	{"P_RWInteger",			G_TYPE_INT64},
	{"P_RWInteger_Min",		G_TYPE_INT64},
	{"P_RWInteger_Max",		G_TYPE_INT64},
	{"P_RWInteger_Inc",		G_TYPE_INT64},
	{"Enumeration",			G_TYPE_STRING},
	{"EnumerationValue",		G_TYPE_INT64},
	{"IntRegisterA",		G_TYPE_INT64},
	{"IntRegisterB",		G_TYPE_INT64},
	{"IntSwissKnifeTest",		G_TYPE_INT64},
	{"DeviceUserID",		G_TYPE_STRING}
};

static void
node_value_type_test (void)
{
	ArvDevice *device;
	ArvGc *genicam;
	ArvGcNode *node;
	int i;

	device = arv_fake_device_new ("TEST0");
	g_assert (ARV_IS_FAKE_DEVICE (device));

	genicam = arv_device_get_genicam (device); 
	g_assert (ARV_IS_GC (genicam));

	for (i = 0; i < G_N_ELEMENTS (node_value_types); i++) {
		node = arv_gc_get_node (genicam, node_value_types[i].name);
		g_assert (arv_gc_feature_node_get_value_type (ARV_GC_FEATURE_NODE (node)) == node_value_types[i].value_type);
	}

	g_object_unref (device);
}

static void
integer_test (void)
{
	ArvDevice *device;
	ArvGc *genicam;
	ArvGcNode *node;
	gint64 v_int64;
	const char *v_string;

	device = arv_fake_device_new ("TEST0");
	g_assert (ARV_IS_FAKE_DEVICE (device));

	genicam = arv_device_get_genicam (device);
	g_assert (ARV_IS_GC (genicam));

	node = arv_gc_get_node (genicam, "RWInteger");
	g_assert (ARV_IS_GC_INTEGER_NODE (node));

	v_int64 = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 1);

	v_int64 = arv_gc_integer_get_min (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, -10);

	v_int64 = arv_gc_integer_get_max (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 10);

	v_int64 = arv_gc_integer_get_inc (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 2);

	v_string = arv_gc_feature_node_get_value_as_string (ARV_GC_FEATURE_NODE (node), NULL);
	g_assert_cmpstr (v_string, ==, "1");

	arv_gc_integer_set_value (ARV_GC_INTEGER (node), 2, NULL);
	v_int64 = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 2);

	node = arv_gc_get_node (genicam, "P_RWInteger");
	g_assert (ARV_IS_GC_INTEGER_NODE (node));

	v_int64 = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 2);

	v_int64 = arv_gc_integer_get_min (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, -20);

	v_int64 = arv_gc_integer_get_max (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 20);

	v_int64 = arv_gc_integer_get_inc (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 3);

	g_object_unref (device);
}

static void
boolean_test (void)
{
	ArvDevice *device;
	ArvGc *genicam;
	ArvGcNode *node;
	ArvGcNode *node_b;
	gboolean v_boolean;
	const char *v_string;
	gint64 v_int64;

	device = arv_fake_device_new ("TEST0");
	g_assert (ARV_IS_FAKE_DEVICE (device));

	genicam = arv_device_get_genicam (device);
	g_assert (ARV_IS_GC (genicam));

	node = arv_gc_get_node (genicam, "RWBoolean");
	g_assert (ARV_IS_GC_BOOLEAN (node));
	g_assert (ARV_IS_GC_INTEGER (node));

	v_boolean = arv_gc_boolean_get_value (ARV_GC_BOOLEAN (node), NULL);
	g_assert_cmpint (v_boolean, ==, TRUE);

	v_int64 = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 1);

	v_string = arv_gc_feature_node_get_value_as_string (ARV_GC_FEATURE_NODE (node), NULL);
	g_assert_cmpstr (v_string, ==, "true");

	arv_gc_boolean_set_value (ARV_GC_BOOLEAN (node), 0, NULL);
	v_boolean = arv_gc_boolean_get_value (ARV_GC_BOOLEAN (node), NULL);
	g_assert_cmpint (v_boolean, ==, FALSE);

	v_int64 = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 0);

	v_string = arv_gc_feature_node_get_value_as_string (ARV_GC_FEATURE_NODE (node), NULL);
	g_assert_cmpstr (v_string, ==, "false");

	node = arv_gc_get_node (genicam, "P_RWBoolean");
	g_assert (ARV_IS_GC_BOOLEAN (node));

	v_boolean = arv_gc_boolean_get_value (ARV_GC_BOOLEAN (node), NULL);
	g_assert_cmpint (v_boolean, ==, TRUE);

	node_b = arv_gc_get_node (genicam, "RWBooleanValue");
	g_assert (ARV_IS_GC_INTEGER (node_b));

	arv_gc_integer_set_value (ARV_GC_INTEGER (node_b), 42, NULL);

	v_boolean = arv_gc_boolean_get_value (ARV_GC_BOOLEAN (node), NULL);
	g_assert_cmpint (v_boolean, ==, FALSE);

	g_object_unref (device);
}

static void
float_test (void)
{
	ArvDevice *device;
	ArvGc *genicam;
	ArvGcNode *node;
	double v_double;
	const char *v_string;

	device = arv_fake_device_new ("TEST0");
	g_assert (ARV_IS_FAKE_DEVICE (device));

	genicam = arv_device_get_genicam (device);
	g_assert (ARV_IS_GC (genicam));

	node = arv_gc_get_node (genicam, "RWFloat");
	g_assert (ARV_IS_GC_FLOAT_NODE (node));

	v_double = arv_gc_float_get_value (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, 0.1);

	v_double = arv_gc_float_get_min (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, -10.0);

	v_double = arv_gc_float_get_max (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, 10.0);

	v_double = arv_gc_float_get_inc (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, 2.0);

	v_string = arv_gc_feature_node_get_value_as_string (ARV_GC_FEATURE_NODE (node), NULL);
	g_assert_cmpstr (v_string, ==, "0.1");

	arv_gc_float_set_value (ARV_GC_FLOAT (node), 0.2, NULL);
	v_double = arv_gc_float_get_value (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, 0.2);

	node = arv_gc_get_node (genicam, "P_RWFloat");
	g_assert (ARV_IS_GC_FLOAT_NODE (node));

	v_double = arv_gc_float_get_value (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, 0.2);

	v_double = arv_gc_float_get_min (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, -20.0);

	v_double = arv_gc_float_get_max (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, 20.0);

	v_double = arv_gc_float_get_inc (ARV_GC_FLOAT (node), NULL);
	g_assert_cmpfloat (v_double, ==, 3.0);

	g_object_unref (device);
}

static void
enumeration_test (void)
{
	ArvDevice *device;
	ArvGc *genicam;
	ArvGcNode *node;
	gint64 v_int64;
	gint64 *values;
	guint n_values;
	const char *v_string;

	device = arv_fake_device_new ("TEST0");
	g_assert (ARV_IS_FAKE_DEVICE (device));

	genicam = arv_device_get_genicam (device);
	g_assert (ARV_IS_GC (genicam));

	node = arv_gc_get_node (genicam, "Enumeration");
	g_assert (ARV_IS_GC_ENUMERATION (node));
	g_assert (ARV_IS_GC_INTEGER (node));
	g_assert (ARV_IS_GC_STRING (node));

	v_int64 = arv_gc_enumeration_get_int_value (ARV_GC_ENUMERATION (node), NULL);
	g_assert_cmpint (v_int64, ==, 0);

	values = arv_gc_enumeration_get_available_int_values (ARV_GC_ENUMERATION (node), &n_values, NULL);
	g_assert_cmpint (n_values, ==, 2);
	g_assert (values != NULL);
	g_free (values);

	arv_gc_enumeration_set_string_value (ARV_GC_ENUMERATION (node), "Entry1", NULL);

	v_int64 = arv_gc_enumeration_get_int_value (ARV_GC_ENUMERATION (node), NULL);
	g_assert_cmpint (v_int64, ==, 1);

	v_int64 = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 1);

	v_string = arv_gc_string_get_value (ARV_GC_STRING (node), NULL);
	g_assert_cmpstr (v_string, ==, "Entry1");

	arv_gc_string_set_value (ARV_GC_STRING (node), "Entry0", NULL);

	v_int64 = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (v_int64, ==, 0);

	v_string = arv_gc_string_get_value (ARV_GC_STRING (node), NULL);
	g_assert_cmpstr (v_string, ==, "Entry0");

	v_int64 = arv_gc_string_get_max_length (ARV_GC_STRING (node), NULL);
	g_assert_cmpint (v_int64, ==, strlen ("EntryNotImplemented"));

	g_object_unref (device);
}

static void
swiss_knife_test (void)
{
	ArvDevice *device;
	ArvGc *genicam;
	ArvGcNode *node;
	gint64 value;

	device = arv_fake_device_new ("TEST0");
	g_assert (ARV_IS_FAKE_DEVICE (device));

	genicam = arv_device_get_genicam (device);
	g_assert (ARV_IS_GC (genicam));

	node = arv_gc_get_node (genicam, "IntSwissKnifeTest");
	g_assert (ARV_IS_GC_SWISS_KNIFE (node));

	value = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (value, ==, 0x1234);

	node = arv_gc_get_node (genicam, "IntSwissKnifeTestEntity");
	g_assert (ARV_IS_GC_SWISS_KNIFE (node));

	value = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (value, ==, 0x10305070);

	node = arv_gc_get_node (genicam, "IntSwissKnifeTestEntity2");
	g_assert (ARV_IS_GC_SWISS_KNIFE (node));

	value = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (value, ==, 3);

	node = arv_gc_get_node (genicam, "IntSwissKnifeBug699228");
	g_assert (ARV_IS_GC_SWISS_KNIFE (node));

	value = arv_gc_integer_get_value (ARV_GC_INTEGER (node), NULL);
	g_assert_cmpint (value, ==, 4);

	g_object_unref (device);
}

static void
register_test (void)
{
	ArvDevice *device;
	ArvGc *genicam;
	ArvGcNode *node;
	gint64 value;

	device = arv_fake_device_new ("TEST0");
	g_assert (ARV_IS_FAKE_DEVICE (device));

	genicam = arv_device_get_genicam (device);
	g_assert (ARV_IS_GC (genicam));

	node = arv_gc_get_node (genicam, "IntRegisterA");
	g_assert (ARV_IS_GC_REGISTER (node));

	value = arv_gc_register_get_address (ARV_GC_REGISTER (node), NULL);
	g_assert_cmpint (value, ==, 0x1050);

	node = arv_gc_get_node (genicam, "IntRegisterB");
	g_assert (ARV_IS_GC_REGISTER (node));

	value = arv_gc_register_get_address (ARV_GC_REGISTER (node), NULL);
	g_assert_cmpint (value, ==, 0x20ff);

	g_object_unref (device);
}

GRegex *arv_gv_device_get_url_regex (void);

static void
url_test (void)
{
	char **tokens;
	unsigned int i;

	tokens = g_regex_split (arv_gv_device_get_url_regex (), "Local:Basler_Ace_GigE_e7c9b87e_Version_3_3.zip;c0000000;10cca", 0);

	g_assert_cmpint (g_strv_length (tokens), ==, 6);

	g_assert_cmpstr (tokens[0], ==, "");
	g_assert_cmpstr (tokens[1], ==, "Local:");
	g_assert_cmpstr (tokens[2], ==, "Basler_Ace_GigE_e7c9b87e_Version_3_3.zip");
	g_assert_cmpstr (tokens[3], ==, "c0000000");
	g_assert_cmpstr (tokens[4], ==, "10cca");

	g_strfreev (tokens);

	tokens = g_regex_split (arv_gv_device_get_url_regex (), "Local:C4_2040_GigE_1.0.0.zip;0x8C400904;0x4D30", 0);

	g_assert_cmpint (g_strv_length (tokens), ==, 6);

	g_assert_cmpstr (tokens[0], ==, "");
	g_assert_cmpstr (tokens[1], ==, "Local:");
	g_assert_cmpstr (tokens[2], ==, "C4_2040_GigE_1.0.0.zip");
	g_assert_cmpstr (tokens[3], ==, "8C400904");
	g_assert_cmpstr (tokens[4], ==, "4D30");

	g_strfreev (tokens);

}

static void
mandatory_test (void)
{
	ArvDevice *device;
	ArvGc *genicam;
	ArvGcNode *node;
	gint64 value;

	device = arv_fake_device_new ("TEST0");
	g_assert (ARV_IS_FAKE_DEVICE (device));

	genicam = arv_device_get_genicam (device);
	g_assert (ARV_IS_GC (genicam));

	node = arv_gc_get_node (genicam, "TLParamsLocked");
	g_assert (ARV_IS_GC_INTEGER (node));

	g_object_unref (device);
}

int
main (int argc, char *argv[])
{
	int result;
	int i;

	g_test_init (&argc, &argv, NULL);

	arv_g_type_init ();

	arv_set_fake_camera_genicam_filename (GENICAM_FILENAME);

	g_test_add_func ("/genicam/value_type", node_value_type_test);
	g_test_add_func ("/genicam/integer", integer_test);
	g_test_add_func ("/genicam/boolean", boolean_test);
	g_test_add_func ("/genicam/float", float_test);
	g_test_add_func ("/genicam/enumeration", enumeration_test);
	g_test_add_func ("/genicam/swissknife", swiss_knife_test);
	g_test_add_func ("/genicam/register", register_test);
	g_test_add_func ("/genicam/url", url_test);
	g_test_add_func ("/genicam/mandatory", mandatory_test);

	result = g_test_run();

	arv_shutdown ();

	return result;
}

