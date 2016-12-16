#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/encode.h"
#include "../include/compress_lut.h"
#include "../include/expand_lut.h"
#include "../include/llog_exp_lut.h"
#include "../include/g711_lut.h"
#include "../include/synch_quan_lut.h"

#define NUM_CHANNELS 32

long decode();
long getSample();

FILE *fpin ;
FILE *fpout ;

char prog_opts[] = "hatdvi:o:r:";
extern int optind;
extern char *optarg;
void usage();

extern void openFiles();
extern void closeFiles();
extern void printVars();
extern void printChannel();

extern long     reconbr32_tbl[];
extern long     reconbr40_tbl[];
extern long     reconbr24_tbl[];
extern long     reconbr16_tbl[];

extern long multbr40_tbl[];
extern long multbr32_tbl[];
extern long multbr24_tbl[];
extern long multbr16_tbl[];

void dec_flattened(int val, struct channel* singleChannel, int printVectors);
void initializeChannel(struct channel*, long aLaw, long br, long smdq);

main(argc,argv)
int argc;
char *argv[];
{
    fpin = stdin;
    fpout = stdout;
    int i,len,cnt;
	int val;
	int inFlag = 0;
	int outFlag = 0;
	int printVectors = 0;

	char *outfile;
	char *infile;
	char c;
	int debug = 0 ;
	long br = 16;	
	int tst = 0;
	long aLaw = 0;
   	long smdq = 16;
    int forceAllVectors = 0;

	/* get command line options */
	while ((c = getopt(argc, argv, prog_opts)) != EOF)
		switch (c) {
			/* user passed input filename */
			case 'i':
				infile = optarg;
				inFlag++;
				break;
			/* user passed output filename */
			case 'o':
				outfile = optarg;
				outFlag++;
				break;
			/* print test sequence */
			case 't':
				tst = 1;
				break;
			/* print debug messages */
			case 'd':
				debug = 1;
				break;
			/* process a-law pcm */
			case 'a':
				aLaw = 1;
				break;
			/* print variable vectors */
            case 'v':
                printVectors = 1;
                break;
			/* set bit rate */
            case 'r':
                br = atoi(optarg);
                break;
            case 'f':
                forceAllVectors = 1;
                break;
			/* print usage (help) message */
			case 'h':
			default:
				usage();
				break;
			}

    if ( inFlag ) {
		if (( fpin = fopen(infile,"r" )) == NULL )
    		{
			printf(" can't open file %s \n", infile);
			exit(0);
    		}
	}

    if ( outFlag ) {
		if (( fpout = fopen(outfile,"w" )) == NULL )
    		{
			printf(" can't open file %s \n", outfile);
			exit(0);
    		}
	}

    if (!forceAllVectors)
    {
        setOnlyTopLevel();
    }

    if (printVectors)
    {
        openFiles();
    }

    struct channel channels[NUM_CHANNELS] = {0};
    struct channel* singleChannel;

    int n = 0;

    /* initialize channel values */
    for (n=0; n<NUM_CHANNELS; n++)
    {
        singleChannel = &channels[n];
        initializeChannel(singleChannel, aLaw, br, smdq);
    }

    /* initialize program values*/
	i = 1;

    /* get adpcm sample */
	val = getSample(fpin);

    n = 0;

    long lIn = 0;
    long lOut = 0;

    while(!feof(fpin))
    {
        lIn = (long)val;
        if (printVectors)
        {
            printinput(lIn);
        }

		if(tst){
			printf("\n################### %d ###############\n", i);
		}
		
        singleChannel = &channels[n];
        dec_flattened(val, singleChannel, printVectors && forceAllVectors);

		/* output adpcm */
		fprintf( fpout, "%02x", singleChannel->sd );
		if (( i != 0 ) && ((i % FILE_LINE) == 0 ))
			fprintf( fpout, "\n" );
		i++;

        lOut = singleChannel->sd;
        if (printVectors)
        {
            printoutput(lOut);
        }

    	/* get adpcm sample */
		val = getSample(fpin);

        n++;
        if (n == NUM_CHANNELS)
        {
            n = 0;
        }
     }

    fclose(fpin);
    fclose(fpout);

    if (printVectors){
        closeFiles();
    }

	exit(0);
}

void initializeChannel(struct channel* singleChannel, long aLaw, long br, long smdq){
    /* initialize channel values */
    singleChannel->aLaw = aLaw;
    singleChannel->br = br;
    singleChannel->smdq = smdq;
    singleChannel->br_2bit = (br == 40 ? 00 : (br == 32 ? 01 : (br == 24 ? 02 : 03)));
    singleChannel->I = 0;	//added
    singleChannel->dms = 0;
    singleChannel->dml = 0;
    singleChannel->ap = 0;
    singleChannel->pk1 = 0;
    singleChannel->pk2 = 0;
    singleChannel->b1 = 0;
    singleChannel->b2 = 0;
    singleChannel->b3 = 0;
    singleChannel->b4 = 0;
    singleChannel->b5 = 0;
    singleChannel->b6 = 0;
    singleChannel->a2 = 0;
    singleChannel->a1 = 0;
    singleChannel->yl = 34816;
    singleChannel->yu = 544;
    singleChannel->sr1 = 32; 
    singleChannel->sr2 = 32;
    singleChannel->dq1 = 32;
    singleChannel->dq2 = 32;
    singleChannel->dq3 = 32;
    singleChannel->dq4 = 32;
    singleChannel->dq5 = 32;
    singleChannel->dq6 = 32;
    singleChannel->td = 0;
    singleChannel->len = 0;	//added
}

long
getSample( fpin )
FILE *fpin;
{
    char    buff[3];
    long    val;

    buff[2] = (char)NULL;
    buff[0] = fgetc(fpin);
    if ( !isxdigit(buff[0]))
        buff[0] = fgetc(fpin);
    buff[1] = fgetc(fpin);
    if ( !isxdigit(buff[1]))
        buff[1] = fgetc(fpin);
    sscanf( buff, "%x", &val );

    return( val );
}

void usage()
{

	fprintf(stderr,"\n decode ADPCM to PCM, ANSI spec. T1.301-1987");
	fprintf(stderr,"\n usage: dec -h -a -t -d -o <output_file_name> -i <input_file_name> -b <bitrate>");
	fprintf(stderr,"\n  where:");
	fprintf(stderr,"\n \t\t-h print this help message");
	fprintf(stderr,"\n \t\t-a process a-law pcm");
	fprintf(stderr,"\n \t\t-t enable sample tracing");
	fprintf(stderr,"\n \t\t-d enable debug messages");
	fprintf(stderr,"\n \t\t-o <output_file_name>\t(output file created)");
	fprintf(stderr,"\n \t\t   (default: standard out)");
	fprintf(stderr,"\n \t\t-i <input_file_name>\t(input file to process)");
	fprintf(stderr,"\n \t\t   (default: standard out)\n");
    fprintf(stderr,"\n \t\t-r <bitrate>\t(16, 24, 32, or 40)");
    fprintf(stderr,"\n \t\t   (default: 16)");
    fprintf(stderr,"\n \t\t-v enable test vector generation");
    fprintf(stderr,"\n");
	exit(1);
}

