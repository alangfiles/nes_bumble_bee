/*	example code for cc65, for NES
 *  use the four score to move 4 players,
 *  around a collideable bg
 *	using neslib, nesdoug, and nesfourscore
 *	Based off code by Doug Fraker 2018 (www.nesdoug.com)
 *  Written by Alan Files 2024
 */

#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "LIB/nesfourscore.h"
#include "Sprites.h" // holds our metasprite data
#include "bumble_bee_hive.h"

void main(void)
{
	
	
	/*
	 1. title screen
	 2. 4 players, can it work with other numbers?
	   - 1 player (bee) + ais
	 3. game over screens


	*/
	
	init_system();
	init_title_loop();

	//idk why this code is in here -agf 7/29
	// one_vram_buffer(0x63, NTADR_A(17, 1));
	// one_vram_buffer(0x63, NTADR_A(21, 1));
	// one_vram_buffer(0x63, NTADR_A(22, 1));

	while (1)
	{
		if (game_mode == MODE_TITLE)
		{
			title_loop();
		}
		if (game_mode == MODE_GAME)
		{
			game_loop();
		}
		if (game_mode == MODE_GAMEOVER)
		{
			gameover_loop();
		}
	} 
}

void load_room(void)
{
	// this will just load the nam
	ppu_off();
	vram_adr(NAMETABLE_A);
	for (largeindex = 0; largeindex < 1024; ++largeindex)
	{
		vram_put(tinyhoney[largeindex]);
		flush_vram_update2();
	}
	ppu_on_all();
}

void draw_sprites(void)
{
	// clear all sprites from sprite buffer
	oam_clear();

	temp_x = BoxGuy1.x >> 8;
	temp_y = BoxGuy1.y >> 8;
	if (temp_x == 0)
		temp_x = 1;
	if (temp_y == 0)
		temp_y = 1;

	// draw 1 metasprite
	oam_meta_spr(temp_x, temp_y, SmallBee1);

	temp_x = BoxGuy2.x >> 8;
	temp_y = BoxGuy2.y >> 8;
	if (temp_x == 0)
		temp_x = 1;
	if (temp_y == 0)
		temp_y = 1;

	// draw 1 metasprite
	oam_meta_spr(temp_x, temp_y, SmallBee2);

	temp_x = BoxGuy3.x >> 8;
	temp_y = BoxGuy3.y >> 8;
	if (temp_x == 0)
		temp_x = 1;
	if (temp_y == 0)
		temp_y = 1;

	// draw 1 metasprite
	oam_meta_spr(temp_x, temp_y, SmallBee3);

	temp_x = BoxGuy4.x >> 8;
	temp_y = BoxGuy4.y >> 8;
	if (temp_x == 0)
		temp_x = 1;
	if (temp_y == 0)
		temp_y = 1;

	// draw 1 metasprite
	oam_meta_spr(temp_x, temp_y, SmallBee4);
}

void movement(void)
{
	// handle x
	old_x = GenericBoxGuy.x;

	if (generic_pad & PAD_LEFT)
	{
		hero_velocity_x = -SPEED;
	}
	else if (generic_pad & PAD_RIGHT)
	{
		hero_velocity_x = SPEED;
	}
	else
	{ // nothing pressed
		hero_velocity_x = 0;
	}

	GenericBoxGuy.x += hero_velocity_x;

	if (GenericBoxGuy.x > 0xf000)
	{ // too far, don't wrap around
		if (old_x >= 0x8000)
		{
			GenericBoxGuy.x = 0xf000; // max right
		}
		else
		{
			GenericBoxGuy.x = 0x0000; // max left
		}
	}

	Generic.x = GenericBoxGuy.x >> 8; // the collision routine needs an 8 bit value
	Generic.y = GenericBoxGuy.y >> 8;

	Generic.width = HERO_WIDTH;
	Generic.height = HERO_HEIGHT;

	if (hero_velocity_x < 0)
	{ // going left
		if (bg_coll_L())
		{ // check collision left
			GenericBoxGuy.x = old_x; // revert to old position
		}
	}
	else if (hero_velocity_x > 0)
	{ // going right
		if (bg_coll_R())
		{ // check collision right
			GenericBoxGuy.x = old_x; // revert to old position
		}
	}
	// else 0, skip it

	// handle y
	old_y = GenericBoxGuy.y;

	if (generic_pad & PAD_UP)
	{
		hero_velocity_y = -SPEED;
	}
	else if (generic_pad & PAD_DOWN)
	{
		hero_velocity_y = SPEED;
	}
	else
	{ // nothing pressed
		hero_velocity_y = 0;
	}

	GenericBoxGuy.y += hero_velocity_y;

	if (GenericBoxGuy.y > 0xe000)
	{ // too far, don't wrap around

		if (old_y >= 0x8000)
		{
			GenericBoxGuy.y = 0xe000; // max down
		}
		else
		{
			GenericBoxGuy.y = 0x0000; // max up
		}
	}

	Generic.x = GenericBoxGuy.x >> 8; // the collision routine needs an 8 bit value
	Generic.y = GenericBoxGuy.y >> 8;
	//	Generic.width = HERO_WIDTH; // already is this
	//	Generic.height = HERO_HEIGHT;

	if (hero_velocity_y < 0)
	{ // going up
		if (bg_coll_U())
		{ // check collision up
			GenericBoxGuy.y = old_y; // revert to old position
		}
	}
	else if (hero_velocity_y > 0)
	{ // going down
		if (bg_coll_D())
		{ // check collision down
			GenericBoxGuy.y = old_y; // revert to old position
		}
	}
	// else 0, skip it

	// player has moved, now collect any pellets
	check_tile_and_collect();
}

