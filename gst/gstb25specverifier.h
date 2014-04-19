/*
 * Copyright (c) 2014 TOKITA Hiroshi <tokita.hiroshi@gmail.com>
 * https://github.com/tokitah/arib25-gst
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __GST_B25SPECVERIFIER_H__
#define __GST_B25SPECVERIFIER_H__

#include <gst/gst.h>

#include <arib_std_b25.h>
#include <b_cas_card.h>

G_BEGIN_DECLS

/* #defines don't like whitespacey bits */
#define GST_TYPE_B25SPECVERIFIER \
  (gst_b25specverifier_get_type())
#define GST_B25SPECVERIFIER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_B25SPECVERIFIER,GstB25SpecVerifier))
#define GST_B25SPECVERIFIER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_B25SPECVERIFIER,GstB25SpecVerifierClass))
#define GST_IS_B25SPECVERIFIER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_B25SPECVERIFIER))
#define GST_IS_B25SPECVERIFIER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_B25SPECVERIFIER))

typedef struct _GstB25SpecVerifier      GstB25SpecVerifier;
typedef struct _GstB25SpecVerifierClass GstB25SpecVerifierClass;

typedef enum {
  GST_B25SPECVERIFIER_INITIALIZED = (GST_ELEMENT_FLAG_LAST << 0),
} GstB25SpecVerifierFlags;

struct _GstB25SpecVerifier {
  GstElement element;
  GstPad *sinkpad, *srcpad;

  ARIB_STD_B25 *b25;
  B_CAS_CARD *bcas;
  gboolean error_occured;

  int opt_round;
  gboolean opt_strip;
  gboolean opt_emm;
  gboolean opt_keepgoing;
};

struct _GstB25SpecVerifierClass 
{
  GstElementClass parent_class;
};

GType gst_b25specverifier_get_type (void);

G_END_DECLS

#endif /* __GST_B25SPECVERIFIER_H__ */
