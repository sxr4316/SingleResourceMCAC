/*
           This file will convert uLaw PCM data to ADPCM using formulas 
	   in ANSI spec. T1.301-1987.
*/

/*
 *
 *  @(#) enc.c 3.4@(#)
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "encode.h"

long encode();

FILE *fpin ;
FILE *fpout ;

long yl;
long dq1,dq2,dq3,dq4,dq5,dq6;
long b1,b2,b3,b4,b5,b6,a1,a2;
long pk1,pk2,sr1,sr2;
long ap,yu;
long dms,dml;
long y,se,al,sez,sr,td;
long pcm,sd;
int br = 16;
int smdq = 16;

long I;
long tdr,sigpk,tdp;
long pk0,ss,sl,wb1,b1p,b2p,b3p,b4p,b5p,b6p;
long u1,u2,u3,u4,u5,u6;
long wb2,wb3,wb4,wb5,wb6,wa2,wa1;
long d,dl,ds,dln,dqln,dqs,dql;
long dq,wi,yut,fi,ax,a2t,a1t;
long yup,dmsp,dmlp,app,sr0,dq0,a2p,a1p;
long a1r,a2r,b1r,b2r,b3r,b4r,b5r,b6r;
long ylp;

long aLaw;

long I_store,tr,apr;

extern long inv_g711(),g711(),expand(),subta(),subtb();
extern long quan(),adda(),antilog();
extern long filtd(),functw();
extern long filte(),upa2();
extern long mix(),functf(),addb(),addc();
extern long floata(),floatb(),fmult();
extern long limc(),limb(),limd(),upa1();
extern long upa1(),upb(),xor(),lima(),subtc(),llog();
extern long filta(),filtb(),filtc();
extern long compress(),synch();
extern long adapt_pred(),inv_adapt_quan(),adapt_quan();
extern long triga(),trigb(),tone(),trans();
extern long quan_scl_fact(),adapt_speed(),output_conv();
extern long getSample();
extern void dumpState();

char prog_opts[] = "hatdvi:o:r:";
extern int optind;
extern char *optarg;
void usage();

int debug = 0 ;

extern void openFiles();
extern void closeFiles();
extern void printVars();

main(argc,argv)
int argc;
char *argv[];
{
    fpin = stdin;
    fpout = stdout;
    int len,tst,init_cnt;
	int inFlag = 0;
	int outFlag = 0;
    int printVectors = 0;
	int i, val;

	char *outfile;
	char *infile;
	char c;

	tst = 0;
	aLaw = 0;
	sd = 0; /* not used in encoder */


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

    if (printVectors)
    {
        openFiles();
    }

