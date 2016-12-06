#define LIBSUPERDERPY_DATA_TYPE struct CommonResources
#include <libsuperderpy.h>
#ifdef WITH_USBBUTTON
#include "libs/common.h"
#include "libs/ulboard.h"
#include <libusb.h>
#endif

struct Player {
	bool active;
	float x;
	float y;
	float rot;
	bool rotating;
	bool berek;
	int offtime;

	int keycode;
	ALLEGRO_JOYSTICK *device;
};

struct CommonResources {
		// Fill in with common data accessible from all gamestates.
		struct libusb_device_handle *handle[10];
		int buttons;
		bool firsttime;

		struct Player players[1024];
		int activeplayers;
		int berek;

		ALLEGRO_SAMPLE *musicsample;
		ALLEGRO_SAMPLE_INSTANCE *music;

};

struct CommonResources* CreateGameData(struct Game *game);
void DestroyGameData(struct Game *game, struct CommonResources *data);

#define USBBTN_VENDOR  0xD209
#define USBBTN_PRODUCT 0x1200
#define USBBTN_INTERFACE 0
#define USBBTN_VALUE 0x0200
#define USBBTN_MESG_LENGTH 4
#define USBBTN_SIZE 64
