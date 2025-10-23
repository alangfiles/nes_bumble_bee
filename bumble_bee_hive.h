#define GAME_LENGTH 99 // Timer starts at 99 and counts down to 0
#define GAME_DURATION_SECONDS 40 // Actual game duration in seconds
#define TIMER_TICK_FREQUENCY 24 //((GAME_DURATION_SECONDS * 60) / GAME_LENGTH) // ~24.24 frames per tick
#define POWERUP_TIMER 10 // Powerup duration in game seconds
#define STUN_DURATION 3
#define QUACK_COOLDOWN 10
#define HITBOX_DEBUG 0

// Speed options
#define SPEED_SLOW 0x060
#define SPEED_REGULAR 0x0A0
#define SPEED_FAST 0x0E0
#define SPEED_QUACK 0x060 // additional quack speed boost

#define DUCK_SPEED_LOW 20
#define DUCK_SPEED_MED 0
#define DUCK_SPEED_HIGH 20
#define DUCK_SPEED_MAX 40

#define SPEED_TURBO_BOOST 0x060
#define SPEED_BIGBEE_BOOST 0x060 //big bee has constant turbo (old value was 0x030)
#define TURBO_LOW 0x40
#define TURBO_MEDIUM 0x80
#define TURBO_HIGH 0xF0

#define START_POS_DEFAULT 0
#define START_POS_WIDE 1
#define START_POS_CLOSE 2
#define START_POS_CORNERS 3
#define START_POS_MIDDLE 4

#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
unsigned int doublepad;
unsigned char generic_pad;
unsigned char turbo_p1;  // Turbo counter for player 1
unsigned char turbo_p2;  // Turbo counter for player 2
unsigned char turbo_p3;  // Turbo counter for player 3
unsigned char turbo_p4;  // Turbo counter for player 4
unsigned char prev_pad1;
unsigned char prev_pad2;
unsigned char prev_pad3;
unsigned char prev_pad4;
unsigned char pad1;
unsigned char pad2;
unsigned char pad3;
unsigned char pad4;
unsigned char pad1_new;
unsigned char pad2_new;
unsigned char pad3_new;
unsigned char pad4_new;
unsigned char game_mode;
unsigned char winner;
unsigned char game_frame_timer;
unsigned char game_timer;
signed int speed_option; 
signed int current_speed;
unsigned char force_redraw; 
unsigned char use_turbo;
unsigned char turbo_amount;
unsigned char ducks_go_faster_over_time;

unsigned char map_positions;

unsigned char collision;
unsigned char collision_L;
unsigned char collision_R;
unsigned char collision_U;
unsigned char collision_D;
unsigned char coordinates;
unsigned char temp1;
unsigned char temp2;
unsigned char temp3;
unsigned char temp4;
unsigned int temp5;
unsigned int temp6;
unsigned int tempint;
unsigned char eject_L; // from the left
unsigned char eject_R; // remember these from the collision sub routine
unsigned char eject_D; // from below
unsigned char eject_U; // from up
#define LEFT 0
#define RIGHT 1

// Direction constants
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define DIR_NONE 4

int address;
unsigned char x; // room loader code
unsigned char y;
unsigned char index;
unsigned char map;
unsigned int scroll_x;
unsigned int scroll_y;
signed int hero_velocity_x; // signed, low byte is sub-pixel
signed int hero_velocity_y;

unsigned int old_x;
unsigned int old_y;
unsigned char temp;
unsigned char temp_x;
unsigned char temp_y;
unsigned char temp_x2;
unsigned char temp_y2;
unsigned int largeindex;
unsigned char song;
unsigned char collision_box_size;

unsigned char current_settings_choice;
unsigned char settings_map;
unsigned char settings_speed;
unsigned char settings_song;

enum {
	SETTING_SPEED,
	SETTING_MAP,
	SETTING_SONG
};

enum {
	SPEED_OPTION_SLOW,
	SPEED_OPTION_REGULAR,
	SPEED_OPTION_FAST
};

