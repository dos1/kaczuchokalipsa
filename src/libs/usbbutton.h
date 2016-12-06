/*
 ============================================================================
 Name        : usbbutton.h
 Author      : Katie Snow
 Version     :
 Copyright   : Copyright 2015 Robert Abram, Katie Snow
 Description : USB Button configuration library
 ============================================================================
 */

#ifndef USBBUTTON_H_
#define USBBUTTON_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USBBTN_VENDOR  0xD209
#define USBBTN_PRODUCT 0x1200
#define USBBTN_INTERFACE 0
#define USBBTN_VALUE 0x0200
#define USBBTN_MESG_LENGTH 4
#define USBBTN_SIZE 64

typedef struct json_object json_object;
typedef struct ulboard ulboard;

void populateBlankKeys(int row, unsigned char* barray);
bool randomButtonColor (unsigned char* barray, int r, int g, int b, int id);
bool writeUSBButton(unsigned char* barray, int autoconnect, bool transfer);
#ifdef __cplusplus
}
#endif

#endif /* USBBUTTON_H_ */
