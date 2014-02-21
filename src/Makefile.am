ARIB25_SRCS = arib_std_b25.c b_cas_card.c multi2.c td.c ts_section_parser.c
ARIB25_CPPFLAGS = -Wall -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 $(PCSCLITE_CFLAGS)
ARIB25_LDFLAGS = $(PCSCLITE_LDFLAGS)
ARIB25_CFLAGS = -O2 -g -fPIC

if ARIB25_SHARED
lib_LTLIBRARIES = libarib25.la
libarib25_la_LDFLAGS = $(ARIB25_LDFLAGS)
else
noinst_LTLIBRARIES = libarib25.la
libarib25_la_LDFLAGS = $(ARIB25_LDFLAGS) -static
endif

libarib25_la_SOURCES = $(ARIB25_SRCS)
libarib25_la_CFLAGS = $(ARIB25_CFLAGS)
libarib25_la_CPPFLAGS = $(ARIB25_CPPFLAGS)
