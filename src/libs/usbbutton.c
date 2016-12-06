/*
 ============================================================================
 Name        : usbbutton.c
 Author      : Katie Snow
 Version     :
 Copyright   : Copyright 2014 Robert Abram, Katie Snow
 Description : USBButton configuration library
 ============================================================================
 */

/* C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/* Unix */
#include <libusb-1.0/libusb.h>

/* Local */
#include "ulboard.h"
#include "common.h"
#include "ipacseries.h"
#include "usbbutton.h"
#include "dbg.h"



#define USBBTN_ROW_SIZE 6

int usbKeyLookupTable[8][6] = {
/* Primary */
/* Row 1 */
{10, 11, 12, 13, 14, 15},
/* Row 2 */
{16, 17, 18, 19, 20, 21},
/* Row 3 */
{22, 23, 24, 25, 26, 27},
/* Row 4 */
{28, 29, 30, 31, 32, 33},

/* Secondary */
/* Row 1 */
{34, 35, 36, 37, 38, 39},
/* Row 2 */
{40, 41, 42, 43, 44, 45},
/* Row 3 */
{46, 47, 48, 49, 50, 51},
/* Row 4 */
{52, 53, 54, 55, 56, 57}

};


bool randomButtonColor (unsigned char* barray, int r, int g, int b, int id)
{
	bool result = false;

	const char* str = NULL;

	memset (barray, 0, sizeof(*barray)*USBBTN_SIZE);

	/* Header */
	barray[0] = 0x50;
	barray[1] = 0xdd;

	/* Action */
	barray[2] = 2; // alternate; 1 - extended; 2 - both

	/* Release Colors */
	barray[4] = r; //red
	barray[5] = g; //green
	barray[6] = b; //blue

	/* Press Colors */
	barray[7] = r; //red
	barray[8] = g; //green
	barray[9] = b; //blue


	/* Primary Keys*/
	populateBlankKeys(0, barray);
	populateBlankKeys(1, barray);
	populateBlankKeys(2, barray);
	populateBlankKeys(3, barray);
	barray[usbKeyLookupTable[0][0]] = 0x1e + id;


	/* Secondary Keys */
	populateBlankKeys(4, barray);
	populateBlankKeys(5, barray);
	populateBlankKeys(6, barray);
	populateBlankKeys(7, barray);
	barray[usbKeyLookupTable[4][0]] = 0x00;

	//result = writeUSBButton(barray, 1, true);

	return result;
}



void populateBlankKeys(int row, unsigned char* barray)
{
	int pos = 0;
	for(; pos < 6; ++pos)
	{
		barray[usbKeyLookupTable[row][pos]] = 0x00;
	}
	if ((row==0) || (row==4)) {
	barray[usbKeyLookupTable[row][0]] = 0x22;
	}
}

bool writeUSBButton(unsigned char* barray, int autoconnect, bool transfer)
{
	libusb_context *ctx = NULL;
	struct libusb_device_handle *handle = NULL;
	unsigned char mesg[USBBTN_MESG_LENGTH] = {0,0,0,0};

	bool result = true;

	int pos = 0;
	int ret = 0;

	if (transfer)
	{
		handle = openUSB(ctx, USBBTN_VENDOR, USBBTN_PRODUCT, USBBTN_INTERFACE, autoconnect, 0);

		if (!handle)
		{
			result = false;
			goto error;
		}
	}

	while (pos < USBBTN_SIZE)
	{
		memcpy(&mesg[0], &barray[pos], 4);

		debug ("Writing (%i): %x, %x, %x, %x", pos, mesg[0], mesg[1], mesg[2], mesg[3]);
		if (transfer)
		{
			ret = libusb_control_transfer(handle,
			                              UM_REQUEST_TYPE,
			                              UM_REQUEST,
			                              USBBTN_VALUE,
			                              USBBTN_INTERFACE,
			                              mesg,
			                              USBBTN_MESG_LENGTH,
			                              UM_TIMEOUT);
			debug ("Write result: %i", ret);
		}
		pos+=USBBTN_MESG_LENGTH;
	}

exit:
	if (transfer)
	{
		closeUSB(ctx, handle, USBBTN_INTERFACE);
	}
	else
	{
		log_info ("board array was not written out!!!");
	}
	return result;

error:
	return result;
}
