
/**
 * SECTION:element-b25specverifier
 *
 * Specification verifier plugin for ARIB-STD-B25 MPEG-TS stream.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch filesrc location=b25scrambled.m2ts ! b25specverifier ! filesink location=./b25specverifierd.m2ts
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>

#include <string.h>
#include "gstb25specverifier.h"

#define MULTI2_ROUND_DEFAULT_VALUE 4

GST_DEBUG_CATEGORY_STATIC (gst_b25specverifier_debug);
#define GST_CAT_DEFAULT gst_b25specverifier_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_ROUND,
  PROP_STRIP,
  PROP_EMM,
  PROP_KEEPGOING,
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

#define gst_b25specverifier_parent_class parent_class
G_DEFINE_TYPE (GstB25SpecVerifier, gst_b25specverifier, GST_TYPE_ELEMENT);
//GST_BOILERPLATE (GstB25SpecVerifier, gst_b25specverifier, GstElement,
//    GST_TYPE_ELEMENT);

static void gst_b25specverifier_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_b25specverifier_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_b25specverifier_sink_event (GstPad * pad, GstObject * parent, GstEvent * event);
static GstFlowReturn gst_b25specverifier_chain (GstPad * pad, GstObject * parent, GstBuffer * buf);
static GstFlowReturn gst_b25specverifier_pushthrough(GstB25SpecVerifier * filter);
//static gboolean gst_b25specverifier_set_caps (GstPad * pad, GstCaps * caps);

/* GObject vmethod implementations */

/* initialize the b25's class */
static void
gst_b25specverifier_class_init (GstB25SpecVerifierClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_b25specverifier_set_property;
  gobject_class->get_property = gst_b25specverifier_get_property;

  g_object_class_install_property (gobject_class, PROP_ROUND,
      g_param_spec_int("round", "Round", "The hostname to listen as",
          INT32_MIN, INT32_MAX, MULTI2_ROUND_DEFAULT_VALUE,
	  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_STRIP,
      g_param_spec_boolean("strip", "Strip", "Strip null stream",
          FALSE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_EMM,
      g_param_spec_boolean("emm", "EMM", "send EMM to B-CAS card",
          FALSE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_KEEPGOING,
      g_param_spec_boolean("keepgoing", "Keep Going", "Input data outputs directly if errored.",
          FALSE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  gst_element_class_set_details_simple(gstelement_class,
    "ARIB-STD-B25 SpecVerifierr.",
    "Filter/Video",
    "SpecVerifier ARIB-STD-B25 scrambled TS stream.",
    "nanashi@example.com");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&src_factory));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_b25specverifier_init (GstB25SpecVerifier * filter)
{
  int err = 0;

  filter->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");
  gst_pad_set_event_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_b25specverifier_sink_event));
  gst_pad_set_chain_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_b25specverifier_chain));
  GST_PAD_SET_PROXY_CAPS (filter->sinkpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->sinkpad);

  filter->srcpad = gst_pad_new_from_static_template (&src_factory, "src");
  GST_PAD_SET_PROXY_CAPS (filter->srcpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->srcpad);

  filter->opt_round = MULTI2_ROUND_DEFAULT_VALUE;

  GST_OBJECT_FLAG_UNSET(filter, GST_B25SPECVERIFIER_INITIALIZED);

  filter->b25 = create_arib_std_b25();
  if(!filter->b25) {
    GST_ERROR("create_arib_std_b25 failed (%d)", err);
    return;
  }

  GST_INFO("round=%d", filter->opt_round);
  err = filter->b25->set_multi2_round(filter->b25, filter->opt_round);
  if(err < 0) {
    GST_ERROR("set_multi2_round failed (%d)", err);
    return;
  }

  GST_INFO("strip=%d", filter->opt_strip);
  err = filter->b25->set_strip(filter->b25, filter->opt_strip);
  if(err < 0) {
    GST_ERROR("set_strip failed (%d)", err);
    return;
  }

  GST_INFO("emm=%d", filter->opt_emm);
  err = filter->b25->set_emm_proc(filter->b25, filter->opt_emm);
  if(err < 0) {
    GST_ERROR("set_emm_proc failed (%d)", err);
    return;
  }

  GST_INFO("keepgoing=%d", filter->opt_keepgoing);

  filter->bcas = create_b_cas_card();
  if(!filter->bcas) {
    GST_ERROR("create_b_cas_card failed (%d)", err);
    return;
  }

  err = filter->bcas->init(filter->bcas);
  if(err < 0) {
    GST_ERROR("bcas->init failed (%d)", err);
    return;
  }

  err = filter->b25->set_b_cas_card(filter->b25, filter->bcas);
  if(err < 0) {
    GST_ERROR("set_b_cas_card failed (%d)", err);
    return;
  }

  GST_OBJECT_FLAG_SET(filter, GST_B25SPECVERIFIER_INITIALIZED);
}

