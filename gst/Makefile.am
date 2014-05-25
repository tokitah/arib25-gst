plugindir = `pkg-config --variable=pluginsdir gstreamer-1.0`

plugin_LTLIBRARIES = libarib25specverifier.la

# sources used to compile this plug-in
libarib25specverifier_la_SOURCES = gstb25specverifier.c gstb25specverifier.h

# compiler and linker flags used to compile this plugin, set in configure.ac
libarib25specverifier_la_CFLAGS = $(GST_CFLAGS)
libarib25specverifier_la_LIBADD = $(GST_LIBS) -larib25
libarib25specverifier_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libarib25specverifier_la_LIBTOOLFLAGS = $(GST_PLUGIN_LIBTOOLFLAGS)

# headers we need but don't want installed
noinst_HEADERS = gstb25specverifier.h
