
#ifndef __DS3D_H__
#define __DS3D_H__

extern LPDIRECTSOUND3DLISTENER	pDS3D_listener;

int	ds3d_init(int voice_manager_required);
void	ds3d_close();
int	ds3d_update_listener(vector *pos, vector *vel, matrix *orient, float distance_factor, float doppler, float rolloff);
int	ds3d_update_buffer(int channel, game_snd *gs, float min, float max, vector *pos, vector *vel);
int	ds3d_set_sound_cone(int channel, vector *orient, int inner_angle, int outer_angle, int vol);

#endif /* __DS3D_H__ */
