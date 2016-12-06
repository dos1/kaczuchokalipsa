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

		ALLEGRO_BITMAP *bg, *circle, *duck, *logo;
		struct Character *hashdash;

		struct Kaczucha kaczuchy[ILOSCKACZUCHUF];

		int transy;
		int outy;

		bool out;

		bool starting;
};

int Gamestate_ProgressCount = 1; // number of loading steps as reported by Gamestate_Load

void buttonledstop(struct Game *game) {
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

void Gamestate_Logic(struct Game *game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	AnimateCharacter(game, data->hashdash, 1);
	data->blink_counter++;
	if (data->blink_counter >= 60) {
		data->blink_counter = 0;
	}

	data->transy+=1;
	if (data->out) {
		data->outy++;
	}

	for (int i=0; i<ILOSCKACZUCHUF; i++) {
		data->kaczuchy[i].rot+=0.1;

		data->kaczuchy[i].y++;

		if (data->kaczuchy[i].y > 100) {
			if (rand()%5000==0) {
				if (!data->out) {
					data->kaczuchy[i].y = -20;
				}
			}
		}
	}

	if (data->out) {
		if (data->outy > 150) {
			if (data->starting) {
				StopCurrentGamestate(game);
				StartGamestate(game, "players");
			} else {
				UnloadAllGamestates(game);
			}
		}
	}

	data->position++;
	if (data->position > 400) data->position = 0;
data->counter++;

if (!(data->out)) {

if (data->blinker==60) {
#ifdef WITH_USBBUTTON

	for (int i=0; i<game->data->buttons; i++) {

		if (data->current==0) {
			if ((i!=game->data->buttons-1) && (i!=data->current)) {
				continue;
}
		} else if ((i!=data->current) && (i!=data->current-1)) {
			  continue;
		  }

	unsigned char barray[USBBTN_SIZE];
	randomButtonColor(&barray, data->current == i ? 255 : 0, 0, 16, i);

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
	data->current ++;
	if (data->current >= game->data->buttons) {
		data->current = 0;
	}
	data->blinker = 0;
}
data->blinker++;
}
}

void Gamestate_Draw(struct Game *game, struct GamestateResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.

	al_draw_scaled_bitmap(data->bg, 0, 0, al_get_bitmap_width(data->bg), al_get_bitmap_height(data->bg), 0, 0, game->viewport.width, game->viewport.height, 0);

	al_draw_scaled_bitmap(data->logo, 0, 0, al_get_bitmap_width(data->logo), al_get_bitmap_height(data->logo),
	                      (game->viewport.width - (game->viewport.width*0.926875))/2.0, game->viewport.height*0.08 + (sin(data->counter/8.0) * game->viewport.height * 0.03), game->viewport.width*0.926875, game->viewport.height*0.156111, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025 + (4*game->viewport.width/4.0), game->viewport.height*0.65 - (fabs(sin((data->counter+2)/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin((data->counter)/16.0)/4.0, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025 + (3*game->viewport.width/4.0), game->viewport.height*0.65 - (fabs(sin(data->counter/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin((data->counter+1)/16.0)/4.0, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025 + (2*game->viewport.width/4.0), game->viewport.height*0.65 - (fabs(sin((data->counter+6.2)/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin((data->counter+42)/16.0)/4.0, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025 + (1*game->viewport.width/4.0), game->viewport.height*0.65 - (fabs(sin((data->counter+9.213)/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin((data->counter+3.666)/16.0)/4.0, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025, game->viewport.height*0.65 - (fabs(sin((data->counter+2)/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin(data->counter/16.0)/4.0, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025 - game->viewport.width/4.0, game->viewport.height*0.65 - (fabs(sin(data->counter/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin((data->counter+1)/16.0)/4.0, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025 - (2*game->viewport.width/4.0), game->viewport.height*0.65 - (fabs(sin((data->counter+6.2)/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin((data->counter+42)/16.0)/4.0, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025 - (3*game->viewport.width/4.0), game->viewport.height*0.65 - (fabs(sin((data->counter+9.213)/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin((data->counter+3.666)/16.0)/4.0, 0);

	al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
	                              data->position*game->viewport.width*0.0025 - (4*game->viewport.width/4.0), game->viewport.height*0.65 - (fabs(sin((data->counter+2)/16.0)) * game->viewport.height*0.06),
	                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck), (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck),
	                              sin(data->counter/16.0)/4.0, 0);


	al_draw_scaled_bitmap(data->circle, 0, 0, al_get_bitmap_width(data->circle), al_get_bitmap_height(data->circle),
	                      0, game->viewport.height-game->viewport.height*0.247222,
	                      game->viewport.width*0.1390625, game->viewport.height*0.247222, 0);

/*	al_draw_scaled_bitmap(data->hashdash->bitmap, 0, 0, al_get_bitmap_width(data->hashdash->bitmap), al_get_bitmap_height(data->hashdash->bitmap),
												0, 0,
												0.06*game->viewport.width, 0.10666*game->viewport.height, 0);

*/	DrawScaledCharacter(game, data->hashdash, al_map_rgba(255,255,255,255),
	                    (game->viewport.width*0.06)/192.0, (game->viewport.height*0.10666)/192.0,
	                    0);

	al_draw_filled_rectangle(0,  game->viewport.height*data->transy/100.0, game->viewport.width, game->viewport.height, !game->data->firsttime ? al_map_rgb(65,54,92) : al_map_rgb(0,0,0));

	if (!game->data->firsttime) {
		al_draw_bitmap_region(game->data->winbitmap, 0, fmax(0, game->viewport.height*data->transy/100.0), game->viewport.width, game->viewport.height, 0,  fmax(0, game->viewport.height*data->transy/100.0), 0);
	}

	if (data->out) {
		al_draw_filled_rectangle(0,  0, game->viewport.width, game->viewport.height*data->outy/100.0, data->starting ? al_map_rgb(65,54,92) : al_map_rgb(0,0,0));
	}

	for (int i=0; i<ILOSCKACZUCHUF; i++) {

		al_draw_scaled_rotated_bitmap(data->duck, al_get_bitmap_width(data->duck)/2, al_get_bitmap_height(data->duck)/2,
		                              data->kaczuchy[i].x*game->viewport.width/100.0, game->viewport.height*(data->kaczuchy[i].y)/100.0,
		                              (game->viewport.width*0.32)/(float)al_get_bitmap_width(data->duck) / 3.0, (game->viewport.height*0.56888)/(float)al_get_bitmap_height(data->duck) / 3.0,
		                              data->kaczuchy[i].rot * 1.0, 0);


	}

}

void Gamestate_ProcessEvent(struct Game *game, struct GamestateResources* data, ALLEGRO_EVENT *ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_TILDE)) { return; }
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		if (!data->starting) {
		if (data->out) {
			UnloadCurrentGamestate(game); // mark this gamestate to be stopped and unloaded
		} else {
			al_stop_sample_instance(data->music);
			al_play_sample_instance(data->music);
			data->out = true;
			buttonledstop(game);

			for (int i=0; i<ILOSCKACZUCHUF; i++) {
				if ((data->kaczuchy[i].y > 110) || (rand() % 4 == 0)) {
					data->kaczuchy[i].x = rand() % 101;
					data->kaczuchy[i].y = rand() % 31 - 15;
					data->kaczuchy[i].dx = rand() % 4 - 2;
					data->kaczuchy[i].rot = rand() % 20;
					if (rand()%5 == 0) {
						data->kaczuchy[i].y-= rand() % 20;
					}
					if (rand()%6 == 0) {
						data->kaczuchy[i].y+= rand() % 20;
					}
					data->kaczuchy[i].y -= 50;
				}
			}

		}
		}
		// When there are no active gamestates, the engine will quit.
		return;
	}

if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_F)) {
	game->config.fullscreen = !game->config.fullscreen;
	if (game->config.fullscreen) {
		      SetConfigOption(game, "SuperDerpy", "fullscreen", "1");
					al_hide_mouse_cursor(game->display);
	} else {
		      SetConfigOption(game, "SuperDerpy", "fullscreen", "0");
					al_show_mouse_cursor(game->display);
	}
	al_set_display_flag(game->display, ALLEGRO_FULLSCREEN_WINDOW, game->config.fullscreen);
	SetupViewport(game, game->viewport_config);
	PrintConsole(game, "Fullscreen toggled");
	al_destroy_bitmap(game->data->winbitmap);
	game->data->winbitmap = al_create_bitmap(game->viewport.width, game->viewport.height);
return;}

  if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) || (ev->type==ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) || (ev->type==ALLEGRO_EVENT_TOUCH_BEGIN) ||  (ev->type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)) {

		if ((!data->starting) && (!data->out)) {

			//			game->data->berek = ev->keyboard.keycode - ALLEGRO_KEY_1;
			al_stop_sample_instance(data->music);
			al_play_sample_instance(data->music);

data->starting = true;
      data->out = true;
buttonledstop(game);
      for (int i=0; i<ILOSCKACZUCHUF; i++) {
				if ((data->kaczuchy[i].y > 110) || (rand() % 4 == 0)) {
					data->kaczuchy[i].x = rand() % 101;
					data->kaczuchy[i].y = rand() % 31 - 15;
					data->kaczuchy[i].dx = rand() % 4 - 2;
					data->kaczuchy[i].rot = rand() % 20;
					if (rand()%5 == 0) {
						data->kaczuchy[i].y-= rand() % 20;
					}
					if (rand()%6 == 0) {
						data->kaczuchy[i].y+= rand() % 20;
					}
					data->kaczuchy[i].y -= 50;
				}
			}

		}
	  }

}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct GamestateResources *data = malloc(sizeof(struct GamestateResources));
	data->font = al_create_builtin_font();

	data->bg = al_load_bitmap(GetDataFilePath(game, "bg.png"));
	data->logo = al_load_bitmap(GetDataFilePath(game, "logo.png"));
	data->duck = al_load_bitmap(GetDataFilePath(game, "duck.png"));
	data->circle = al_load_bitmap(GetDataFilePath(game, "circle.png"));

	data->hashdash = CreateCharacter(game, "hashdash");
	RegisterSpritesheet(game, data->hashdash, "blink");
	LoadSpritesheets(game, data->hashdash);
	SelectSpritesheet(game, data->hashdash, "blink");
	SetCharacterPositionF(game, data->hashdash, 0.02, 0.85, 0);

	data->musicsample = al_load_sample( GetDataFilePath(game, "kwa.wav") );
	data->music = al_create_sample_instance(data->musicsample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.fx);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_ONCE);


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

	for (int i=0; i<10; i++) {
		game->data->players[i].active = false;
		game->data->players[i].offtime = 0;
	}

	al_stop_sample_instance(game->data->music);
	al_play_sample_instance(game->data->music);
	al_play_sample_instance(data->music);
	al_set_sample_instance_gain(game->data->music, 0.8);
	data->blink_counter = 0;
	data->counter = 0;
	data->starting = false;
	data->position = 0;
	data->blinker = 60;
	//data->kaczuchy = NULL;
	for (int i=0; i<ILOSCKACZUCHUF; i++) {
		data->kaczuchy[i].x = rand() % 101;
		data->kaczuchy[i].y = rand() % 31 - 15;
		data->kaczuchy[i].dx = rand() % 4 - 2;
		data->kaczuchy[i].rot = rand() % 20;
		if (rand()%5 == 0) {
			data->kaczuchy[i].y-= rand() % 20;
		}
		if (rand()%6 == 0) {
			data->kaczuchy[i].y+= rand() % 20;
		}
		data->kaczuchy[i].y -= 50;
	}
	data->transy = -50;
	data->outy = -50;

	data->out = false;
	data->current = 0;


#ifdef WITH_USBBUTTON

	for (int i=0; i<game->data->buttons; i++) {


	unsigned char barray[USBBTN_SIZE];
	randomButtonColor(&barray, 0, 0, 16, i);

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

void Gamestate_Stop(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
	game->data->firsttime = false;
	buttonledstop(game);
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
