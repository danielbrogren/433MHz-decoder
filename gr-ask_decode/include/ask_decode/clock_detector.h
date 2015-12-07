/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_ASK_DECODE_CLOCK_DETECTOR_H
#define INCLUDED_ASK_DECODE_CLOCK_DETECTOR_H

#include <ask_decode/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ask_decode {

    /*!
     * \brief <+description of block+>
     * \ingroup ask_decode
     *
     */
    class ASK_DECODE_API clock_detector : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<clock_detector> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ask_decode::clock_detector.
       *
       * To avoid accidental use of raw pointers, ask_decode::clock_detector's
       * constructor is in a private implementation
       * class. ask_decode::clock_detector::make is the public interface for
       * creating new instances.
       */
      static sptr make(float threshold, int target_rate, int target_samp_per_sym);
	virtual float samp_per_symb() = 0;
	virtual int samp_adjust() = 0;

    };

  } // namespace ask_decode
} // namespace gr

#endif /* INCLUDED_ASK_DECODE_CLOCK_DETECTOR_H */

