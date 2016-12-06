/*
 ============================================================================
 Name        : pacseries.c
 Author      : Katie Snow
 Version     :
 Copyright   : Copyright 2014 Robert Abram, Katie Snow
 Description : Ultimarc 2015 PAC board configuration library
 ============================================================================
 */

/* C */
#include <stdio.h>
#include <string.h>

/* Unix */
#include <libusb-1.0/libusb.h>

/* Local */
#include "common.h"
#include "dbg.h"
#include "ipacseries.h"

/* Array row order
1up, 1dn, 1rt, 1lt, 2up, 2dn, 2rt, 2lt, 3up, 3dn, 3rt, 3lt, 4up, 4dn, 4rt, 4lt,
1s1, 1s2, 1s3, 1s4, 1s5, 1s6, 1s7, 1s8, 2s1, 2s2, 2s3, 2s4, 2s5, 2s6, 2s7, 2s8,
3s1, 3s2, 3s3, 3s4, 3s5, 3s6, 3s7, 3s8, 4s1, 4s2, 4s3, 4s4, 4s5, 4s6, 4s7, 4s8,
1start, 1coin, 1a, 1b, 2start, 2coin, 2a, 2b, 3start, 3coin, 4start, 4coin,
1test, 2test, service */

/* Normal key press value */
int keyLookupTable[8][63] = {
/* Pre2015 IPAC2 */
{1, 6, 2, 4, 13, 14, 9, 11, -1, -1, -1, -1, -1, -1, -1, -1, 3, 8, 5, 10, 7, 12, 24, 26,
 15, 17, 19, 21, 23, 25, 27, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, 16, 20, 29, 30, 18, 22, 31, 32, -1, -1, -1, -1, -1, -1, -1},

/* Pre2015 IPAC4 */
{1, 6, 2, 4, 13, 14, 9, 11, 101, 106, 102, 104, 113, 114, 109, 111, 3, 8, 5, 10, 7, 12,
 24, 26, 15, 17, 19, 21, 23, 25, 27, 28, 103, 108, 105, 110, 107, 112, 124, 126, 115,
 117, 119, 121, 123, 125, 127, 128, 16, 20, -1, -1, 18, 22, -1, -1, 116, 120, 118, 122,
 -1, -1, -1},

/* Ultimate I/O */
{5, 7, 1, 3, 28, 26, 32, 30, 17, 19, 23, 23, 31, 29, 10, 9, 8, 6, 4, 2, 24, 22, 20, 18,
 48, 46, 44, 42, 12, 50, 49, 11, 37, 39, 33, 35, -1, -1, -1, -1, 27, 25, 47, 45, -1,
 -1, -1, -1, 40, 36, 13, 15, 38, 34, 41, 43, -1, -1, -1, -1, -1, -1, -1},

/* 2015 IPAC2 */
{20, 18, 24, 22, 21, 19, 1, 23, -1, -1, -1, -1, -1, -1, -1, -1, 40, 38, 36, 34, 32, 30,
 28, 26, 17, 39, 37, 35, 33, 29, 27, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, 48, 46, 44, 42, 47, 45, 43, 41, -1, -1, -1, -1, -1, -1, -1},

/* 2015 MinIPAC */
{11, 9, 15, 13, 38, 40, 34, 36, -1, -1, -1, -1, -1, -1, -1, -1, 10, 12, 14, 16, 42, 44,
 46, 48, 18, 20, 22, 24, 2, 4, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, 26, 30, 7, 5, 28, 32, 3, 1, -1, -1, -1, -1, -1, -1, -1},

/* 2015 IPAC4 */
{16, 14, 20, 18, 13, 11, 17, 15, 48, 38, 47, 40, 37, 35, 39, 45, 12, 10, 32, 30, 28, 53,
 64, 56, 9, 31,29, 27, 60, 61, 49, 57, 36, 34, 8, 6, 4, 2, 50, 58, 33, 7, 5, 3, 1, 23,
 59, 51, 62, 54, -1, -1, 55, 63, -1, -1, 24, 22, 21, 19, -1, -1, -1},

/* 2015 JPAC */
{25, 29, 37, 33, 27, 31, 39, 35, -1, -1, -1, -1, -1, -1, -1, -1, 17, 21, 1, 5, 20, 18,
 40, 38, 19, 23, 3, 7, 32, 30, 34, 36, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, 45, 41, -1, -1, 47, 43, -1, -1, -1, -1, -1, -1, 13, 15, 11},

 /* Pre2015 MINIPAC */
{1, 6, 2, 4, 13, 14, 9, 11, -1, -1, -1, -1, -1, -1, -1, -1, 3, 8, 5, 10, 7, 12, 24, 26,
 15, 17, 19, 21, 23, 25, 27, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, 16, 20, 29, 30, 18, 22, 31, 32, -1, -1, -1, -1, -1, -1, -1},

/* 2015 HIDIO {} */
};