enum {
	SONG_BEE,
	SONG_LAKE,
	SONG_HONEY
};

// pointer to currently active map data (set in init_game_loop)
const unsigned char *map_ptr;

unsigned char bit_index;
unsigned char bit_offset;
unsigned char frame_counter;
unsigned char ai_counter;
unsigned char current_player; // tracks which player is currently being processed
unsigned char team1_score; // score for team 1 (players 1 & 2)
unsigned char team2_score; // score for team 2 (players 3 & 4)
unsigned char team1_wins;
unsigned char team2_wins;
unsigned char win_reason; // tracks how the game was won

unsigned char powerup1;
unsigned char powerup2;
unsigned char powerup3;
unsigned char powerup4;

// Bigbee transformation variables
unsigned char bee1_bigbee_timer; // Timer for player 1 bigbee transformation (0 = not transformed)
unsigned char bee3_bigbee_timer; // Timer for player 3 bigbee transformation (0 = not transformed)

unsigned char sprite_rotation; // tracks which order to draw sprites in

// Animation frame tracking (cycles every 10 frames)
unsigned char anim_frame_1; // animation frame for player 1
unsigned char anim_frame_2; // animation frame for player 2
unsigned char anim_frame_3; // animation frame for player 3
unsigned char anim_frame_4; // animation frame for player 4

#pragma bss-name(push, "BSS")
unsigned char consumed_dots[128]; // tracks which dots have been consumed (8 bits per byte = 1024 bits)

// Title screen variables
unsigned char start_hold_timer; // Timer for holding start button
unsigned char start_held; // Whether start is currently held

// #include "MAPS/tinyhoney.h"
#include "MAPS/combs.h"
#include "MAPS/vines.h"
#include "MAPS/outdoors.h"
#include "MAPS/settings.h"
#include "MAPS/title.h"



struct Base {
	unsigned char x;
	unsigned char y;
	unsigned char width;
	unsigned char height;
};

struct Base Generic;


struct BoxGuy {
	unsigned int x;
	unsigned int y;
	unsigned char direction;
	unsigned char moving;
};

// Quack projectile for each duck (2 and 4)
struct BoxGuy quack2;
struct BoxGuy quack4;

unsigned char quack2_cooldown; // Cooldown timer for duck 2's quack
unsigned char quack4_cooldown; // Cooldown timer for duck 4's qu

// Stun state for each player (frames left stunned)
 unsigned char stun_p1;
 unsigned char stun_p2;
 unsigned char stun_p3;
 unsigned char stun_p4;

struct BoxGuy GenericBoxGuy;

struct BoxGuy BoxGuy1 = {0x4000,0x2800};
struct BoxGuy BoxGuy2 = {0x8000,0x3000};
struct BoxGuy BoxGuy3 = {0xA000,0x3000};
struct BoxGuy BoxGuy4 = {0xC000,0x4000};

#define HERO_WIDTH 6
#define HERO_HEIGHT 6

const unsigned char COMBS_FLOWER_X[4] = {16, 226, 16, 226};
const unsigned char COMBS_FLOWER_Y[4] = {32, 32, 208, 208};
const unsigned char VINES_FLOWER_X[4] = {16, 224, 16, 224};
const unsigned char VINES_FLOWER_Y[4] = {32, 32, 208, 208};
const unsigned char OUTDOORS_FLOWER_X[4] = {32, 208, 16, 224};
const unsigned char OUTDOORS_FLOWER_Y[4] = {112, 112, 208, 208};

enum
{
	MODE_TITLE,
	MODE_OPTIONS,
	MODE_GAME,
	MODE_GAMEOVER,
	MODE_ROUNDOVER
};

enum{
	ONETWO_WINNER,
	THREEFOUR_WINNER,
	TIE_WINNER
};

enum{
	WIN_DOTS,
	WIN_FRIENDLY_FIRE,
	WIN_ENEMY_KILL,
	WIN_TIME_UP,
	WIN_BIGBEE_EAT_DUCK
};

