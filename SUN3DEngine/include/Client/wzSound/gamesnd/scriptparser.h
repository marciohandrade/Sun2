#ifndef __SCRIPTPARSER_H__
#define __SCRIPTPARSER_H__
// scriptparser.h


#pragma warning(disable: 4786)
#pragma warning(disable: 4284)
#include <map>

#include "sound/sound.h"

#include "ambientsnd.h"
#include "voicesnd.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

extern int use_secure_filesystem;

typedef std::map<int, game_snd> SndTable;
extern SndTable Snds;
extern SndTable Snds_iface;


typedef std::map<int, AmbientSnd> AmbientTable;
extern AmbientTable AmbientTbl;

typedef std::map<int, VoiceSnd> VoiceTable;
extern VoiceTable VoiceTbl;

typedef std::map<int, int, std::less<int> > int2int_map;
typedef int2int_map::iterator int2int_iter;

extern int2int_map iface_soundHandle_table;
extern int2int_map index_table_event_music;
extern int2int_map index_table_voice_sound;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void gamesnd_parse_sound(char* fname, bool bTxt = true);
void gamesnd_parse_music(char* fname, bool bTxt = true);
void gamesnd_parse_voice(char* fname, bool bTxt = true);
void gamesnd_parse_config(char* fname, bool bTxt = true);

void gamesnd_parse_soundtbl(char* fname);
void gamesnd_parse_musictbl(char* fname);
void gamesnd_parse_voicetbl(char* fname);
void gamesnd_parse_configtbl(char* fname);

void gamesnd_parse_soundtxt(char* fname);
void gamesnd_parse_musictxt(char* fname);
void gamesnd_parse_voicetxt(char* fname);
void gamesnd_parse_configtxt(char* fname);


#endif //__SCRIPTPARSER_H__