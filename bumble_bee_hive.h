#define SPEED 0x180


#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
unsigned int doublepad;
unsigned char generic_pad;
unsigned char pad1;
unsigned char pad2;
unsigned char pad3;
unsigned char pad4;

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

unsigned char bit_index;
unsigned char bit_offset;
unsigned char frame_counter;

#pragma bss-name(push, "BSS")
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
struct BoxGuy BoxGuy4 = {0xC000,0x3000};

#define HERO_WIDTH 6
#define HERO_HEIGHT 6







const unsigned char palette_bg[]={
0x0f, 0x16, 0x27, 0x08, // black, gray, lt gray, white
0x0f, 0x07, 0x17, 0x27, // oranges
0x0f, 0x02, 0x12, 0x22, // blues
0x0f, 0x09, 0x19, 0x29, // greens
}; 

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
void movement(void);	
void check_tile_and_collect(void);
void initialize_collision_map(void);

char bg_collision_sub(void);
char bg_coll_L(void);
char bg_coll_R(void);
char bg_coll_U(void);
char bg_coll_D(void);


char sprite_collision(void);