int shiftAdjTable[] = {32, 28, 50, 50, 50, 64, 50, 32, -1};
int shiftPosAdjTable[] = {-1, -1, 100, 100, 100, 128, 100, -1, -1};


/* Only the pre2015 boards will use this lookup array */
int macroLookupTable[8][4] = {
/* Pre2015 IPAC2 */
{63, 67, 71, 75},

/* Pre2015 IPAC4 */
{59, 63, 67, 71},

/* Ultimate I/O */
{-1, -1, -1, -1},

/* 2015 IPAC2 */
{-1, -1, -1, -1},

/* 2015 MinIPAC */
{-1, -1, -1, -1},

/* 2015 IPAC4 */
{-1, -1, -1, -1},

/* 2015 JPAC */
{-1, -1, -1, -1},

 /* Pre2015 MINIPAC */
{63, 67, 71, 75}

/* 2015 HIDIO {} */
};


int decipherLookupKey (const char* key)
{
	int lkey = -1;

	if (!strcasecmp(key, "1up"))
		lkey = 0;
	if (!strcasecmp(key, "1down"))
		lkey = 1;
	if (!strcasecmp(key, "1right"))
		lkey = 2;
	if (!strcasecmp(key, "1left"))
		lkey = 3;
	if (!strcasecmp(key, "2up"))
		lkey = 4;
	if (!strcasecmp(key, "2down"))
		lkey = 5;
	if (!strcasecmp(key, "2right"))
		lkey = 6;
	if (!strcasecmp(key, "2left"))
		lkey = 7;
	if (!strcasecmp(key, "3up"))
		lkey = 8;
	if (!strcasecmp(key, "3down"))
		lkey = 9;
	if (!strcasecmp(key, "3right"))
		lkey = 10;
	if (!strcasecmp(key, "3left"))
		lkey = 11;
	if (!strcasecmp(key, "4up"))
		lkey = 12;
	if (!strcasecmp(key, "4down"))
		lkey = 13;
	if (!strcasecmp(key, "4right"))
		lkey = 14;
	if (!strcasecmp(key, "4left"))
		lkey = 15;
	if (!strcasecmp(key, "1sw1"))
		lkey = 16;
	if (!strcasecmp(key, "1sw2"))
		lkey = 17;
	if (!strcasecmp(key, "1sw3"))
		lkey = 18;
	if (!strcasecmp(key, "1sw4"))
		lkey = 19;
	if (!strcasecmp(key, "1sw5"))
		lkey = 20;
	if (!strcasecmp(key, "1sw6"))
		lkey = 21;
	if (!strcasecmp(key, "1sw7"))
		lkey = 22;
	if (!strcasecmp(key, "1sw8"))
		lkey = 23;
	if (!strcasecmp(key, "2sw1"))
		lkey = 24;
	if (!strcasecmp(key, "2sw2"))
		lkey = 25;
	if (!strcasecmp(key, "2sw3"))
		lkey = 26;
	if (!strcasecmp(key, "2sw4"))
		lkey = 27;
	if (!strcasecmp(key, "2sw5"))
		lkey = 28;
	if (!strcasecmp(key, "2sw6"))
		lkey = 29;
	if (!strcasecmp(key, "2sw7"))
		lkey = 30;
	if (!strcasecmp(key, "2sw8"))
		lkey = 31;
	if (!strcasecmp(key, "3sw1"))
		lkey = 32;
	if (!strcasecmp(key, "3sw2"))
		lkey = 33;
	if (!strcasecmp(key, "3sw3"))
		lkey = 34;
	if (!strcasecmp(key, "3sw4"))
		lkey = 35;
	if (!strcasecmp(key, "3sw5"))
		lkey = 36;
	if (!strcasecmp(key, "3sw6"))
		lkey = 37;
	if (!strcasecmp(key, "3sw7"))
		lkey = 38;
	if (!strcasecmp(key, "3sw8"))
		lkey = 39;
	if (!strcasecmp(key, "4sw1"))
		lkey = 40;
	if (!strcasecmp(key, "4sw2"))
		lkey = 41;
	if (!strcasecmp(key, "4sw3"))
		lkey = 42;
	if (!strcasecmp(key, "4sw4"))
		lkey = 43;
	if (!strcasecmp(key, "4sw5"))
		lkey = 44;
	if (!strcasecmp(key, "4sw6"))
		lkey = 45;
	if (!strcasecmp(key, "4sw7"))
		lkey = 46;
	if (!strcasecmp(key, "4sw8"))
		lkey = 47;
	if (!strcasecmp(key, "1start"))
		lkey = 48;
	if (!strcasecmp(key, "1coin"))
		lkey = 49;
	if (!strcasecmp(key, "1a"))
		lkey = 50;
	if (!strcasecmp(key, "1b"))
		lkey = 51;
	if (!strcasecmp(key, "2start"))
		lkey = 52;
	if (!strcasecmp(key, "2coin"))
		lkey = 53;
	if (!strcasecmp(key, "2a"))
		lkey = 54;
	if (!strcasecmp(key, "2b"))
		lkey = 55;
	if (!strcasecmp(key, "3start"))
		lkey = 56;
	if (!strcasecmp(key, "3coin"))
		lkey = 57;
	if (!strcasecmp(key, "4start"))
		lkey = 58;
	if (!strcasecmp(key, "4coin"))
		lkey = 59;
	if (!strcasecmp(key, "1test"))
		lkey = 60;
	if (!strcasecmp(key, "2test"))
		lkey = 61;
	if (!strcasecmp(key, "service"))
		lkey = 62;

	if (lkey == -1)
	{
		log_info("Unable to decipher pin '%s'.", key);
	}

	return lkey;
}

