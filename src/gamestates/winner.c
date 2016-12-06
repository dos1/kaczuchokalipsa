/*! \file empty.c
 *  \brief Empty gamestate.
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

#include "../common.h"
#include "strings.h"
#ifdef WANT_USBBUTTON
#include "../libs/ulboard.h"
#include "../libs/common.h"
#endif
#include <libsuperderpy.h>

struct Kaczucha;

struct Kaczucha {
		int x, y, dx;
		float rot;
};

#define ILOSCKACZUCHUF 100

struct GamestateResources {
		// This struct is for every resource allocated and used by your gamestate.
		// It gets created on load and then gets passed around to all other function calls.
		ALLEGRO_FONT *font;
		int blink_counter;
		ALLEGRO_SAMPLE *musicsample;
		ALLEGRO_SAMPLE_INSTANCE *music;
		int counter;
		int position;

		int current;

		int blinker;
ALLEGRO_FONT *scorefont;
    ALLEGRO_BITMAP *bg, *circle, *duck, *logo;
		struct Character *hashdash;

		struct Kaczucha kaczuchy[ILOSCKACZUCHUF];

		int transy;
		int outy;

		bool out;

		bool starting;
};

int Gamestate_ProgressCount = 1; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.


data->counter++;

}

void Gamestate_Draw(struct Game *game, struct GamestateResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.
al_set_target_bitmap(game->data->winbitmap);
  al_clear_to_color(al_map_rgb(65,54,92));

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              game->viewport.width*0.02, game->viewport.height*0.65 - (fabs(sin((data->counter+2)/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin((data->counter)/16.0)/4.0, 0);

	char timeleft[10];
	    snprintf(&timeleft, 10, "%d", game->data->berek+1);

			DrawTextWithShadow(data->scorefont, al_map_rgb(255,255,255), game->viewport.width*0.7, game->viewport.height*0.2, ALLEGRO_ALIGN_CENTER, timeleft);

al_set_target_backbuffer(game->display);
al_draw_bitmap(game->data->winbitmap, 0, 0, 0);

}

void Gamestate_ProcessEvent(struct Game *game, struct GamestateResources* data, ALLEGRO_EVENT *ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		  StopCurrentGamestate(game);
			StartGamestate(game, "empty"); // mark this gamestate to be stopped and unloaded
			  // When there are no active gamestates, the engine will quit.
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_TILDE)) { return; }
if ((((ev->type==ALLEGRO_EVENT_KEY_DOWN) || (ev->type==ALLEGRO_EVENT_TOUCH_BEGIN) || (ev->type==ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)) || (ev->type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)) && (data->counter > 3*60)) {
	StopCurrentGamestate(game);
	StartGamestate(game, "empty"); // mark this gamestate to be stopped and unloaded
}
/*
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) &&
		 ( (ev->keyboard.keycode == ALLEGRO_KEY_1) ||
			 (ev->keyboard.keycode == ALLEGRO_KEY_2) ||
			 (ev->keyboard.keycode == ALLEGRO_KEY_3) ||
			 (ev->keyboard.keycode == ALLEGRO_KEY_4) ||
			 (ev->keyboard.keycode == ALLEGRO_KEY_5) ||
			 (ev->keyboard.keycode == ALLEGRO_KEY_6) ||
			 (ev->keyboard.keycode == ALLEGRO_KEY_7) ||
			 (ev->keyboard.keycode == ALLEGRO_KEY_8) ||
			 (ev->keyboard.keycode == ALLEGRO_KEY_9)
			)) {

		SwitchCurrentGamestate(game, "empty");
	}
*/
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct GamestateResources *data = malloc(sizeof(struct GamestateResources));
	data->font = al_create_builtin_font();

	data->bg = al_load_bitmap(GetDataFilePath(game, "bg.png"));
	data->duck = al_load_bitmap(GetDataFilePath(game, "king2.png"));

	data->musicsample = al_load_sample( GetDataFilePath(game, "okalsky.wav") );
	data->music = al_create_sample_instance(data->musicsample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.fx);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_ONCE);
	data->scorefont = al_load_font(GetDataFilePath(game, "fonts/chlorinuh.ttf"), game->viewport.height*0.5, 0);

	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar
	return data;
}

void Gamestate_Unload(struct Game *game, struct GamestateResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	al_destroy_font(data->font);
	free(data);
}

void Gamestate_Start(struct Game *game, struct GamestateResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.
	PrintConsole(game, "WINNER %d", game->data->berek);

	data->scorefont = al_load_font(GetDataFilePath(game, "fonts/chlorinuh.ttf"), game->viewport.height*0.5, 0);
	data->counter=0;
	al_play_sample_instance(data->music);
#ifdef WITH_USBBUTTON

	for (int i=0; i<game->data->buttons; i++) {


	unsigned char barray[USBBTN_SIZE];
	if (i==game->data->berek) {
		randomButtonColor(&barray, 255,255,255, i);

	} else {
		randomButtonColor(&barray, 0, 0, 1, i);
	}

	unsigned char mesg[USBBTN_MESG_LENGTH] = {0,0,0,0};
	int pos = 0;

	while (pos < USBBTN_SIZE)
	{
		memcpy(&mesg[0], &barray[pos], 4);

		libusb_control_transfer(game->data->handle[i],
		                              UM_REQUEST_TYPE,
		                              UM_REQUEST,
		                              USBBTN_VALUE,
		                              USBBTN_INTERFACE,
		                              mesg,
		                              USBBTN_MESG_LENGTH,
		                              UM_TIMEOUT);

		pos+=USBBTN_MESG_LENGTH;
	}

	pos = 0;

}
#endif
	data->blinker=0;

}

void Gamestate_Stop(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
#ifdef WITH_USBBUTTON

	for (int i=0; i<game->data->buttons; i++) {


	unsigned char barray[USBBTN_SIZE];
	randomButtonColor(&barray, 0, 0, 0, i);

	unsigned char mesg[USBBTN_MESG_LENGTH] = {0,0,0,0};
	int pos = 0;

	while (pos < USBBTN_SIZE)
	{
		memcpy(&mesg[0], &barray[pos], 4);

		libusb_control_transfer(game->data->handle[i],
		                              UM_REQUEST_TYPE,
		                              UM_REQUEST,
		                              USBBTN_VALUE,
		                              USBBTN_INTERFACE,
		                              mesg,
		                              USBBTN_MESG_LENGTH,
		                              UM_TIMEOUT);

		pos+=USBBTN_MESG_LENGTH;
	}

	pos = 0;

}
#endif
}

void Gamestate_Pause(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets paused (so only Draw is being called, no Logic not ProcessEvent)
	// Pause your timers here.
}

void Gamestate_Resume(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets resumed. Resume your timers here.
}

// Ignore this for now.
// TODO: Check, comment, refine and/or remove:
void Gamestate_Reload(struct Game *game, struct GamestateResources* data) {}