enum{
	SFX_TEAM1_DOT_COLLECT,
	SFX_TEAM2_DOT_COLLECT,
	SFX_TEAM1_WIN,
	SFX_TEAM2_WIN,
	SFX_DRAW_GAME,
	SFX_BLITZ,
	SFX_POWERUP,
	SFX_DUCKEATSBEE,
	SFX_START
};

enum{
	MAP_COMBS,
	MAP_OUTDOORS,
	MAP_VINES,
	MAP_RANDOM
};

enum {
	GAME_SLOW,
	GAME_REGULAR,
	GAME_FAST
};


const unsigned char palette_combsnplants_bg[]={	0x0f,0x30,0x27,0x07,0x0f,0x35,0x26,0x07,0x0f,0x0f,0x30,0x30,0x0f,0x21,0x16,0x30};
const unsigned char palette_outdoors_bg[16]={ 0x0f,0x30,0x17,0x28,0x0f,0x35,0x26,0x07,0x0f,0x30,0x2a,0x1b,0x0f,0x30,0x21,0x11 };
const unsigned char palette_vineswoflowers_bg[16]={ 0x0f,0x30,0x27,0x07,0x0f,0x30,0x2a,0x1b,0x0f,0x1a,0x30,0x38,0x0f,0x30,0x15,0x00 };

const unsigned char palette_sp[]={
0x0f, 0x17, 0x26, 0x38, 
0x0f, 0x0C, 0x2C, 0x38, 
0x0f, 0x17, 0x20, 0x26, 
0x0f, 0x0C, 0x2C, 0x38, 
}; 

const unsigned char brian_pallete_sp[] = {0x0f,0x38,0x30,0x15,0x0f,0x2c,0x15,0x30,0x0f,0x2b,0x30,0x38,0x0f,0x16,0x28,0x1a};

// Title Palettes: 
const unsigned char palette_title_bg[]={ 0x0f,0x30,0x27,0x07,0x0f,0x07,0x2a,0x1b,0x0f,0x30,0x2a,0x1b,0x0f,0x30,0x21,0x11 };

// Settings Palettes: 
const unsigned char palette_options_bg[]={ 0x0f,0x30,0x27,0x07,0x0f,0x24,0x2a,0x1b,0x0f,0x30,0x2a,0x1b,0x0f,0x27,0x2a,0x1b };




// 5 bytes per metatile definition, tile TL, TR, BL, BR, palette 0-3
// T means top, B means bottom, L left,R right
// 51 maximum # of metatiles = 255 bytes

const unsigned char metatiles1[]={
	2, 2, 2, 2,  3,
	4, 4, 4, 4,  1,
	9, 9, 9, 9,  2,
	5, 6, 8, 7,  1,
	5, 6, 8, 7,  0
};

// data is exactly 240 bytes, 16 * 15
// doubles as the collision map data



// PROTOTYPES
void load_room(void);
void draw_sprites(void);
void draw_player_1(void);
void draw_player_2(void);
void draw_player_3(void);
void draw_player_4(void);
void movement(void);	
void check_tile_and_collect(void);
void initialize_collision_map(void);
void debug_extras(void);
void read_controllers(void);
void seeker_ai(void);
void chaser_ai(void);
void game_loop(void);
void title_loop(void);
void options_loop(void);
void gameover_loop(void);
void init_game_loop(void);
void init_options_loop(void);
void init_gameover_loop(void);
void init_title_loop(void);
void init_system(void);
void clear_background(void);
void update_hud(void);
void init_roundover(void);
void start_round(void);
void roundover_loop(void);
void game_counters(void);
void quack_movement(void);
void load_bg_palette(void);
void update_options_screen(void);

char bg_collision_sub(void);
char bg_coll_L(void);
char bg_coll_R(void);
char bg_coll_U(void);
char bg_coll_D(void);


char sprite_collision(void);
char check_powerup_collision(void);