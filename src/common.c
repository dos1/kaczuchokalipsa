/*! \file common.c
 *  \brief Common stuff that can be used by all gamestates.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "common.h"
#include "libs/ulboard.h"
#include "strings.h"
#include "libs/common.h"
#include <libsuperderpy.h>

struct CommonResources* CreateGameData(struct Game *game) {
	struct CommonResources *data = calloc(1, sizeof(struct CommonResources));
data->buttons = 0;
libusb_context *ctx = NULL;
data->activeplayers = 0;

struct libusb_device_handle *handle = NULL;
int ret;
/* Open USB communication */
ret = libusb_init(&ctx);
libusb_set_debug(ctx, 3);


libusb_device **list;
libusb_get_device_list(ctx, &list);


int count = libusb_get_device_list(ctx, &list);
struct libusb_device_descriptor desc = {0};
libusb_device *device;

for (size_t idx = 0; idx < count; ++idx) {
	  device = list[idx];

		int rc = libusb_get_device_descriptor(device, &desc);

//		printf("Vendor:Device = %04x:%04x\n", desc.idVendor, desc.idProduct);

		if ((desc.idVendor == USBBTN_VENDOR) && (desc.idProduct == USBBTN_PRODUCT)) {
			data->buttons++;
		}

}




  for (int i=0; i<data->buttons; i++) {
		data->handle[i] = openUSB(ctx, USBBTN_VENDOR, USBBTN_PRODUCT, USBBTN_INTERFACE, true, i);
	}


	data->musicsample = al_load_sample( GetDataFilePath(game, "music.wav") );
	data->music = al_create_sample_instance(data->musicsample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.music);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_LOOP);

	return data;
}

void DestroyGameData(struct Game *game, struct CommonResources *data) {
	free(data);
}


