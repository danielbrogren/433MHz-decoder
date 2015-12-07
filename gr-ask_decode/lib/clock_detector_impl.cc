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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "clock_detector_impl.h"

namespace gr {
  namespace ask_decode {

    clock_detector::sptr
    clock_detector::make(float threshold, int target_rate, int target_samp_per_sym)
    {
      return gnuradio::get_initial_sptr
        (new clock_detector_impl(threshold, target_rate, target_samp_per_sym));
    }

    /*
     * The private constructor
     */
    clock_detector_impl::clock_detector_impl(float threshold, int target_rate, int target_samp_per_sym)
      : gr::sync_block("clock_detector",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0, 0, 0)),
    	d_threshold(threshold), d_last_sample(0), d_count_positive_sample(500), 
	d_count_negative_sample(500), d_count_positive_flanks(0), 
	d_count_negative_flanks(0), estiamated_samples_per_sym(5),
	d_positive_flank(), d_negative_flank(), d_samp_adjust(),
	d_target_rate(target_rate), d_target_samp_per_sym(target_samp_per_sym)
    {
	d_samp_per_symb_port = pmt::string_to_symbol("samp_per_symb");
	message_port_register_out(d_samp_per_symb_port);
	d_samp_adjust_port = pmt::string_to_symbol("samp_adjust");
	message_port_register_out(d_samp_adjust_port);

	}

    /*
     * Our virtual destructor.
     */
    clock_detector_impl::~clock_detector_impl()
    {
    }

    int
    clock_detector_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
	float *out = (float *) output_items[0];

        // Tell runtime system how many input items we consumed on
        // each input stream.

	for(int i = 0; i < noutput_items; i++)
        {
		//calling function to estimate samp/sym
		out[i] = get_ask_samp_per_sym(in[i]);
        }

        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

	int clock_detector_impl::samp_adjust()
    {
	d_samp_adjust = (int)((float)(d_target_rate*d_target_samp_per_sym)/estiamated_samples_per_sym) - d_target_rate;
	return d_samp_adjust;
    }

    float
    clock_detector_impl::samp_per_symb()
    {

	return estiamated_samples_per_sym;
    }


float 
clock_detector_impl::get_ask_samp_per_sym(const float &sample)
{
	//Check for positiv flank
	if(d_last_sample < d_threshold && sample >= d_threshold)
	{
		//printf("positive flank\n");
		if(d_count_positive_sample < 500)
		{
			d_positive_flank[d_count_positive_sample]++;
			//printf("position %i \n", d_positive_flank[d_count_positive_sample]);
		}
		d_count_positive_flanks++;
		d_count_positive_sample = 0;
	}	


	//Check for negative flank  		
	if(d_last_sample > d_threshold && sample <= d_threshold)
	{
		if(d_count_negative_sample < 500)
		{
			d_negative_flank[d_count_negative_sample]++;
		}
		d_count_negative_flanks++;
		d_count_negative_sample = 0;			
	}

	//see to that it dosent wrap around
	if(d_count_positive_sample > 32760)
	{d_count_positive_sample = 501; }
	if(d_count_negative_sample > 32760)
	{d_count_negative_sample = 501; }		

	//Estimate sample per symbol
	//only if we have at least total 100 flanks detected
	if(d_count_positive_flanks + d_count_negative_flanks >= 100)
	{
		//printf("estimate flank\n");
		bool found_max = false;
		int i = 3; //start check at sample 3 (magic number....)
		//Positive flank
		while(found_max == false)
		{
			if(d_positive_flank[i-1] <= d_positive_flank[i] && 
				d_positive_flank[i] >= d_positive_flank[i+1] && 
				d_positive_flank[i] > 5) 
				//We should detect at least 5 flanks at that sample (magic number...)
			{
				found_max = true;
		
				//printf("Found max pos %i\n", i);
				//for(int j = 0; j<300; j++)
				//	{printf("%i ", d_positive_flank[j]);}
				//printf("\n");

				estiamated_samples_per_sym = (float(estiamated_samples_per_sym) / 2) + 
					(float(d_positive_flank[i-1]*(i-1) + 
					d_positive_flank[i]*(i) + 
					d_positive_flank[i+1]*(i+1)) / 
					float(d_positive_flank[i-1] + d_positive_flank[i] + d_positive_flank[i+1]) / 
					float(4));
				//uses half the last value as "memory". The /4 is due to that we use half the new value and that we have two symbols between two flanks					

				d_count_positive_flanks = 0;

				std::fill(d_positive_flank, d_positive_flank+500, 0);
			}
			i++;
		}//End positive flank

		//Negative flank
		found_max = false;
		i = 3;
		while(found_max == false)
		{
			if(d_negative_flank[i-1] <= d_negative_flank[i] && 
				d_negative_flank[i] >= d_negative_flank[i+1] && 
				d_negative_flank[i] > 5)
			{
				found_max = true;
				//printf("Found max neg %i\n", i);
				//for(int j = 0; j<300; j++)
				//	{printf("%i ", d_negative_flank[j]);}
				//printf("\n");

				estiamated_samples_per_sym = (float(estiamated_samples_per_sym) / 2) + 
					(float(d_negative_flank[i-1]*(i-1) + 
					d_negative_flank[i]*(i) + 
					d_negative_flank[i+1]*(i+1)) / 
					float(d_negative_flank[i-1] + d_negative_flank[i] + d_negative_flank[i+1]) / 
					float(4));

				d_count_negative_flanks = 0;

				std::fill(d_negative_flank, d_negative_flank+500, 0);
			}
			i++;
		}//End negative flank

		message_port_pub(d_samp_adjust_port, pmt::from_float((float)samp_adjust()));
		message_port_pub(d_samp_per_symb_port, pmt::from_float(samp_per_symb()));

	}//ENd 100 flanks detected

	//stores the last sample
	d_last_sample = sample;
	//Counting up the positive/negative flanks
	d_count_positive_sample++;
	d_count_negative_sample++;

	return estiamated_samples_per_sym;

}




  } /* namespace ask_decode */
} /* namespace gr */

