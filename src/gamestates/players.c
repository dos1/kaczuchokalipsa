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
#include "../libs/ulboard.h"
#include "strings.h"
#include "math.h"
#include <stdio.h>
#include "../libs/common.h"
#include <libsuperderpy.h>


#define ILOSCKACZUCHUF 100

struct GamestateResources {
		// This struct is for every resource allocated and used by your gamestate.
		// It gets created on load and then gets passed around to all other function calls.
		ALLEGRO_FONT *font, *scorefont;
		int blink_counter;
		int count;
		int counter;

		bool crowntaken;

		int starttimer;
		bool started;
		ALLEGRO_SAMPLE *bamsample;
		ALLEGRO_SAMPLE_INSTANCE *bam;

		bool someone;

		int timer;

		ALLEGRO_BITMAP *kaczka, *kaczka2, *player, *crown;
};

int Gamestate_ProgressCount = 1; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
data->counter++;
  for (int i=0; i<10; i++) {
		if (!game->data->players[i].active) {
			continue;
		}
if (data->started) {
	  game->data->players[i].x += 0.1 * cos(game->data->players[i].rot);
		game->data->players[i].y += 0.1 * sin(game->data->players[i].rot);

		if (game->data->players[i].x < 0) {
			game->data->players[i].x = 100;
		}
		if (game->data->players[i].y < 0) {
			game->data->players[i].y = 100;
		}
		if (game->data->players[i].x > 100) {
			game->data->players[i].x = 0;
		}
		if (game->data->players[i].y > 100) {
			game->data->players[i].y = 0;
		}

		if (game->data->players[i].rotating) {
			game->data->players[i].rot-=0.015;
		}
  }
}
	if (data->count%60==0) {
		if (!data->started) {
			data->started = data->starttimer==0;
		if (data->starttimer==0) {
			data->timer = 60;
			data->crowntaken=false;

		} else {
			if (data->someone) {
			data->starttimer--;
			}
		}
		} else {
			data->timer--;
		}

		if (data->timer==0) {
			SwitchCurrentGamestate(game, "winner");
		}
}

	for (int i=0; i<game->data->buttons; i++) {
		if (game->data->players[i].offtime) game->data->players[i].offtime--;
	}

	if (data->count%25==0) {

		for (int i=0; i<game->data->buttons; i++) {
int wrog = -1;
      float d = 200;
			for (int j=0; j<10; j++) {
				if (i==j) continue;
				if (!game->data->players[i].active) continue;
				if (!game->data->players[j].active) continue;

				if ((!game->data->players[i].berek) && (!game->data->players[j].berek) && (data->crowntaken)) continue;
float dd;
//				PrintConsole(game, "i: %d, j: %d - x1: %f, y1: %f;;; x2: %f, y2: %f", i,j,game->data->players[i].x, game->data->players[i].y, game->data->players[j].x, game->data->players[j].y);
if (data->crowntaken) {
	        dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x,2) + pow(game->data->players[i].y-game->data->players[j].y,2));
					if (dd<d) {
						d=dd;
						wrog=j;
					}
					dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x-100,2) + pow(game->data->players[i].y-game->data->players[j].y,2));
					if (dd<d) {
						d=dd;
						wrog=j;
					}
					dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x-100,2) + pow(game->data->players[i].y-game->data->players[j].y-100,2));
					if (dd<d) {
						d=dd;
						wrog=j;
					}
					dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x-100,2) + pow(game->data->players[i].y-game->data->players[j].y+100,2));
					if (dd<d) {
						d=dd;
						wrog=j;
					}
					dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x,2) + pow(game->data->players[i].y-game->data->players[j].y-100,2));
					if (dd<d) {
						d=dd;
						wrog=j;
					}
					dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x,2) + pow(game->data->players[i].y-game->data->players[j].y+100,2));
					if (dd<d) {
						d=dd;
						wrog=j;
					}
					dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x+100,2) + pow(game->data->players[i].y-game->data->players[j].y,2));
					if (dd<d) {
						d=dd;
						wrog=j;
					}
					dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x+100,2) + pow(game->data->players[i].y-game->data->players[j].y-100,2));
					if (dd<d) {
						d=dd;
						wrog=j;
					}
					dd = sqrt(pow(game->data->players[i].x-game->data->players[j].x+100,2) + pow(game->data->players[i].y-game->data->players[j].y+100,2));
					if (dd<d) {
						d=dd;
						wrog=j;
				}
}

          if (!data->crowntaken) {
						dd = sqrt(pow(game->data->players[i].x-50,2) + pow(game->data->players[i].y-50,2));
						if (dd<d) {
							d=dd;
}
					}
			}

		unsigned char barray[USBBTN_SIZE];

