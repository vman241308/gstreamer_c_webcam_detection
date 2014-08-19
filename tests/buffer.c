#include <glib.h>
#include <arv.h>

static void
simple_buffer_test (void)
{
	ArvBuffer *buffer;
	size_t size;
	const void *data;

	buffer = arv_buffer_new (1024, NULL);

	g_assert (ARV_IS_BUFFER (buffer));

	data = arv_buffer_get_data (buffer, &size);

	g_assert (data != NULL);
	g_assert (size == 1024);

	g_assert (arv_buffer_get_status (buffer) == ARV_BUFFER_STATUS_CLEARED);

	g_object_unref (buffer);
}

static void
preallocated_buffer_test (void)
{
	ArvBuffer *buffer;
	void *data = g_malloc (1024);

	buffer = arv_buffer_new (1024, data);
	g_assert (ARV_IS_BUFFER (buffer));

	g_assert (arv_buffer_get_data (buffer, NULL) == data);

	g_assert (arv_buffer_get_status (buffer) == ARV_BUFFER_STATUS_CLEARED);

	g_object_unref (buffer);
}

static void
full_buffer_destroy_func (void *data)
{
	*((int *) data) = 4321;
}

static void
full_buffer_test (void)
{
	ArvBuffer *buffer;
	int value = 1234;
	const void *data;
	size_t size;

	buffer = arv_buffer_new_full (1024, NULL, &value, full_buffer_destroy_func);

	g_assert (ARV_IS_BUFFER (buffer));

	data = arv_buffer_get_data (buffer, &size);

	g_assert (data != NULL);
	g_assert (size == 1024);

	g_assert (arv_buffer_get_status (buffer) == ARV_BUFFER_STATUS_CLEARED);

	g_object_unref (buffer);

	g_assert (value == 4321);
}

int
main (int argc, char *argv[])
{
	int result;

	g_test_init (&argc, &argv, NULL);

	arv_g_type_init ();

	g_test_add_func ("/buffer/simple-buffer", simple_buffer_test);
	g_test_add_func ("/buffer/preallocated-buffer", preallocated_buffer_test);
	g_test_add_func ("/buffer/full-buffer", full_buffer_test);

	result = g_test_run();

	arv_shutdown ();

	return result;
}

