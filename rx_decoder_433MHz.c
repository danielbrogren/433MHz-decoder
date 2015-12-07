#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>



struct decoder_433_info {

	int numberOfZeros;
	int dataStart;
	int dataStop;
	int ongoingData;
	int numberOfZerosThreshold;
	uint8_t data[1024];


};

void init_decoder_433_info(struct decoder_433_info *rxInfo)
{
	rxInfo->numberOfZeros = 0;
 	rxInfo->dataStart = 0;
 	rxInfo->dataStop = 0;
	rxInfo->ongoingData = 0;
    rxInfo->numberOfZerosThreshold = 10;
}

void data_done(struct decoder_433_info *rxInfo)
{

int i;
int firstOne =0;
//

if(rxInfo->dataStop > 20)//TODO
{
printf("datadone %i ",  rxInfo->dataStop);
for(i=0; i<rxInfo->dataStop; i++)
{
    if(rxInfo->data[i] == 1)
    {
    firstOne =1;
    }
    if(firstOne ==1)
    {
    printf("%lu", rxInfo->data[i]);
    }

}
 printf("\n");
}

}

void rx_decoder(struct decoder_433_info *rxInfo, uint8_t *bits, int len)
{
//TODO

    uint8_t buf[1024];
    int i;
    int j;
    int bitsLeftForNextRound;
    int startBitForNextRound;
/*
printf ("new buffer, len:%i\n", len);
    for(j=0; j< len; j++)
    {
        printf("%lu",bits[j]);
    }
printf ("\n");

printf ("old buffer, len:%i\n", rxInfo->dataStop);
    for(j=0; j< rxInfo->dataStop; j++)
    {
        printf("%lu",rxInfo->data[j]);
    }
printf ("\n");
*/
    //First copy the new data to the end of the old buffer
    memcpy(&rxInfo->data[rxInfo->dataStop], bits, len);
    rxInfo->dataStop = rxInfo->dataStop + len;
/*
printf ("after copy buffer, len:%i\n", rxInfo->dataStop);
    for(i=0; i< rxInfo->dataStop; i++)
    {
        printf("%lu",rxInfo->data[i]);
    }
printf ("\n");
*/

    for(i = 0; i< rxInfo->dataStop; i++)
    {
        if(rxInfo->data[i] == 0)
        {
            rxInfo->numberOfZeros++;
            //Check if data is done
            if(rxInfo->numberOfZeros >= rxInfo->numberOfZerosThreshold)
            {
                rxInfo->ongoingData = 0;
                data_done(rxInfo);
                rxInfo->dataStop = 0;
                rxInfo->numberOfZeros = 0;

            }
        }
        else if(rxInfo->data[i] == 1)
        {
        rxInfo->numberOfZeros = 0;
            rxInfo->ongoingData = 1;
        }


    }





}



int main(int argc, char **argv)
{
	int fd;
	struct decoder_433_info *rxInfo = malloc(sizeof (struct decoder_433_info));

	init_decoder_433_info(rxInfo);

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <file_with_1_byte_per_bit>\n", argv[0]);
		exit(1);
	}




	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(2);
	}



	while (1) {


//printf("daniel");

        //Doing the decoding
		uint8_t buf[10];
		int len;

		len = read(fd, buf, sizeof(buf));

		if (len < 0) {
			perror("read");
			exit(1);
		} else if (len == 0) {
			printf("EOF");
			break;
		}
//printf("incomming %i %1u \n", len, buf[0]);
		rx_decoder(rxInfo, buf, len);
	}



	free(rxInfo);
	exit(0);
}











