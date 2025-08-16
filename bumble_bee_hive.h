#define GAME_LENGTH 99 // Timer starts at 99 and counts down to 0
#define GAME_DURATION_SECONDS 40 // Actual game duration in seconds
#define TIMER_TICK_FREQUENCY 24 //((GAME_DURATION_SECONDS * 60) / GAME_LENGTH) // ~24.24 frames per tick

// Speed options
#define SPEED_SLOW 0x060
#define SPEED_REGULAR 0x0A0
#define SPEED_FAST 0x0E0

#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
unsigned int doublepad;
unsigned char generic_pad;
unsigned char pad1;
unsigned char pad2;
unsigned char pad3;
unsigned char pad4;
unsigned char game_mode;
unsigned char winner;
unsigned char game_frame_timer;
unsigned char game_timer;
unsigned char speed_option; // 0=slow, 1=regular, 2=fast
unsigned char force_redraw; 

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

int address;
unsigned char x; // room loader code
unsigned char y;
unsigned char index;
unsigned char map;
unsigned int scroll_x;
unsigned int scroll_y;
signed int hero_velocity_x; // signed, low byte is sub-pixel
signed int hero_velocity_y;
//unsigned int hero_x;
//unsigned int hero_y;
//unsigned char L_R_switch;
unsigned int old_x;
unsigned int old_y;
unsigned char temp;
unsigned char temp_x;
unsigned char temp_y;
unsigned char temp_x2;
unsigned char temp_y2;
unsigned int largeindex;
unsigned char song;

unsigned char bit_index;
unsigned char bit_offset;
unsigned char frame_counter;
unsigned char ai_counter;
unsigned char current_player; // tracks which player is currently being processed
unsigned char team1_score; // score for team 1 (players 1 & 2)
unsigned char team2_score; // score for team 2 (players 3 & 4)
unsigned char win_reason; // tracks how the game was won

#pragma bss-name(push, "BSS")
unsigned char consumed_dots[128]; // tracks which dots have been consumed (8 bits per byte = 1024 bits)

// Title screen variables
unsigned char start_hold_timer; // Timer for holding start button
unsigned char start_held; // Whether start is currently held

#include "tinyhoney.h"


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
};

struct BoxGuy GenericBoxGuy;

struct BoxGuy BoxGuy1 = {0x4000,0x2800};
struct BoxGuy BoxGuy2 = {0x8000,0x3000};
struct BoxGuy BoxGuy3 = {0xA000,0x3000};
struct BoxGuy BoxGuy4 = {0xC000,0x4000};

#define HERO_WIDTH 6
#define HERO_HEIGHT 6


enum
{
	MODE_TITLE,
	MODE_OPTIONS,
	MODE_GAME,
	MODE_GAMEOVER
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
	WIN_TIME_UP
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

enum {
	SONG_MAIN_SONG
};




const unsigned char palette_bg[]={
	0x0f,0x30,0x27,0x07,0x0f,0x35,0x26,0x07,0x0f,0x0f,0x30,0x30,0x0f,0x21,0x16,0x30};

// 0x0f, 0x16, 0x27, 0x08, // black, gray, lt gray, white
// 0x0f, 0x07, 0x17, 0x27, // oranges
// 0x0f, 0x02, 0x12, 0x22, // blues
// 0x0f, 0x09, 0x19, 0x29, // greens
// }; 

const unsigned char palette_sp[]={
0x0f, 0x07, 0x28, 0x38, // dk brown, yellow, white yellow
0x0f, 0x07, 0x0A, 0x38, // dk brown, yellow, white yellow
0x0f, 0x07, 0x20, 0x38, // dk brown, yellow, white yellow
0x0f, 0x07, 0x14, 0x38, // dk brown, yellow, white yellow
}; 


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
void draw_title_sprites(void);
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


char bg_collision_sub(void);
char bg_coll_L(void);
char bg_coll_R(void);
char bg_coll_U(void);
char bg_coll_D(void);


char sprite_collision(void);