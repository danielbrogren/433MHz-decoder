#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	int fd, fd_out, opt, num_samp_per_sym;

	int opt_verbose = 0;

	while ((opt = getopt(argc, argv, "v")) != -1) {
		switch (opt) {
		case 'v':
			opt_verbose = 1;
			break;
		default:
			exit(2);
		}
	}

	if (argc <= optind+2) {
		fprintf(stderr, "Usage: %s [-v] <infile> <outfile> <samp/symbol>\n", argv[0]);
		exit(2);
	}

	fd = open(argv[optind], O_RDONLY);
	if (fd < 0) {
		perror("open infile");
		exit(1);
	}

	fd_out = creat(argv[optind+1], 0660);
	if (fd_out < 0) {
		perror("open outfile");
		exit(1);
	}

    num_samp_per_sym = 5;
    char *p;
    num_samp_per_sym = strtol(argv[optind+2], &p, 10);
    if(num_samp_per_sym < 5)
    {
		fprintf(stderr, "the number of samples per symbol must be > 4");
		exit(2);
    }
    if(opt_verbose){
        printf(" \n num_samp_per_sym: %i \n", num_samp_per_sym);
    }

	while (1) {
		int rc;
		float fl;
		uint8_t bit;
		int count_1;
		int count_0;
		rc = read(fd, &fl, sizeof(fl));
		if (rc < 0) {
			perror("read");
			exit(1);
		} else if (rc == 0)
			break;

		if(fl > 0.01)
		{
			count_1++;
			count_0=0;
		}
		else
		{
			count_0++;
			count_1=0;
		}

		if(count_1 % num_samp_per_sym == num_samp_per_sym -2)
		{
			bit=1;
            if(opt_verbose){
                printf("%1u", bit);
                }
			write(fd_out, &bit, 1);
		}

		if(count_0 % num_samp_per_sym == num_samp_per_sym -2)
		{
			bit=0;
            if(opt_verbose){
                printf("%1u", bit);
                }
			write(fd_out, &bit, 1);
		}
	}// End while
	exit(0);
}
