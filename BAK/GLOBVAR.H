#ifndef __TK__GLOBVAR__
#define __TK__GLOBVAR__

#include <time.h>
#include <keyb.hpp>
#include "defines.h"
#include "types.h"
#include "classes.h"

extern int ipxavail;
extern int BACK_TO_MENU;
extern int VESA2,FORCE_0x13,scr_y_size,GAME_MODE,NETWORK_MODE;
extern int KILLING_MODE,SHOW_ENEMIES,target_cost;  // t�ht�imen hinta
extern volatile unsigned framecount;
extern int effect_age_count;
extern int dust_blocks[];
extern int Light_sizes[LIGHTS];
extern int DARK_MODE,LIGHT_EFFECTS,SHADOWS,SMOKE_EFFECTS;
extern int sell_price[DIFF_WEAPONS+2]; // myyntihinnat muuttuvia
extern struct BLOCK *level;
extern char *see_level;
extern char cratespr[],shadowspr[],
     c4spr[],
     grenadespr[],
	 targetspr[],
	 floorspr[],
	 wallspr[],
	 floorspr2[],
	 rambospr[],
	 bodypartspr[],
	 effectspr[],
	 pal[],
     picture[],
     warespr[],
	 enemyspr0[],
	 enemyspr1[],
	 enemyspr2[],
	 enemyspr3[],
	 enemyspr4[],
	 enemyspr5[],
	 enemyspr6[],
	 minespr[];
extern char name1[10],name2[10];
extern char trans_table[256][256];
extern char shadow_table[256][16];
extern char *menu_text[Menu_texts];
extern int first,all_dead,quit,MAP_ON,next_level,complete_time;
extern char DEATH_MATCH_LEVEL[13];
extern int DEATH_MATCH_EPISODE;
extern int DEATH_MATCH_SPEED;
extern int episode;

extern int EFFECT_VOLUME,MUSIC_VOLUME;
extern clock_t st,et;
extern int F_PER_SEC,OVER_POWER,START_LEVEL,LEVELS;

extern int Spot_amount,Steam_amount,Spot_count,Steam_count;
extern int level_x_size,level_y_size;
extern int pl_start_x[MAX_PLAYERS],pl_start_y[MAX_PLAYERS];
extern int PLAYERS,ACTIVE_PLAYERS,ENEMIES,LEVEL,DEAD_ENEMIES,ENEMIES_ON_GAME;

extern int FRAMES_ON,target_frames,delau;
extern char *k_text[DIFF_K];
extern struct special spec[SPEC_K];
extern Effect_type effect_type[10];
extern char virbuff[];
extern float sini[],cosi[];
extern char *thefonts[10];
extern int f_x_size[10],f_y_size[10];
extern int FONT_NUM;
extern KeyboardHandler k;
extern Keys keys1,keys2;
extern Player player[MAX_PLAYERS];	   // PLAYERS 
extern Body_part body_part[MAX_BODY_PARTS];
extern Enemy enemy[MAX_ENEMIES];
extern Effect effect[MAX_EFFECTS];
extern Bullet bullet[MAX_BULLETS];
extern Spot spot_light[MAX_SPOTS];
extern Steam steam[MAX_STEAM];
extern Light light[LIGHTS];
extern Light_table normal_l,red_l,yellow_l,explo_l;
extern General_level_info     general_level_info;
extern Normal_game_level_info normal_game_level_info;
extern Death_match_level_info death_match_level_info;
extern Crate crate[MAX_CRATES];
extern Bullet_type bullet_type[DIFF_BULLETS+2];
extern struct s shield_data;
extern struct WEapon weapon[DIFF_WEAPONS+1];
extern struct juu enemy_info[DIFF_ENEMIES];

extern MIDASsample magnumwav,
            shotgunwav,
			pistolwav,
			sinkowav,
			hitwav,
			explowav,
			klikwav,
			deathwav,
			wooshwav,
			flamewav;

extern MIDASmodule biisi;
extern MIDASmodulePlayHandle ph;
extern int sample_rates[DIFF_WEAPONS+1];
extern Level leveldata;

extern char *player_opt_text[num_player_options];
extern char *game_opt_text[num_game_options];
extern char *sound_opt_text[num_sound_options];
extern char *opt_text[num_options];
extern char *deathmatch_opt_text[num_deathmatch_options];
extern char *multip_opt_text[num_multip_options];
extern vile filelist[256][1024];
extern vile dirlist[256];
extern int files_in_episode[256];
extern int dirs;

#endif 