/* initialize values */
    dms = dml = 0;
    ap = pk1 = pk2 = 0;
    b1 = b2 = b3 = b4 = b5 = b6 = 0;
    a2 = a1 = 0;
    yl = 34816; yu = 544;
    sr1 = sr2 = 32;
    dq1 = dq2 = dq3 = dq4 = dq5 = dq6 = 32;
    td=0;
    len = 0;
	i = 1;

    /* get pcm sample */
	val = getSample(fpin);

    long lIn = 0;
    long lOut = 0;

    while(!feof(fpin))
    {
		
        

		if(tst)
			printf("#################### %d #################\n", i);
		if(tst)
			dumpState(i, aLaw, MODE_ENCODER);

        pcm = (long)val & 0xff;
        ss = g711(pcm, aLaw);		/* expand 8 bits to 14/13 bit */
        sl = expand(ss, aLaw);		/* convert into linear */
/* block a */
    /* step 1 */
        wb1 = fmult(b1,dq1);
        wb2 = fmult(b2,dq2);
        wb3 = fmult(b3,dq3);
        wb4 = fmult(b4,dq4);
        wb5 = fmult(b5,dq5);
        wb6 = fmult(b6,dq6);
        wa2 = fmult(a2,sr2);
        wa1 = fmult(a1,sr1);
    /* step 2 */
        accum(wa1,wa2,wb1,wb2,wb3,wb4,wb5,wb6);
    /* step 3 */
        al = lima(ap);		/* limit speed control */
    /* step 4 */
        y = mix(al,yu,yl);	/* form linear combination of fast and slow */
/* encode block */
    /* step 5 */
        d = subta(sl,se);	/* compute difference  */
    /* step 6 */		/* adaptive quantizer */
        llog(d,&dl,&ds);		/* convert linear to log domain */
    /* step 7 */
        dln = subtb(dl,y);
    /* step 8 */
        I = quan(dln,ds);	/* quantize difference signal */
/* block b */
    /* step 9 */		/* inverse adaptive quantizer */
        reconst(I,&dqln,&dqs);	/* reconstruct quantized signal */
    /* step 10 */
        dql = adda(dqln,y);	/* scale log of quantized signal */
    /* step 11 */
        dq = antilog(dql,dqs);	/* convert quantized signal to linear */
    /* step 12 */
	    tr = trans(td,yl,dq);
    /* step 13 */
        pk0 = addc(dq,sez,&sigpk);
    /* step 14 */
        a2t = upa2(pk0,pk1,pk2,a1,a2,sigpk);
    /* step 15 */
        a2p = limc(a2t);
    /* step 16 */
	   tdp = tone(a2p);
    /* step 17 */
	   tdr = trigb(tr,tdp);
    /* step 18 */
        sr = addb(dq,se);
    /* step 19 */
        sr0 = floatb(sr);
    /* step 20 */
        dq0 = floata(dq);
    /* step 21 */
        u1 = xor(dq1,dq);
        u2 = xor(dq2,dq);
        u3 = xor(dq3,dq);
        u4 = xor(dq4,dq);
        u5 = xor(dq5,dq);
        u6 = xor(dq6,dq);
    /* step 22 */
        b1p = upb(u1,b1,dq);
        b2p = upb(u2,b2,dq);
        b3p = upb(u3,b3,dq);
        b4p = upb(u4,b4,dq);
        b5p = upb(u5,b5,dq);
        b6p = upb(u6,b6,dq);
    /* step 23 */
		b1r = trigb(tr,b1p);
		b2r = trigb(tr,b2p);
		b3r = trigb(tr,b3p);
		b4r = trigb(tr,b4p);
		b5r = trigb(tr,b5p);
		b6r = trigb(tr,b6p);
    /* step 24 */
		a2r = trigb(tr,a2p);
    /* step 25 */
        a1t = upa1(pk0,pk1,a1,sigpk);
    /* step 26 */
        a1p = limd(a1t,a2p);
    /* step 27 */
    	a1r = trigb(tr,a1p);
    /* step 28 */
        wi = functw(I);		/* map quantizer into logarithmic version */
    /* step 29 */
        yut = filtd(wi,y);	/* update fast quantizer */
    /* step 30 */
        yup = limb(yut);	/* limit quantizer */
    /* step 31 */
        ylp = filte(yup,yl);	/* update slow quantizer */
    /* step 32 */
        fi = functf(I);		/* map quantizer output into fi function */
    /* step 33 */
        dmsp = filta(fi,dms);	/* update short term average */
    /* step 34 */
        dmlp = filtb(fi,dml);	/* update long term average */
    /* step 35 */
        ax = subtc(dmsp,dmlp,y,tdp); /* compute difference of shrt/lng term*/
    /* step 36 */
        app = filtc(ax,ap);	/* low pass filter */
    /* step 37 */
		apr = triga(tr,app);
/* block c */

        // Output variables
        if (printVectors)
        {
            printVars();
        }

    /* now perform the delays, steps 38-59 */
		yu = yup;
		yl = ylp;
		dms = dmsp;
		dml = dmlp;
		ap = apr;
		pk2 = pk1;
		pk1 = pk0;
		sr2 = sr1;
		sr1 = sr0;
		b1 = b1r;
		b2 = b2r;
		b3 = b3r;
		b4 = b4r;
		b5 = b5r;
		b6 = b6r;
		dq6 = dq5;
		dq5 = dq4;
		dq4 = dq3;
		dq3 = dq2;
		dq2 = dq1;
		dq1 = dq0;
		a2 = a2r;
		a1 = a1r;
		td = tdr;

		/* output adpcm */
		fprintf( fpout, "%02x", I );
		if (( i != 0 ) && ((i % FILE_LINE) == 0 ))
			fprintf( fpout, "\n" );
		i++;

        lOut = I;
        if (printVectors)
        {
          printoutput(lOut);
        }

    	/* get pcm sample */
		val = getSample(fpin);

    }
    fclose(fpin);
    fclose(fpout);

    if (printVectors)
    {
        closeFiles();
    }

	exit(0);
}

atoh(val)
char val;
{
    if(val <= 0x39) return(val & 0x0f);
    else return((val & 0x0f) + 9);
}

/* print usage message */
void usage()
{

	fprintf(stderr,"\n encode PCM to ADPCM, ANSI spec. T1.301-1987");
	fprintf(stderr,"\n usage: enc -h -a -t -d -o <output_file_name> -i <input_file_name> -r <bitrate>");
	fprintf(stderr,"\n  where:");
	fprintf(stderr,"\n \t\t-h print this help message)");
	fprintf(stderr,"\n \t\t-a process a-law pcm");
	fprintf(stderr,"\n \t\t-t enable sample tracing");
	fprintf(stderr,"\n \t\t-d enable debug messages");
	fprintf(stderr,"\n \t\t-o <output_file_name>\t(output file created)");
	fprintf(stderr,"\n \t\t   (default: standard out)");
	fprintf(stderr,"\n \t\t-i <input_file_name>\t(input file to process)");
	fprintf(stderr,"\n \t\t   (default: standard in)");
    fprintf(stderr,"\n \t\t-r <bitrate>\t(16, 24, 32, or 40)");
    fprintf(stderr,"\n \t\t   (default: 16)");
    fprintf(stderr,"\n \t\t-v enable test vector generation");
    fprintf(stderr,"\n");
	exit(1);
}