static void
gst_b25specverifier_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstB25SpecVerifier *filter = GST_B25SPECVERIFIER (object);

  switch (prop_id) {
    case PROP_ROUND:
      filter->opt_round = g_value_get_int(value);
      GST_INFO("round = %d", filter->opt_round);
      break;
    case PROP_STRIP:
      filter->opt_strip = g_value_get_boolean (value);
      GST_INFO("strip = %d", filter->opt_strip);
      break;
    case PROP_EMM:
      filter->opt_emm = g_value_get_boolean (value);
      GST_INFO("emm = %d", filter->opt_emm);
      break;
    case PROP_KEEPGOING:
      filter->opt_keepgoing = g_value_get_boolean (value);
      GST_INFO("keepgoing = %d", filter->opt_keepgoing);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_b25specverifier_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstB25SpecVerifier *filter = GST_B25SPECVERIFIER (object);

  switch (prop_id) {
    case PROP_ROUND:
      g_value_set_int(value, filter->opt_round);
      break;
    case PROP_STRIP:
      g_value_set_boolean(value, filter->opt_strip);
      break;
    case PROP_EMM:
      g_value_set_boolean(value, filter->opt_emm);
      break;
    case PROP_KEEPGOING:
      g_value_set_boolean(value, filter->opt_keepgoing);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstElement vmethod implementations */

/* this function handles sink events */
static gboolean
gst_b25specverifier_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
  gboolean ret = FALSE;
  GstB25SpecVerifier *filter = GST_B25SPECVERIFIER(parent);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS:
    {
      GstCaps * caps;

      gst_event_parse_caps (event, &caps);
      /* do something with the caps */

      break;
    }
    case GST_EVENT_EOS:
      /* end-of-stream, we should close down all stream leftovers here */
      if(filter->error_occured) {
        GST_INFO("already error occured");
        break;
      }
      gst_b25specverifier_pushthrough(filter);
      if(ret != GST_FLOW_OK) {
        GST_ERROR("gst_b25specverifier_pushthrough error=%d", ret);
        break;
      }
      break;
    default:
      break;
  }
  ret = gst_pad_event_default (pad, parent, event);
  return ret;
}

