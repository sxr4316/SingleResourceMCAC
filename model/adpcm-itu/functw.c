#include <stdio.h>
#include <stdlib.h>
#include "encode.h"


extern int debug;

extern long yu;
extern long yl;
extern long dms;
extern long dml;
extern long ap;
extern long pk2;
extern long pk1;
extern long sr2;
extern long sr1;
extern long b1;
extern long b2;
extern long b3;
extern long b4;
extern long b5;
extern long b6;
extern long dq6;
extern long dq5;
extern long dq4;
extern long dq3;
extern long dq2;
extern long dq1;
extern long a2;
extern long a1;
extern long td;
extern long sez;
extern long se;
extern long sd;
extern long pcm;

long	inv_g711(), g711(), expand(), subta(), subtb();
long	quan(), adda(), antilog();
long	filtd(), functw();
long	filte(), upa2();
long	mix(), functf(), addb(), addc();
long	floata(), floatb(), fmult();
long	limc(), limb(), limd(), upa1(), calcbn();
long	upa1(), upb(), xor(), lima(), subtc();
long	filta(), filtb(), filtc();
long	compress(), synch();
long	adapt_pred(), inv_adapt_quan(), adapt_quan();
long	quan_scl_fact(), adapt_speed(), output_conv();
long	getSample();

extern struct tbl quan_tbl[];
extern long multbr40_tbl[];
extern long multbr32_tbl[];
extern long multbr24_tbl[];
extern long multbr16_tbl[];
extern long	recon_tbl[];
extern long	mult_tbl[];
extern int br;

long
functw(	I )
long	I;
{
	long is, im, wi;

	if(br==40)
	{
		is = I >> 4;
		im = is ? (31 - I) & 15 : I & 15;
		wi = multbr40_tbl[im];
		return(wi);
	}
	else if(br==32)
	{
		is = I >> 3;
		im = is ? (15 - I) & 7 : I & 7;
		wi = multbr32_tbl[im];
		return(wi);
	}
	else if(br==24)
	{
		is = I >> 2;
		im = is ? (7 - I) & 3 : I & 3;
		wi = multbr24_tbl[im];
		return(wi);
	}
	else if(br==16)
	{
		is = I >> 1;
		im = is ? (3 - I) & 1 : I & 1;
		wi = multbr16_tbl[im];
		return(wi);
	}
}

