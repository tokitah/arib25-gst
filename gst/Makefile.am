plugindir = `pkg-config --variable=pluginsdir gstreamer-1.0`

plugin_LTLIBRARIES = libarib25gst.la

# sources used to compile this plug-in
libarib25gst_la_SOURCES = gstb25specverifier.c gstb25specverifier.h

# compiler and linker flags used to compile this plugin, set in configure.ac
libarib25gst_la_CFLAGS = $(GST_CFLAGS) -I$(top_srcdir)/src
libarib25gst_la_LIBADD = $(GST_LIBS) $(top_srcdir)/src/libarib25.la
libarib25gst_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS) -L$(top_srcdir)/src/.libs/
libarib25gst_la_LIBTOOLFLAGS = $(GST_PLUGIN_LIBTOOLFLAGS)

# headers we need but don't want installed
noinst_HEADERS = gstb25specverifier.h

FORCE:

$(top_srcdir)/src/libarib25.la: FORCE
	( cd $(top_srcdir)/src && $(MAKE) $(AM_MAKEFLAGS) libarib25.la )
