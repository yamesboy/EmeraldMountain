/* Copyright (c) 2012, Texas Instruments Incorporated
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

*  Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

*  Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

*  Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include <ti/grlib/grlib.h>

static const unsigned char pixel_Miner00004BPP_UNCOMP[] =
{
0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 
0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 
0x33, 0x33, 0x30, 0x33, 0x33, 0x33, 
0x33, 0x33, 0x33, 0x33, 0x11, 0x13, 
0x33, 0xee, 0x03, 0x33, 0x33, 0x11, 
0x33, 0x30, 0x03, 0x33, 0x31, 0x31, 
0x33, 0xe3, 0x3e, 0x33, 0x13, 0x31, 
0x00, 0xe2, 0x2e, 0x01, 0x33, 0x33, 
0x33, 0x22, 0x22, 0x33, 0x33, 0x33, 
0x32, 0x23, 0x32, 0x23, 0x33, 0x33, 
0x32, 0x23, 0x32, 0x23, 0x33, 0x33, 
0x33, 0x33, 0x33, 0x33, 0x33, 0x33
};

static const unsigned long palette_Miner00004BPP_UNCOMP[]=
{
	0xfdd14d, 	0x212d4d, 	0x023ada, 	0x572f17, 
	0x000000, 	0x000000, 	0x000000, 	0x000000, 
	0x000000, 	0x000000, 	0x000000, 	0x000000, 
	0x000000, 	0x000000, 	0x000000, 	0xffffff
};

const tImage  Miner00004BPP_UNCOMP=
{
	IMAGE_FMT_4BPP_UNCOMP,
	12,
	12,
	16,
	palette_Miner00004BPP_UNCOMP,
	pixel_Miner00004BPP_UNCOMP,
};