#define ERROR_OR_KEEPGOING(...) \
  if(!filter->error_occured) { \
    GST_ERROR(__VA_ARGS__); filter->error_occured = TRUE; \
  } \
  if(filter->opt_keepgoing) { return gst_pad_push (filter->srcpad, input); } \
  return GST_FLOW_ERROR;

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_b25specverifier_chain (GstPad * pad, GstObject * parent, GstBuffer *input)
{
  int err = 0;
  GstB25SpecVerifier *filter = GST_B25SPECVERIFIER(parent);
  GstBuffer *buf = NULL;
  GstMapInfo map;

  ARIB_STD_B25_BUFFER dbuf = {0};
  ARIB_STD_B25_BUFFER sbuf = {0};

  if( !GST_OBJECT_FLAG_IS_SET(filter, GST_B25SPECVERIFIER_INITIALIZED) ) {
    ERROR_OR_KEEPGOING("Not initialized.");
  }

  if (gst_buffer_map (input, &map, GST_MAP_READ)) {
    //sbuf.data = GST_BUFFER_DATA(input),
    //sbuf.size = gst_buffer_get_size(input)
    sbuf.data = map.data;
    sbuf.size = map.size;
    gst_buffer_unmap (input, &map); 

    if( gst_buffer_get_size(input) > 16) {
      GST_DEBUG(" input.size=%d   %2x %2x %2x %2x %2x %2x %2x %2x  %2x %2x %2x %2x %2x %2x %2x %2x",
        map.size,
       	map.data[0],  map.data[1],
       	map.data[2],  map.data[3],
       	map.data[4],  map.data[5],
       	map.data[6],  map.data[7],
       	map.data[8],  map.data[9],
       	map.data[10], map.data[11],
       	map.data[12], map.data[13],
       	map.data[14], map.data[15]);
    }

    err = filter->b25->put(filter->b25, &sbuf);
    if(err < 0) {
      ERROR_OR_KEEPGOING("b25->put failed %d", err)
    }
  }

  err = filter->b25->get(filter->b25, &dbuf);
  if(err < 0) {
    ERROR_OR_KEEPGOING("b25->get failed %d", err);
  }

  if(dbuf.size == 0) {
    GST_DEBUG("buffer empty");
  }

  buf = gst_buffer_new_allocate(NULL, dbuf.size, NULL);
  if(buf == NULL) {
    ERROR_OR_KEEPGOING("gst_buffer_new_allocate failed");
  }

  gst_buffer_fill(buf, 0, dbuf.data, dbuf.size);
/*
  if( gst_buffer_get_size(buf) > 16) {
    GST_DEBUG("output.size=%d   %2x %2x %2x %2x %2x %2x %2x %2x  %2x %2x %2x %2x %2x %2x %2x %2x",
      gst_buffer_get_size(buf),
      GST_BUFFER_DATA(buf)[0],  GST_BUFFER_DATA(buf)[1],
      GST_BUFFER_DATA(buf)[2],  GST_BUFFER_DATA(buf)[3],
      GST_BUFFER_DATA(buf)[4],  GST_BUFFER_DATA(buf)[5],
      GST_BUFFER_DATA(buf)[6],  GST_BUFFER_DATA(buf)[7],
      GST_BUFFER_DATA(buf)[8],  GST_BUFFER_DATA(buf)[9],
      GST_BUFFER_DATA(buf)[10], GST_BUFFER_DATA(buf)[11],
      GST_BUFFER_DATA(buf)[12], GST_BUFFER_DATA(buf)[13],
      GST_BUFFER_DATA(buf)[14], GST_BUFFER_DATA(buf)[15]);
  }
*/
  gst_buffer_unref(input);

  /* just push out the incoming buffer without touching it */
  return gst_pad_push (filter->srcpad, buf);
}


/* push remaining data in the buffers out */
static GstFlowReturn
gst_b25specverifier_pushthrough(GstB25SpecVerifier * filter)
{
  int err = 0;
  GstBuffer *buf = NULL;
  ARIB_STD_B25_BUFFER dbuf = {0};

  GST_DEBUG("pushthrough");

  err = filter->b25->flush(filter->b25);
  if(err < 0) {
      GST_ERROR("b25->flush failed. %d", err);
      return GST_FLOW_ERROR;
  }

  err = filter->b25->get(filter->b25, &dbuf);
  if(err < 0) {
      GST_ERROR("b25->get failed. %d", err);
      return GST_FLOW_ERROR;
  }

  buf = gst_buffer_new_allocate(NULL, dbuf.size, NULL);
  if(buf == NULL) {
    GST_ERROR("gst_buffer_new_allocate failed");
    return GST_FLOW_ERROR;
  }	

  gst_buffer_fill(buf, 0, dbuf.data, dbuf.size);

  return gst_pad_push (filter->srcpad, buf);
}



/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
plugin_init (GstPlugin * plugin)
{
  /* debug category for fltering log messages
   *
   * exchange the string 'Template plugin' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_b25specverifier_debug, "b25specverifier",
      0, "ARIB-STD-B25 specification verifier plugin");

  return gst_element_register (plugin, "b25specverifier", GST_RANK_NONE,
      GST_TYPE_B25SPECVERIFIER);
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "b25specverifier"
#endif

/* 
 * Plugin's description.
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    b25specverifier,
    "ARIB-STD-B25 spec-verification plugin",
    plugin_init,
    VERSION,
    GST_LICENSE_UNKNOWN,
    "GStreamer pluggable ARIB STD-B25 spec-verification program.",
    "http://example.com/"
)