//if (d>100) d = 100;
//d-=80;
//d=20-d;
//if (d<0) d=0;
if (game->data->players[i].active) {
	if (!data->crowntaken) {
		if (sqrt(pow(game->data->players[i].x-50,2) + pow(game->data->players[i].y-50,2)) < 10) {
			game->data->players[i].berek = true;
			game->data->berek = i;
			data->crowntaken = true;
		}
	}
}
if (wrog>=0) {
if (!game->data->players[wrog].offtime) {
	if ((game->data->players[i].berek) && (d<10)) {
		game->data->players[i].offtime = 60*5;
		game->data->players[i].berek = false;
		game->data->players[wrog].berek = true;
		game->data->berek = wrog;
		al_play_sample_instance(data->bam);

	}
}
}

int val = (d/200.0)*255;
val=255-val;
val-=192;
val-=32;
if (val<0) val=0;
val*=8;
//PrintConsole(game, "d: %f; led: %d", d, val);

    if (game->data->players[i].active) {
			if (val>=200) {
				randomButtonColor(&barray, 255, 255, 255, i);
			} else {
				randomButtonColor(&barray, ((game->data->players[i].rotating) ? 16 : 0), 1, val, i);
			}
		} else {
			randomButtonColor(&barray, 0, 0, 0, i);
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

	}
	data->count++;
}

void Gamestate_Draw(struct Game *game, struct GamestateResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.
al_clear_to_color(al_map_rgb(65,54,92));

char timeleft[10];

if (!data->started) {
	snprintf(&timeleft, 10, "%d", data->starttimer);
DrawTextWithShadow(data->scorefont, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.2, ALLEGRO_ALIGN_CENTER, timeleft);
} else {
	snprintf(&timeleft, 10, "%d", data->timer);
DrawTextWithShadow(data->scorefont, al_map_rgb(255,255,255), game->viewport.width, game->viewport.height*0.5, ALLEGRO_ALIGN_RIGHT, timeleft);
}

for (int i=0; i<10; i++) {
	  if (!game->data->players[i].active) {
			continue;
		}

al_set_target_bitmap(data->player);
al_clear_to_color(al_map_rgba(0,0,0,0));
ALLEGRO_BITMAP *bmp = game->data->players[i].berek ? data->kaczka2 : data->kaczka;
al_draw_rotated_bitmap(bmp, al_get_bitmap_width(bmp)/2, al_get_bitmap_height(bmp)/2,
                       600, 600,
                       sin(data->counter/16.0)/4.0, 0);

char a[2] = "1";
a[0]+=i;

DrawTextWithShadow(data->font, al_map_rgb(255,255,255), 600, 450, ALLEGRO_ALIGN_CENTER, a);

al_set_target_backbuffer(game->display);

al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              game->data->players[i].x * game->viewport.width / 100.0, game->data->players[i].y * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),

                               game->data->players[i].rot, 0);

al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              (100+game->data->players[i].x) * game->viewport.width / 100.0, game->data->players[i].y * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),
                              game->data->players[i].rot, 0);

al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              (game->data->players[i].x-100) * game->viewport.width / 100.0, game->data->players[i].y * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),
                              game->data->players[i].rot, 0);



al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              game->data->players[i].x * game->viewport.width / 100.0, (game->data->players[i].y+100) * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),
                               game->data->players[i].rot, 0);

al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              (100+game->data->players[i].x) * game->viewport.width / 100.0, (game->data->players[i].y+100) * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),
                               game->data->players[i].rot, 0);

