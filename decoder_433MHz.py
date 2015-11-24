#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Decoder 433Mhz
# Generated: Tue Nov 24 21:04:41 2015
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
from gnuradio.wxgui import scopesink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import osmosdr
import time
import wx

class decoder_433MHz(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Decoder 433Mhz")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.target_rate_IM = target_rate_IM = 26000
        self.target_rate = target_rate = 1300
        self.samp_rate = samp_rate = 2000000
        self.xlate_offset = xlate_offset = -54600
        self.samp_per_sym = samp_per_sym = 5
        self.firdes_tap_2 = firdes_tap_2 = firdes.low_pass(1, target_rate_IM, 500, target_rate/4, firdes.WIN_HAMMING, 6.76)
        self.firdes_tap = firdes_tap = firdes.low_pass(1, samp_rate, 2000, target_rate_IM/4, firdes.WIN_HAMMING, 6.76)

        ##################################################
        # Blocks
        ##################################################
        _xlate_offset_sizer = wx.BoxSizer(wx.VERTICAL)
        self._xlate_offset_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_xlate_offset_sizer,
        	value=self.xlate_offset,
        	callback=self.set_xlate_offset,
        	label="xlate_offset",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._xlate_offset_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_xlate_offset_sizer,
        	value=self.xlate_offset,
        	callback=self.set_xlate_offset,
        	minimum=-samp_rate/20,
        	maximum=samp_rate/20,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_xlate_offset_sizer, 2, 2, 1, 1)
        self.wxgui_scopesink2_0 = scopesink2.scope_sink_f(
        	self.GetWin(),
        	title="Scope Plot",
        	sample_rate=target_rate*samp_per_sym,
        	v_scale=0.2,
        	v_offset=0,
        	t_scale=0,
        	ac_couple=False,
        	xy_mode=False,
        	num_inputs=1,
        	trig_mode=wxgui.TRIG_MODE_AUTO,
        	y_axis_label="Counts",
        )
        self.GridAdd(self.wxgui_scopesink2_0.win, 2, 1, 10, 1)
        self.wxgui_fftsink2_1 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=target_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.GridAdd(self.wxgui_fftsink2_1.win, 1, 2, 1, 1)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.GridAdd(self.wxgui_fftsink2_0.win, 1, 1, 1, 1)
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=samp_per_sym,
                decimation=target_rate_IM/target_rate,
                taps=None,
                fractional_bw=None,
        )
        self.osmosdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + "" )
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(434e6, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(0, 0)
        self.osmosdr_source_0.set_iq_balance_mode(0, 0)
        self.osmosdr_source_0.set_gain_mode(False, 0)
        self.osmosdr_source_0.set_gain(10, 0)
        self.osmosdr_source_0.set_if_gain(20, 0)
        self.osmosdr_source_0.set_bb_gain(20, 0)
        self.osmosdr_source_0.set_antenna("", 0)
        self.osmosdr_source_0.set_bandwidth(0, 0)
          
        self.freq_xlating_fir_filter_xxx_1 = filter.freq_xlating_fir_filter_ccc(int(samp_rate/target_rate_IM), (firdes_tap), xlate_offset, samp_rate)
        self.blocks_multiply_const_vxx_1 = blocks.multiply_const_vcc((10, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((0.001, ))
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_float*1, "/tmp/gnu_radio_out", False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blocks_complex_to_mag_squared_0 = blocks.complex_to_mag_squared(1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.wxgui_scopesink2_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_1, 0), (self.blocks_complex_to_mag_squared_0, 0))    
        self.connect((self.freq_xlating_fir_filter_xxx_1, 0), (self.rational_resampler_xxx_0, 0))    
        self.connect((self.freq_xlating_fir_filter_xxx_1, 0), (self.wxgui_fftsink2_1, 0))    
        self.connect((self.osmosdr_source_0, 0), (self.freq_xlating_fir_filter_xxx_1, 0))    
        self.connect((self.osmosdr_source_0, 0), (self.wxgui_fftsink2_0, 0))    
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_multiply_const_vxx_1, 0))    


    def get_target_rate_IM(self):
        return self.target_rate_IM

    def set_target_rate_IM(self, target_rate_IM):
        self.target_rate_IM = target_rate_IM
        self.set_firdes_tap(firdes.low_pass(1, self.samp_rate, 2000, self.target_rate_IM/4, firdes.WIN_HAMMING, 6.76))
        self.set_firdes_tap_2(firdes.low_pass(1, self.target_rate_IM, 500, self.target_rate/4, firdes.WIN_HAMMING, 6.76))

    def get_target_rate(self):
        return self.target_rate

    def set_target_rate(self, target_rate):
        self.target_rate = target_rate
        self.set_firdes_tap_2(firdes.low_pass(1, self.target_rate_IM, 500, self.target_rate/4, firdes.WIN_HAMMING, 6.76))
        self.wxgui_fftsink2_1.set_sample_rate(self.target_rate)
        self.wxgui_scopesink2_0.set_sample_rate(self.target_rate*self.samp_per_sym)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_firdes_tap(firdes.low_pass(1, self.samp_rate, 2000, self.target_rate_IM/4, firdes.WIN_HAMMING, 6.76))
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)

    def get_xlate_offset(self):
        return self.xlate_offset

    def set_xlate_offset(self, xlate_offset):
        self.xlate_offset = xlate_offset
        self._xlate_offset_slider.set_value(self.xlate_offset)
        self._xlate_offset_text_box.set_value(self.xlate_offset)
        self.freq_xlating_fir_filter_xxx_1.set_center_freq(self.xlate_offset)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym
        self.wxgui_scopesink2_0.set_sample_rate(self.target_rate*self.samp_per_sym)

    def get_firdes_tap_2(self):
        return self.firdes_tap_2

    def set_firdes_tap_2(self, firdes_tap_2):
        self.firdes_tap_2 = firdes_tap_2

    def get_firdes_tap(self):
        return self.firdes_tap

    def set_firdes_tap(self, firdes_tap):
        self.firdes_tap = firdes_tap
        self.freq_xlating_fir_filter_xxx_1.set_taps((self.firdes_tap))


if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = decoder_433MHz()
    tb.Start(True)
    tb.Wait()