char bg_coll_L(void)
{
	// check 2 points on the left side
	temp_x = Generic.x;

	eject_L = temp_x & 0x07; // distance to next tile boundary
	temp_y = Generic.y + 2;
	if (bg_collision_sub())
		return 1;

	temp_y = Generic.y + Generic.height;
	temp_y -= 2;
	if (bg_collision_sub())
		return 1;

	return 0;
}

char bg_coll_R(void)
{
	// check 2 points on the right side
	temp_x = Generic.x + Generic.width;

	eject_R = 8 - (temp_x & 0x07); // distance to next tile boundary
	temp_y = Generic.y + 2;
	if (bg_collision_sub())
		return 1;

	temp_y = Generic.y + Generic.height;
	temp_y -= 2;
	if (bg_collision_sub())
		return 1;

	return 0;
}

char bg_coll_U(void)
{
	// check 2 points on the top side
	temp_x = Generic.x + 2;

	temp_y = Generic.y;
	eject_U = temp_y & 0x07; // distance to next tile boundary
	if (bg_collision_sub())
		return 1;

	temp_x = Generic.x + Generic.width;
	temp_x -= 2;

	if (bg_collision_sub())
		return 1;

	return 0;
}

char bg_coll_D(void)
{
	// check 2 points on the bottom side
	temp_x = Generic.x + 2;

	temp_y = Generic.y + Generic.height;
	eject_D = 8 - (temp_y & 0x07); // distance to next tile boundary
	if (bg_collision_sub())
		return 1;

	temp_x = Generic.x + Generic.width;
	temp_x -= 2;

	if (bg_collision_sub())
		return 1;

	return 0;
}

const unsigned char pellet_tiles[5] = {
		0xc6, 0xc7, 0xc8, 0xc9, 0xca};

const unsigned char blank_tiles[5] = {
		0xb6, 0xb7, 0xb8, 0xb9, 0xba};

