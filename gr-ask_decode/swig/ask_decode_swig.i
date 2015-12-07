/* -*- c++ -*- */

#define ASK_DECODE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ask_decode_swig_doc.i"

%{
#include "ask_decode/clock_detector.h"
%}


%include "ask_decode/clock_detector.h"
GR_SWIG_BLOCK_MAGIC2(ask_decode, clock_detector);
