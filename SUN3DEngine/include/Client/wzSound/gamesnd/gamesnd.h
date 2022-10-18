#ifndef __GAMESND_H__
#define __GAMESND_H__



// initializer

void gamesnd_init_sounds();


// loader, unloader
void gamesnd_load_gameplay_sounds();
void gamesnd_unload_gameplay_sounds();

void gamesnd_load_interface_sounds();
void gamesnd_unload_interface_sounds();

//
void gamesnd_preload_common_sounds();

int gamesnd_play_iface(int n, float vol_scale);
void gamesnd_stop_iface(int handle);
void gamesnd_play_error_beep();
void common_play_highlight_sound();	// called from interface code

void gamesnd_start_music(char* music_name);
void gamesnd_stop_music();
void gamesnd_close();

#endif	/* __GAMESND_H__ */