char bg_collision_sub(void)
{
	if (temp_y >= 0xf0)
		return 0;
	// temp_x and temp_y are the coordinates to check the collision

	temp = tinyhoney[((temp_y >> 3) << 5) + (temp_x >> 3)];
	// don't collide with pellets, but everything else
	if (temp == 0xc6 || temp == 0xc7 || temp == 0xc8 || temp == 0xc9 || temp == 0xca)
	{
		return 0;
	}
	else if (temp == 0xb6 || temp == 0xb7 || temp == 0xb8 || temp == 0xb9 || temp == 0xba)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

// Helper function to check if a dot has been consumed
char is_dot_consumed(unsigned int map_index) {
	unsigned char byte_index = map_index >> 3; // divide by 8
	unsigned char bit_index = map_index & 0x07; // modulo 8
	return (consumed_dots[byte_index] >> bit_index) & 0x01;
}

// Helper function to mark a dot as consumed
void mark_dot_consumed(unsigned int map_index) {
	unsigned char byte_index = map_index >> 3; // divide by 8
	unsigned char bit_index = map_index & 0x07; // modulo 8
	consumed_dots[byte_index] |= (0x01 << bit_index);
}

void check_tile_and_collect()
{
	// Only seekers (players 1 and 3) can collect dots
	if (current_player != 1 && current_player != 3)
	{
		return; // Chasers cannot collect dots
	}
	
	// the (+4) centers the location
	temp_x = (Generic.x + 4) >> 3; // get this between 0-30
	temp_y = (Generic.y + 2) >> 3; // get this between 0-32
	largeindex = (temp_y << 5) + temp_x;

	// Get the tile at the player's position
	temp = tinyhoney[largeindex];

	for (index = 0; index < 5; index++)
	{
		if (temp == pellet_tiles[index])
		{
			// Check if this dot has already been consumed
			if (!is_dot_consumed(largeindex)) {
				// Mark this dot as consumed
				mark_dot_consumed(largeindex);
				// Update the screen
				one_vram_buffer(blank_tiles[frame_counter % 5], NTADR_A(temp_x, temp_y));
						// update team score
		if (current_player == 1) {
			team1_score++;
			// Check for win condition
			if (team1_score >= 100) {
				winner = ONETWO_WINNER;
				win_reason = WIN_DOTS;
				init_gameover_loop();
				return;
			}
		} else if (current_player == 3) {
			team2_score++;
			// Check for win condition
			if (team2_score >= 100) {
				winner = THREEFOUR_WINNER;
				win_reason = WIN_DOTS;
				init_gameover_loop();
				return;
			}
		}
			}
			break;
		}
		else if (temp == blank_tiles[index])
		{
			// do nothing
			break;
		}
		else
		{
			// PLAYER COLLISION
		}
	}
}

// Collision detection function for 8x8 sprites
char sprite_collision()
{
	if (temp_x < temp_x2 + 8 && temp_x + 8 > temp_x2 &&
			temp_y < temp_y2 + 8 && temp_y + 8 > temp_y2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void debug_extras(void){
	// Display team 1 score (2 digits)
	temp1 = (team1_score / 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(6, 1));
	temp1 = (team1_score % 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(7, 1));


	//Timer
	temp1 = (game_timer / 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(15, 1));
	temp1 = (game_timer % 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(16, 1));



	// Display team 2 score (2 digits)
	temp1 = (team2_score / 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(23, 1));
	temp1 = (team2_score % 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(24, 1));
}

void read_controllers(void){
	// read controllers 1 and 3 into an integer
	doublepad = pad_poll_4score_1_3();
	pad1 = high_byte(doublepad); // the high byte is the first controller's input
	pad3 = low_byte(doublepad);	 // low byte is the third controller's input

	// read controllers 2 and 4 into an integer
	doublepad = pad_poll_4score_2_4();
	pad2 = high_byte(doublepad);
	pad4 = low_byte(doublepad);

	seeker_ai();
	chaser_ai();


	//debug just random values
	// [0b00010000,0b00100000,0b01000000,0b10000000]
}

void seeker_ai(void){ //ai for the bee
		/*
			avoid the nearest enemy chaser (player 2)
			avoid the friendly chaser (player 4)
			move towards nearest dot
		*/
		// pad3= PAD_DOWN;
}

void chaser_ai(void){ //ai for the duck
	/*
			move towards the nearest enemy seeker
			avoid the friendly seeker
		*/
	// pad2 = PAD_LEFT;
	// pad4 = PAD_LEFT;
}

void game_loop(void){
	// wait till beginning of the frame
		ppu_wait_nmi();

		// 0. DEBUGGING CODE
		debug_extras();

		// 1. INCREMENT GLOBAL COUNTERS
		frame_counter++;
		game_frame_timer++;
		if(game_frame_timer % 60 == 0) // every 60 frames
		{
			game_frame_timer = 0; // reset the frame timer
			game_timer--;
			if (game_timer == 0) {
				// time's up, check scores
				if (team1_score > team2_score) {
					winner = ONETWO_WINNER;
					win_reason = WIN_TIME_UP;
				} else if (team2_score > team1_score) {
					winner = THREEFOUR_WINNER;
					win_reason = WIN_TIME_UP;
				} else {
					winner = TIE_WINNER;
					win_reason = WIN_TIME_UP;
				}
				init_gameover_loop();
			}
		}
		ai_counter++;

		// 2.  READ CONTROLLER
		read_controllers();
		
		// 3. PLAYER MOVEMENT

		// player 1 is seeker, 2 is chaser
		// player 3 is seeker, 4 is chaser
		// Deal with movement for each player

		// setup generics for player 1 (seeker)
		current_player = 1;
		GenericBoxGuy = BoxGuy1;
		generic_pad = pad1;
		// call movement for generics
		movement(); //this assigns old_x and old_y	
		temp_x = GenericBoxGuy.x >> 8;
		temp_y = GenericBoxGuy.y >> 8;
		temp_x2 = BoxGuy3.x >> 8;
		temp_y2 = BoxGuy3.y >> 8;
		if (sprite_collision()) //1 and 3, both seekers
		{
			//players bounce off each other
			BoxGuy1.x = old_x;
			BoxGuy1.y = old_y;
		} else {
			BoxGuy1.x = GenericBoxGuy.x;
			BoxGuy1.y = GenericBoxGuy.y;
		}

		
		// setup generics for player 2 (chaser)
		current_player = 2;
		GenericBoxGuy = BoxGuy2;
		generic_pad = pad2;
		movement();
		
		temp_x = GenericBoxGuy.x >> 8;
		temp_y = GenericBoxGuy.y >> 8;
		temp_x2 = BoxGuy4.x >> 8;
		temp_y2 = BoxGuy4.y >> 8;
		if (sprite_collision()) //player 2 blocks player 4 (chasers)
		{
			//players bounce off each other
			BoxGuy2.x = old_x;
			BoxGuy2.y = old_y;
		} else {
			BoxGuy2.x = GenericBoxGuy.x;
			BoxGuy2.y = GenericBoxGuy.y;
		}


		// setup generics for player 3 (seeker)
		current_player = 3;
		GenericBoxGuy = BoxGuy3;
		generic_pad = pad3;
		movement();
		//player 1 blocks player 3
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;
		temp_x2 = GenericBoxGuy.x >> 8;
		temp_y2 = GenericBoxGuy.y >> 8;
		if (sprite_collision()) // 1 blocks 3 (seekers)
		{
			//players bounce off each other
			BoxGuy3.x = old_x;
			BoxGuy3.y = old_y;
		} else {
			BoxGuy3.x = GenericBoxGuy.x;
			BoxGuy3.y = GenericBoxGuy.y;
		}

		// setup generics for player 4 (chaser)
		current_player = 4;
		GenericBoxGuy = BoxGuy4;
		generic_pad = pad4;
		movement();
		//player 2 blocks player 4
		temp_x = BoxGuy2.x >> 8;
		temp_y = BoxGuy2.y >> 8;
		temp_x2 = GenericBoxGuy.x >> 8;
		temp_y2 = GenericBoxGuy.y >> 8;
		if (sprite_collision()) // 2 blocks 4 (seekers)
		{
			//players bounce off each other
			BoxGuy4.x = old_x;
			BoxGuy4.y = old_y;
		} else {
			BoxGuy4.x = GenericBoxGuy.x;
			BoxGuy4.y = GenericBoxGuy.y;
		}


		// 4. CHECK COLLISON

		//check for player deaths (1 collide with 2, 3 collide with 4)
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;
		temp_x2 = BoxGuy2.x >> 8;
		temp_y2 = BoxGuy2.y >> 8;
		if (sprite_collision())
		{
			//player 1 dies (friendly fire)
			winner = THREEFOUR_WINNER;
			win_reason = WIN_FRIENDLY_FIRE;
			init_gameover_loop();
		}
		temp_x = BoxGuy3.x >> 8;
		temp_y = BoxGuy3.y >> 8;
		temp_x2 = BoxGuy4.x >> 8;
		temp_y2 = BoxGuy4.y >> 8;
		if (sprite_collision())
		{
			//player 3 dies (friendly fire)
			winner = ONETWO_WINNER;
			win_reason = WIN_FRIENDLY_FIRE;
			init_gameover_loop();
		}
		//check 1 with 4 and 2 with 3
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;
		temp_x2 = BoxGuy4.x >> 8;
		temp_y2 = BoxGuy4.y >> 8;
		if (sprite_collision())
		{
			//player 1 dies (enemy fire)
			winner = THREEFOUR_WINNER;
			win_reason = WIN_ENEMY_KILL;
			init_gameover_loop();
		}
		temp_x = BoxGuy2.x >> 8;
		temp_y = BoxGuy2.y >> 8;
		temp_x2 = BoxGuy3.x >> 8;
		temp_y2 = BoxGuy3.y >> 8;
		if (sprite_collision())
		{
			//player 2 dies (enemy fire)
			winner = ONETWO_WINNER;
			win_reason = WIN_ENEMY_KILL;
			init_gameover_loop();
		}


		// 5. DRAW SPRITES
		draw_sprites();
}

void title_loop(void){

	while (1)
	{
		ppu_wait_nmi();
		pad1 = pad_poll(0); // read the first controller

		if (pad1 & PAD_START)
		{
			init_game_loop();
			break;
		}
	}
	
}

void gameover_loop(void){

	while (1)
	{
		ppu_wait_nmi();
		pad1 = pad_poll(0); // read the first controller

		if (pad1 & PAD_START)
		{
			init_title_loop();
			break;
		}
	}
}

void init_game_loop(void){
	game_mode = MODE_GAME;  
	// Initialize scores
	team1_score = 0;
	team2_score = 0;
	game_timer = GAME_LENGTH;
	win_reason = WIN_DOTS; // default
	// Initialize consumed dots tracking
	for (index = 0; index < 128; index++) {
		consumed_dots[index] = 0;
	}
	//move all players into starting positions:
	BoxGuy1.x = 0x4000;
	BoxGuy1.y = 0x2800;
	BoxGuy2.x = 0x7000;
	BoxGuy2.y = 0x2800;
	BoxGuy3.x = 0xB000;
	BoxGuy3.y = 0x2800;
	BoxGuy4.x = 0x8800;
	BoxGuy4.y = 0x2800;

	ppu_off(); // screen off 
	clear_vram_buffer();

	// load the palettes
	pal_bg(palette_bg);
	pal_spr(palette_sp);

	load_room();

	set_scroll_y(0xff); // shift the bg down 1 pixel

	ppu_on_all(); // turn on screen

}

void init_title_loop(void){
	delay(30);
	game_mode = MODE_TITLE;   
	ppu_off(); // screen off
	// load the title palettes
	pal_bg(palette_bg);
	pal_spr(palette_sp);
	
	multi_vram_buffer_horz("BRIAN AND ALAN GAMES", 20, NTADR_A(6, 6));
	multi_vram_buffer_horz("HONEY HEIST", 11, NTADR_A(10, 8));
	// draw on screen, probably an unrle eventually for this game
	// probably lets the players select ai or player.
	multi_vram_buffer_horz("PRESS START", 11, NTADR_A(10, 24));

	ppu_on_all(); // turn on screen
	
}

void init_gameover_loop(void){
	game_mode = MODE_GAMEOVER;   
	ppu_off(); // screen off
	// load the title palettes
	clear_vram_buffer();
	pal_bg(palette_bg);
	pal_spr(palette_sp);
	
	
	multi_vram_buffer_horz("GAME OVER", 9, NTADR_A(11, 8));

	if( winner == ONETWO_WINNER)
	{
		multi_vram_buffer_horz("TEAM 1 WINS!", 12, NTADR_A(9, 12));
		
	}
	else if (winner == THREEFOUR_WINNER)
	{
		multi_vram_buffer_horz("TEAM 2 WINS!", 12, NTADR_A(9, 12));
		
	}
	if (win_reason == WIN_DOTS) {
		multi_vram_buffer_horz("COLLECTED 100 DOTS", 18, NTADR_A(6, 14));
	} else if (win_reason == WIN_FRIENDLY_FIRE) {
		multi_vram_buffer_horz("FRIENDLY FIRE KILL", 18, NTADR_A(6, 14));
	} else if (win_reason == WIN_ENEMY_KILL) {
		multi_vram_buffer_horz("ENEMY SEEKER KILLED", 19, NTADR_A(5, 14));
	} 
	// say who won
	// draw on screen, probably an unrle eventually for this game
	// probably lets the players select ai or player.
	multi_vram_buffer_horz("PRESS START", 11, NTADR_A(10, 24));
 
	ppu_on_all(); // turn on screen
}

void init_system(void){
	ppu_off();
	clear_vram_buffer();
	set_vram_buffer();
	bank_spr(1);
	set_scroll_y(0xff); //shift the bg down one pixel
	ppu_on_all(); // turn on screenxw
}