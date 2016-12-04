#define LIBSUPERDERPY_DATA_TYPE struct CommonResources
#include <libsuperderpy.h>
#include "libs/common.h"
#include "libs/ulboard.h"
#include <libusb.h>
#include <json.h>

struct Player {
	bool active;
	float x;
	float y;
	float rot;
	bool berek;
};

struct CommonResources {
		// Fill in with common data accessible from all gamestates.
		struct libusb_device_handle *handle[1024];
		int buttons;

		struct Player players[10];
};

struct CommonResources* CreateGameData(struct Game *game);
void DestroyGameData(struct Game *game, struct CommonResources *data);
struct libusb_device_handle* openUSB(libusb_context *ctx, uint16_t vendor, uint16_t product, int interface, int autoconnect, int id);
void closeUSB(libusb_context *ctx, struct libusb_device_handle *handle, int interface);

bool writeUSBButton(unsigned char* barray, int autoconnect, bool transfer);
bool validateUSBButtonData(json_object* jobj, ulboard* board);
bool validateUSBButtonRowData(json_object* entries, const char* rowStr, bool curResult);
void populateUSBKeys(json_object* keys, int row, unsigned char* barray);

#define USBBTN_VENDOR  0xD209
#define USBBTN_PRODUCT 0x1200
#define USBBTN_INTERFACE 0
#define USBBTN_VALUE 0x0200
#define USBBTN_MESG_LENGTH 4
#define USBBTN_SIZE 64