al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              (game->data->players[i].x-100) * game->viewport.width / 100.0, (game->data->players[i].y+100) * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),
                               game->data->players[i].rot, 0);


al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              game->data->players[i].x * game->viewport.width / 100.0, (game->data->players[i].y-100) * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),
                               game->data->players[i].rot, 0);

al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              (100+game->data->players[i].x) * game->viewport.width / 100.0, (game->data->players[i].y-100) * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),
                              game->data->players[i].rot, 0);

al_draw_scaled_rotated_bitmap(data->player, al_get_bitmap_width(data->kaczka)/2, al_get_bitmap_height(data->kaczka)/2,
                              (game->data->players[i].x-100) * game->viewport.width / 100.0, (game->data->players[i].y-100) * game->viewport.height / 100.0,
                              (game->viewport.width*0.09375)/(float)al_get_bitmap_width(data->player), (game->viewport.height*0.16666)/(float)al_get_bitmap_height(data->player),
                               game->data->players[i].rot, 0);


  }

if (!data->crowntaken) {
	al_draw_scaled_rotated_bitmap(data->crown, al_get_bitmap_width(data->crown)/2, al_get_bitmap_height(data->crown)/2,
	                              game->viewport.width / 2, game->viewport.height / 2,
	                              0.2, 0.2, 0, 0);
}

}

void Gamestate_ProcessEvent(struct Game *game, struct GamestateResources* data, ALLEGRO_EVENT *ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		  SwitchCurrentGamestate(game, "empty"); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}



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
	if (!game->data->players[ev->keyboard.keycode - ALLEGRO_KEY_1].active) {
		data->someone = true;
		if (data->starttimer) { data->starttimer = 5; }
		  game->data->players[ev->keyboard.keycode - ALLEGRO_KEY_1].active = true;
			if (rand()%2) {
				game->data->players[ev->keyboard.keycode - ALLEGRO_KEY_1].x = 8 * (rand()%2 ? 1 : -1);
			} else {
				game->data->players[ev->keyboard.keycode - ALLEGRO_KEY_1].y = 8 * (rand()%2 ? 1 : -1);
			}
	} else {
		game->data->players[ev->keyboard.keycode - ALLEGRO_KEY_1].rotating = !game->data->players[ev->keyboard.keycode - ALLEGRO_KEY_1].rotating;
		int i = ev->keyboard.keycode - ALLEGRO_KEY_1;

		if (i<game->data->buttons) {

		unsigned char barray[USBBTN_SIZE];

		  randomButtonColor(&barray, (game->data->players[i].rotating) ? 255 : 0, 0, 128, i);

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

	}
	}

}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct GamestateResources *data = malloc(sizeof(struct GamestateResources));
	data->font = al_load_font(GetDataFilePath(game, "fonts/DejaVuSansMono.ttf"), game->viewport.height*0.2, 0);

	data->scorefont = al_load_font(GetDataFilePath(game, "fonts/chlorinuh.ttf"), game->viewport.height*0.5, 0);


	data->kaczka = al_load_bitmap(GetDataFilePath(game, "duckgame.png"));
	data->kaczka2 = al_load_bitmap(GetDataFilePath(game, "king.png"));

	data->crown = al_load_bitmap(GetDataFilePath(game, "korona.png"));


	data->player = al_create_bitmap(1200, 1200);

	data->bamsample = al_load_sample( GetDataFilePath(game, "badum.wav") );
	data->bam = al_create_sample_instance(data->bamsample);
	al_attach_sample_instance_to_mixer(data->bam, game->audio.fx);
	al_set_sample_instance_playmode(data->bam, ALLEGRO_PLAYMODE_ONCE);


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
	data->blink_counter = 0;
data->counter=0;
data->someone = false;
data->crowntaken=true;
data->starttimer = 5;
data->started= false;
  for (int i=0; i<10; i++) {
		game->data->players[i].rot = rand() % 100;
		game->data->players[i].x = rand() % 100;
		game->data->players[i].y = rand() % 100;
		game->data->players[i].rotating = false;
	}

	data->timer = 60;
}

void Gamestate_Stop(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.

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
