

all:rx_decoder_433MHz float_to_bit


rx_decoder_433MHz: rx_decoder_433MHz.o

float_to_bit: float_to_bit.o



clean: 
	@rm -f float_to_bit rx_decoder_433MHz  *.o 