int decipherLookupMacroKey (const char* key)
{
	int lkey = -1;

	if (!strcasecmp(key, "m1"))
		lkey = 0;
	if (!strcasecmp(key, "m2"))
		lkey = 1;
	if (!strcasecmp(key, "m3"))
		lkey = 2;
	if (!strcasecmp(key, "m4"))
		lkey = 3;

	if (lkey == -1)
	{
		log_info("Unable to decipher macro '%s'.", key);
	}
	return lkey;
}

bool writeIPACSeriesUSB (unsigned char* barray, int size, uint16_t vendor, uint16_t product, int interface, int autoconnect, bool transfer)
{
	libusb_context *ctx = NULL;
	struct libusb_device_handle *handle = NULL;
	unsigned char mesg[IPACSERIES_MESG_LENGTH] = {0x03,0,0,0,0};

	bool result = true;

	int pos = 0;
	int ret = 0;

	if (transfer)
	{
		handle = openUSB(ctx, vendor, product, interface, autoconnect, 0);

		if (!handle)
		{
			result = false;
			goto error;
		}
	}

	while (pos < size)
	{
		memcpy(&mesg[1], &barray[pos], 4);

		debug ("Writing (%i): %x, %x, %x, %x", pos, mesg[1], mesg[2], mesg[3], mesg[4]);
		if (transfer)
		{
			ret = libusb_control_transfer(handle,
			                              UM_REQUEST_TYPE,
			                              UM_REQUEST,
			                              IPACSERIES_VALUE,
			                              interface,
			                              mesg,
			                              IPACSERIES_MESG_LENGTH,
			                              UM_TIMEOUT);
			debug ("Write result: %i", ret);
		}
		pos+=4;
	}

exit:
	if (transfer)
	{
		closeUSB(ctx, handle, interface);
	}
	else
	{
		log_info ("board array was not written out!!!");
	}
	return result;

error:
	return result;
}
