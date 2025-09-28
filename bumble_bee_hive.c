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
	 TODO before CORGS:
	 * win count [x]
	 * fix colors [x]
	 * power up pellet [x]
	 * game over screen [x]
	 * fix sounds on win [x]
	 * character placements [x]
	 * skip options screen? [x] //dune in debug
	 * 
	 * 
	 * settings screen / pad_new?
	 * New maps, new music
	 * turbo button?
	*/

	/*
	Ideas:
	turbo for duck, half second with 7 second recharge?
	bee turbo uses up honey?
	duck can eat through 1 wall?

	random placement but symmetrical

	win count / medals
	timer for game (40 seconds?)
	ready, fight! logo. (or 3,2,1 blitz )

	character sprites are a little too big hitboxes?
	Big dots worth more points 
	Power up to swap places with your partner. 
	
	*/

	init_system();
	init_title_loop();

	// idk why this code is in here -agf 7/29
	//  one_vram_buffer(0x63, NTADR_A(17, 1));
	//  one_vram_buffer(0x63, NTADR_A(21, 1));
	//  one_vram_buffer(0x63, NTADR_A(22, 1));

	while (1)
	{
		if (game_mode == MODE_TITLE)
		{
			title_loop();
		}
		if (game_mode == MODE_OPTIONS)
		{
			options_loop();
		}
		if (game_mode == MODE_GAME)
		{
			game_loop();
		}
		if (game_mode == MODE_GAMEOVER)
		{
			gameover_loop();
		}
		if(game_mode== MODE_ROUNDOVER){
			roundover_loop();
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
		if(game_mode == MODE_TITLE){
			vram_put(title[largeindex]);
		} else if (game_mode == MODE_OPTIONS){
			vram_put(settings[largeindex]);
		} else{
			vram_put(combmapwoflowers[largeindex]);
		}
		
		flush_vram_update2();
	}
	ppu_on_all();
}

void draw_sprites(void)
{
	// clear all sprites from sprite buffer
	oam_clear();

	//too many on a line potential, need to rotate through them.

	//draw powerups
	if(powerup1 == 1)
		oam_meta_spr(16, 32, gamesprites_powerup_data);
	if(powerup2 == 1)
		oam_meta_spr(226, 32, gamesprites_powerup_data);
	if(powerup3 == 1)
		oam_meta_spr(16, 208, gamesprites_powerup_data);
	if(powerup4 == 1)
		oam_meta_spr(226, 208, gamesprites_powerup_data);

	// Cycle through different drawing orders based on sprite_rotation
	// This prevents sprites from always being drawn in the same order
	switch(sprite_rotation & 0x03) { // Use only bottom 2 bits for 4 different orders
		case 0: // Original order: 1, 2, 3, 4
			draw_player_1();
			draw_player_2();
			draw_player_3();
			draw_player_4();
			break;
			
		case 1: // Order: 2, 1, 4, 3
			draw_player_2();
			draw_player_1();
			draw_player_4();
			draw_player_3();
			break;
			
		case 2: // Order: 3, 4, 1, 2
			draw_player_3();
			draw_player_4();
			draw_player_1();
			draw_player_2();
			break;
			
		case 3: // Order: 4, 3, 2, 1
			draw_player_4();
			draw_player_3();
			draw_player_2();
			draw_player_1();
			break;
	}
}

void draw_player_1(void)
{
	temp_x = BoxGuy1.x >> 8;
	temp_y = BoxGuy1.y >> 8;
	
	// Update animation frame every 10 frames
	if ((frame_counter % 10) == 0) {
		anim_frame_1 = (anim_frame_1 + 1) % 3;
	}
	
	// Check if player is in bigbee form
	if (bee1_bigbee_timer > 0) {

		if(bee1_bigbee_timer < 5){
			// Flicker effect when bigbee is about to expire
			if ((frame_counter % 4) < 2) {
				return; // Skip drawing this frame
			}
		}
		// Draw bigbee sprites
		if (BoxGuy1.direction == DIR_LEFT) {
			switch(anim_frame_1) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft2_data); break;
			}
		} else if (BoxGuy1.direction == DIR_RIGHT) {
			switch(anim_frame_1) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeright0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeright1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeright2_data); break;
			}
		} else {
			// Default to left-facing sprite for UP/DOWN/NONE
			switch(anim_frame_1) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft2_data); break;
			}
		}
	} else {
		// Draw normal small bee sprites
		if (BoxGuy1.direction == DIR_LEFT) {
			switch(anim_frame_1) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee1left0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft2_data); break;
			}
		} else if (BoxGuy1.direction == DIR_RIGHT) {
			switch(anim_frame_1) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright2_data); break;
			}
		} else {
			// Default to left-facing sprite for UP/DOWN/NONE
			switch(anim_frame_1) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee1left0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft2_data); break;
			}
		}
	}
}

void draw_player_2(void)
{
	temp_x = BoxGuy2.x >> 8;
	temp_y = BoxGuy2.y >> 8;
	
	// Update animation frame every 10 frames, but only if player is moving
	if ((frame_counter % 10) == 0 && BoxGuy2.moving) {
		anim_frame_2 = (anim_frame_2 + 1) % 3;
	}
	
	// Choose sprite based on direction and animation frame
	// Note: Duck sprites have limited right-facing variants, so we'll use left-facing for most cases
	if (BoxGuy2.direction == DIR_LEFT) {
		switch(anim_frame_2) { 
			
			case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck1left0_data); break;
			case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft1_data); break;
			case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft2_data); break;
		}
	} else if (BoxGuy2.direction == DIR_RIGHT) {
		// Use left-facing sprites with horizontal flip for right direction
		switch(anim_frame_2) {
			case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck1right0_data); break;
			case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallduckright1_data); break;
			case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallduckright2_data); break;
		}
	} else {
		// Default to left-facing sprite for UP/DOWN/NONE
		switch(anim_frame_2) {
			case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck1left0_data); break;
			case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft1_data); break;
			case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft2_data); break;
		}
	}
}

void draw_player_3(void)
{
	temp_x = BoxGuy3.x >> 8;
	temp_y = BoxGuy3.y >> 8;
	
	// Update animation frame every 10 frames
	if ((frame_counter % 10) == 0) {
		anim_frame_3 = (anim_frame_3 + 1) % 3;
	}
	
	// Check if player is in bigbee form
	if (bee3_bigbee_timer > 0) {

		if(bee3_bigbee_timer < 5){
			// Flicker effect when bigbee is about to expire
			if ((frame_counter % 4) < 2) {
				return; // Skip drawing this frame
			}
		}

		// Draw bigbee sprites (using palette 1 for player 3)
		if (BoxGuy3.direction == DIR_LEFT) {
			switch(anim_frame_3) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left2_data); break;
			}
		} else if (BoxGuy3.direction == DIR_RIGHT) {
			switch(anim_frame_3) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2right0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2right1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2right2_data); break;
			}
		} else {
			// Default to left-facing sprite for UP/DOWN/NONE
			switch(anim_frame_3) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left2_data); break;
			}
		}
	} else {
		// Draw normal small bee sprites
		if (BoxGuy3.direction == DIR_LEFT) {
			switch(anim_frame_3) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left2_data); break;
			}
		} else if (BoxGuy3.direction == DIR_RIGHT) {
			switch(anim_frame_3) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2right0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2right1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2right2_data); break;
			}
		} else {
			// Default to left-facing sprite for UP/DOWN/NONE
			switch(anim_frame_3) {
				case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left0_data); break;
				case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left1_data); break;
				case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left2_data); break;
			}
		}
	}
}

void draw_player_4(void)
{
	temp_x = BoxGuy4.x >> 8;
	temp_y = BoxGuy4.y >> 8;
	
	// Update animation frame every 10 frames, but only if player is moving
	if ((frame_counter % 10) == 0 && BoxGuy4.moving) {
		anim_frame_4 = (anim_frame_4 + 1) % 3;
	}
	
	// Choose sprite based on direction and animation frame
	// Note: Duck sprites have limited right-facing variants, so we'll use left-facing for most cases
	if (BoxGuy4.direction == DIR_LEFT) {
		switch(anim_frame_4) {
			case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left0_data); break;
			case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left1_data); break;
			case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left2_data); break;
		}
	} else if (BoxGuy4.direction == DIR_RIGHT) {
		// Use left-facing sprites with horizontal flip for right direction
		switch(anim_frame_4) {
			case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2right0_data); break;
			case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2right1_data); break;
			case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2right2_data); break;
		}
	} else {
		// Default to left-facing sprite for UP/DOWN/NONE
		switch(anim_frame_4) {
			case 0: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left0_data); break;
			case 1: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left1_data); break;
			case 2: oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left2_data); break;
		}
	}
}

void update_hud(void){
	if(team1_wins > 0){
		one_vram_buffer(0xc8, NTADR_A(13, 1)); //full
	}else {
		one_vram_buffer(0xb7, NTADR_A(13, 1)); //empty
	}
	if(team1_wins > 1){
		one_vram_buffer(0xc9, NTADR_A(12, 1)); //full
	} else {
		one_vram_buffer(0xb8, NTADR_A(12, 1)); //empty
	}
	if(team1_wins > 2){
		one_vram_buffer(0xca, NTADR_A(11, 1)); //full
	} else {
		one_vram_buffer(0xb9, NTADR_A(11, 1)); //empty
	}
	

	if(team2_wins > 0){
		one_vram_buffer(0xc8, NTADR_A(18, 1)); //full
	} else {
		one_vram_buffer(0xb7, NTADR_A(18, 1)); //empty
	}
	if(team2_wins > 1){
		one_vram_buffer(0xc9, NTADR_A(19, 1)); //full
	} else {
		one_vram_buffer(0xb8, NTADR_A(19, 1)); //empty
	}
	if(team2_wins > 2){
		one_vram_buffer(0xca, NTADR_A(20, 1	)); //full
	} else {
		one_vram_buffer(0xb9, NTADR_A(20, 1)); //empty
	}
	
}

void movement(void)
{
	// handle x
	old_x = GenericBoxGuy.x;

	if (generic_pad & PAD_A) {
    // Check which player and if they have turbo available
    if ((current_player == 1 && turbo_p1 > 0) ||
        (current_player == 2 && turbo_p2 > 0) ||
        (current_player == 3 && turbo_p3 > 0) ||
        (current_player == 4 && turbo_p4 > 0)) {
        
				use_turbo = 1;
        switch(current_player) {
            case 1: turbo_p1--; break;
            case 2: turbo_p2--; break;
            case 3: turbo_p3--; break;
            case 4: turbo_p4--; break;
        }
    }
	}

	if (generic_pad & PAD_LEFT)
	{
		hero_velocity_x = -speed_option;
		if(use_turbo == 1){
			hero_velocity_x -= SPEED_TURBO_BOOST; // add turbo boost
			use_turbo = 0; // reset turbo flag
		}
		GenericBoxGuy.direction = DIR_LEFT;
	}
	else if (generic_pad & PAD_RIGHT)
	{
		hero_velocity_x = speed_option;
		if(use_turbo == 1){
			hero_velocity_x += SPEED_TURBO_BOOST; // add turbo boost
			use_turbo = 0; // reset turbo flag
		}
		GenericBoxGuy.direction = DIR_RIGHT;
	}
	else
	{ // nothing pressed
		hero_velocity_x = 0;
		// Don't change direction here - let Y movement override if needed
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
		{													 // check collision left
			GenericBoxGuy.x = old_x; // revert to old position
		}
	}
	else if (hero_velocity_x > 0)
	{ // going right
		if (bg_coll_R())
		{													 // check collision right
			GenericBoxGuy.x = old_x; // revert to old position
		}
	}
	// else 0, skip it

	// handle y
	old_y = GenericBoxGuy.y;

	if (generic_pad & PAD_UP)
	{ 
		hero_velocity_y = -speed_option;
		if(use_turbo == 1){
			hero_velocity_y -= SPEED_TURBO_BOOST; // add turbo boost
			use_turbo = 0; // reset turbo flag
		}  
		// GenericBoxGuy.direction = DIR_UP; //entually we'll set up and down but not now.
	}
	else if (generic_pad & PAD_DOWN)
	{
		hero_velocity_y = speed_option;
		if(use_turbo == 1){
			hero_velocity_y += SPEED_TURBO_BOOST; // add turbo boost
			use_turbo = 0; // reset turbo flag
		}
		// GenericBoxGuy.direction = DIR_DOWN;
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
	// Collision box size already set above for X movement

	if (hero_velocity_y < 0)
	{ // going up
		if (bg_coll_U())
		{													 // check collision up
			GenericBoxGuy.y = old_y; // revert to old position
		}
	}
	else if (hero_velocity_y > 0)
	{ // going down
		if (bg_coll_D())
		{													 // check collision down
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

	temp = combmapwoflowers[((temp_y >> 3) << 5) + (temp_x >> 3)];
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
char is_dot_consumed(unsigned int map_index)
{
	unsigned char byte_index = map_index >> 3;	// divide by 8
	unsigned char bit_index = map_index & 0x07; // modulo 8
	return (consumed_dots[byte_index] >> bit_index) & 0x01;
}

// Helper function to mark a dot as consumed
void mark_dot_consumed(unsigned int map_index)
{
	unsigned char byte_index = map_index >> 3;	// divide by 8
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
	temp = combmapwoflowers[largeindex];

	for (index = 0; index < 5; index++)
	{
		if (temp == pellet_tiles[index])
		{
			// Check if this dot has already been consumed
			if (!is_dot_consumed(largeindex))
			{
				// Mark this dot as consumed
				mark_dot_consumed(largeindex);
				// Update the screen
				one_vram_buffer(blank_tiles[frame_counter % 5], NTADR_A(temp_x, temp_y));
				// update team score
				if (current_player == 1)
				{
					sfx_play(SFX_TEAM1_DOT_COLLECT, 0);
					team1_score++;
					// Check for win condition
					if (team1_score >= 100)
					{
						sfx_play(SFX_TEAM1_WIN, 0);
						winner = ONETWO_WINNER;
						win_reason = WIN_DOTS;
						init_roundover();
						return;
					}
				}
				else if (current_player == 3)
				{
					sfx_play(SFX_TEAM2_DOT_COLLECT, 0);
					team2_score++;
					// Check for win condition
					if (team2_score >= 100)
					{
						sfx_play(SFX_TEAM2_WIN, 0);
						winner = THREEFOUR_WINNER;
						win_reason = WIN_DOTS;
						init_roundover();
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

// Check if a player collides with a powerup
char check_powerup_collision(unsigned char player_x, unsigned char player_y, unsigned char powerup_x, unsigned char powerup_y)
{
	// Powerup is 16x16 pixels, player is 8x8 pixels
	// Check if player's center is within powerup bounds
	if (player_x + 4 >= powerup_x && player_x + 4 < powerup_x + 16 &&
			player_y + 4 >= powerup_y && player_y + 4 < powerup_y + 16)
	{
		return 1;
	}
	return 0;
}

void debug_extras(void)
{
	// Display team 1 score (2 digits)
	temp1 = (team1_score / 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(6, 1));
	temp1 = (team1_score % 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(7, 1));

	// Timer
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

void read_controllers(void)
{
	// read controllers 1 and 3 into an integer
	doublepad = pad_poll_4score_1_3();
	pad1 = high_byte(doublepad); // the high byte is the first controller's input
	pad3 = low_byte(doublepad);	 // low byte is the third controller's input

	// read controllers 2 and 4 into an integer
	doublepad = pad_poll_4score_2_4();
	pad2 = high_byte(doublepad);
	pad4 = low_byte(doublepad);

	// seeker_ai();
	// chaser_ai();

	// debug just random values
	//  [0b00010000,0b00100000,0b01000000,0b10000000]
}

void seeker_ai(void)
{ // ai for the bee
	/*
		avoid the nearest enemy chaser (player 2)
		avoid the friendly chaser (player 4)
		move towards nearest dot
	*/
	// pad3= PAD_DOWN;
}

void chaser_ai(void)
{ // ai for the duck
	/*
			move towards the nearest enemy seeker
			avoid the friendly seeker
		*/
	// pad2 = PAD_LEFT;
	// pad4 = PAD_LEFT;
}

void game_loop(void)
{
	// wait till beginning of the frame
	ppu_wait_nmi();

	// 0. DEBUGGING CODE
	debug_extras();
	 // this should just move to the chr stuff

	// 1. INCREMENT GLOBAL COUNTERS
	frame_counter++;
	game_frame_timer++;
	if (game_frame_timer >= TIMER_TICK_FREQUENCY) // Tick down every ~24.24 frames (40s/99)
	{
		game_frame_timer = 0; // reset the frame timer
		game_timer--;
		if (game_timer == 0)
		{
			// time's up, check scores
			if (team1_score > team2_score)
			{
				winner = ONETWO_WINNER;
				win_reason = WIN_TIME_UP;
				sfx_play(SFX_TEAM1_WIN, 0);
			}
			else if (team2_score > team1_score)
			{
				winner = THREEFOUR_WINNER;
				win_reason = WIN_TIME_UP;
				sfx_play(SFX_TEAM2_WIN, 0);
			}
			else
			{
				winner = TIE_WINNER;
				win_reason = WIN_TIME_UP;
				sfx_play(SFX_DRAW_GAME, 0);
			}
			init_roundover();
		}
	}
	ai_counter++;
	
	// Update sprite rotation every frame
	sprite_rotation++;
	
	// Update bigbee transformation timers
	if (bee1_bigbee_timer > 0 && game_frame_timer == 1) {
		bee1_bigbee_timer--;
	}
	if (bee3_bigbee_timer > 0 && game_frame_timer == 1) { //game timer
		bee3_bigbee_timer--;
	}

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
	movement(); // this assigns old_x and old_y
	BoxGuy1.direction = GenericBoxGuy.direction; // keep direction updated for animation
	temp_x = GenericBoxGuy.x >> 8;
	temp_y = GenericBoxGuy.y >> 8;
	temp_x2 = BoxGuy3.x >> 8;
	temp_y2 = BoxGuy3.y >> 8;
	if (sprite_collision()) // 1 and 3, both seekers
	{
		// players bounce off each other
		BoxGuy1.x = old_x;
		BoxGuy1.y = old_y;
	}
	else
	{
		BoxGuy1.x = GenericBoxGuy.x;
		BoxGuy1.y = GenericBoxGuy.y;
	}

	// setup generics for player 2 (chaser)
	current_player = 2;
	GenericBoxGuy = BoxGuy2;
	generic_pad = pad2;
	movement();
	BoxGuy2.direction = GenericBoxGuy.direction; // keep direction updated for animation
	
	// For ducks, set direction to NONE if no input is pressed
	if (!(pad2 & (PAD_LEFT | PAD_RIGHT | PAD_UP | PAD_DOWN))) {
		BoxGuy2.moving = 0;
	} else {
		BoxGuy2.moving = 1;
	}

	temp_x = GenericBoxGuy.x >> 8;
	temp_y = GenericBoxGuy.y >> 8;
	temp_x2 = BoxGuy4.x >> 8;
	temp_y2 = BoxGuy4.y >> 8;
	if (sprite_collision()) // player 2 blocks player 4 (chasers)
	{
		// players bounce off each other
		BoxGuy2.x = old_x;
		BoxGuy2.y = old_y;
	}
	else
	{
		BoxGuy2.x = GenericBoxGuy.x;
		BoxGuy2.y = GenericBoxGuy.y;
	}

	// setup generics for player 3 (seeker)
	current_player = 3;
	GenericBoxGuy = BoxGuy3;
	generic_pad = pad3;
	movement();
	// player 1 blocks player 3
	BoxGuy3.direction = GenericBoxGuy.direction; // keep direction updated for animation
	temp_x = BoxGuy1.x >> 8;
	temp_y = BoxGuy1.y >> 8;
	temp_x2 = GenericBoxGuy.x >> 8;
	temp_y2 = GenericBoxGuy.y >> 8;
	if (sprite_collision()) // 1 blocks 3 (seekers)
	{
		// players bounce off each other
		BoxGuy3.x = old_x;
		BoxGuy3.y = old_y;
	}
	else
	{
		BoxGuy3.x = GenericBoxGuy.x;
		BoxGuy3.y = GenericBoxGuy.y;
	}

	// setup generics for player 4 (chaser)
	current_player = 4;
	GenericBoxGuy = BoxGuy4;
	generic_pad = pad4;
	movement();
	BoxGuy4.direction = GenericBoxGuy.direction; // keep direction updated for animation
	
	// For ducks, set direction to NONE if no input is pressed
	if (!(pad4 & (PAD_LEFT | PAD_RIGHT | PAD_UP | PAD_DOWN))) {
		BoxGuy4.moving = 0;
	} else {
		BoxGuy4.moving = 1;
	}
	// player 2 blocks player 4
	temp_x = BoxGuy2.x >> 8;
	temp_y = BoxGuy2.y >> 8;
	temp_x2 = GenericBoxGuy.x >> 8;
	temp_y2 = GenericBoxGuy.y >> 8;
	if (sprite_collision()) // 2 blocks 4 (seekers)
	{
		// players bounce off each other
		BoxGuy4.x = old_x;
		BoxGuy4.y = old_y;
	}
	else
	{
		BoxGuy4.x = GenericBoxGuy.x;
		BoxGuy4.y = GenericBoxGuy.y;
	}

	// 4. CHECK POWERUP COLLISIONS
	
	// Check if bee players (1 and 3) collide with powerups
	if (bee1_bigbee_timer == 0) { // Only check if not already transformed
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;
		
		// Check powerup1 (top-left)
		if (powerup1 == 1 && check_powerup_collision(temp_x, temp_y, 16, 32)) {
			powerup1 = 0; // Remove powerup
			bee1_bigbee_timer = POWERUP_TIMER;  // 20 game seconds
			sfx_play(SFX_POWERUP, 0);
		}
		// Check powerup2 (top-right)
		if (powerup2 == 1 && check_powerup_collision(temp_x, temp_y, 226, 32)) {
			powerup2 = 0; // Remove powerup
			bee1_bigbee_timer = POWERUP_TIMER; // 20 game seconds
			sfx_play(SFX_POWERUP, 0);
		}
		// Check powerup3 (bottom-left)
		if (powerup3 == 1 && check_powerup_collision(temp_x, temp_y, 16, 208)) {
			powerup3 = 0; // Remove powerup
			bee1_bigbee_timer = POWERUP_TIMER; // 20 game seconds
			sfx_play(SFX_POWERUP, 0);
		}
		// Check powerup4 (bottom-right)
		if (powerup4 == 1 && check_powerup_collision(temp_x, temp_y, 226, 208)) {
			powerup4 = 0; // Remove powerup
			bee1_bigbee_timer = POWERUP_TIMER; // 20 game seconds
			sfx_play(SFX_POWERUP, 0);
		}
	}
	
	if (bee3_bigbee_timer == 0) { // Only check if not already transformed
		temp_x = BoxGuy3.x >> 8;
		temp_y = BoxGuy3.y >> 8;
		
		// Check powerup1 (top-left)
		if (powerup1 == 1 && check_powerup_collision(temp_x, temp_y, 16, 32)) {
			powerup1 = 0; // Remove powerup
			bee3_bigbee_timer = POWERUP_TIMER; // 20 game seconds
			sfx_play(SFX_POWERUP, 0);
		}
		// Check powerup2 (top-right)
		if (powerup2 == 1 && check_powerup_collision(temp_x, temp_y, 226, 32)) {
			powerup2 = 0; // Remove powerup
			bee3_bigbee_timer = POWERUP_TIMER; // 20 game seconds
			sfx_play(SFX_POWERUP, 0);
		}
		// Check powerup3 (bottom-left)
		if (powerup3 == 1 && check_powerup_collision(temp_x, temp_y, 16, 208)) {
			powerup3 = 0; // Remove powerup
			bee3_bigbee_timer = POWERUP_TIMER; // 20 game seconds
			sfx_play(SFX_POWERUP, 0);
		}
		// Check powerup4 (bottom-right)
		if (powerup4 == 1 && check_powerup_collision(temp_x, temp_y, 226, 208)) {
			powerup4 = 0; // Remove powerup
			bee3_bigbee_timer = POWERUP_TIMER; // 20 game seconds
			sfx_play(SFX_POWERUP, 0);
		}
	}

	// 5. CHECK COLLISON


	// 5. CHECK BIGBEE EATING DUCKS
	
	// Check if bigbee player 1 eats any duck
	if (bee1_bigbee_timer > 0) {
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;
		
		// Check if bigbee player 1 eats friendly duck (player 2) - LOSE
		temp_x2 = BoxGuy2.x >> 8;
		temp_y2 = BoxGuy2.y >> 8;
		if (sprite_collision()) {
			sfx_play(SFX_TEAM2_WIN, 0);
			winner = THREEFOUR_WINNER;
			win_reason = WIN_BIGBEE_EAT_DUCK;
			init_roundover();
			return;
		}
		
		// Check if bigbee player 1 eats enemy duck (player 4) - WIN
		temp_x2 = BoxGuy4.x >> 8;
		temp_y2 = BoxGuy4.y >> 8;
		if (sprite_collision()) {
			sfx_play(SFX_TEAM1_WIN, 0);
			winner = ONETWO_WINNER;
			win_reason = WIN_BIGBEE_EAT_DUCK;
			init_roundover();
			return;
		}
	}
	
	// Check if bigbee player 3 eats any duck
	if (bee3_bigbee_timer > 0) {
		temp_x = BoxGuy3.x >> 8;
		temp_y = BoxGuy3.y >> 8;
		
		// Check if bigbee player 3 eats friendly duck (player 4) - LOSE
		temp_x2 = BoxGuy4.x >> 8;
		temp_y2 = BoxGuy4.y >> 8;
		if (sprite_collision()) {
			sfx_play(SFX_TEAM1_WIN, 0);
			winner = ONETWO_WINNER;
			win_reason = WIN_BIGBEE_EAT_DUCK;
			init_roundover();
			return;
		}
		
		// Check if bigbee player 3 eats enemy duck (player 2) - WIN
		temp_x2 = BoxGuy2.x >> 8;
		temp_y2 = BoxGuy2.y >> 8;
		if (sprite_collision()) {
			sfx_play(SFX_TEAM2_WIN, 0);
			winner = THREEFOUR_WINNER;
		  win_reason = WIN_BIGBEE_EAT_DUCK;
			init_roundover();
			return;
		}
	}

	// check for player deaths (1 collide with 2, 3 collide with 4)
	temp_x = BoxGuy1.x >> 8;
	temp_y = BoxGuy1.y >> 8;
	temp_x2 = BoxGuy2.x >> 8;
	temp_y2 = BoxGuy2.y >> 8;
	if (sprite_collision())
	{
		// player 1 dies (friendly fire)
		sfx_play(SFX_TEAM2_WIN, 0);
		winner = THREEFOUR_WINNER;
		win_reason = WIN_FRIENDLY_FIRE;
		init_roundover();
		return;
	}
	temp_x = BoxGuy3.x >> 8;
	temp_y = BoxGuy3.y >> 8;
	temp_x2 = BoxGuy4.x >> 8;
	temp_y2 = BoxGuy4.y >> 8;
	if (sprite_collision())
	{
		// player 3 dies (enemy fire)
		sfx_play(SFX_TEAM1_WIN, 0);
		winner = ONETWO_WINNER;
		win_reason = WIN_FRIENDLY_FIRE;
		init_roundover();
		return;
	}
	// check 1 with 4 and 2 with 3
	temp_x = BoxGuy1.x >> 8;
	temp_y = BoxGuy1.y >> 8;
	temp_x2 = BoxGuy4.x >> 8;
	temp_y2 = BoxGuy4.y >> 8;
	if (sprite_collision())
	{
		// player 1 dies (enemy fire)
		sfx_play(SFX_TEAM2_WIN, 0);
		winner = THREEFOUR_WINNER;
		win_reason = WIN_ENEMY_KILL;
		init_roundover();
		return;
	}
	temp_x = BoxGuy2.x >> 8;
	temp_y = BoxGuy2.y >> 8;
	temp_x2 = BoxGuy3.x >> 8;
	temp_y2 = BoxGuy3.y >> 8;
	if (sprite_collision())
	{
		// player 2 dies (enemy fire)
		sfx_play(SFX_TEAM1_WIN, 0);
		winner = ONETWO_WINNER;
		win_reason = WIN_ENEMY_KILL;
		init_roundover();
		return;
	}


	// 6. DRAW SPRITES
	draw_sprites();
}

void title_loop(void)
{
	while (1)
	{
		frame_counter++;
		ppu_wait_nmi();

		// Read all controllers for title screen
		read_controllers();

		// Handle up movement for each player
		if (pad1 & PAD_UP)
		{
			if ((BoxGuy1.y >> 8) > 160)
			{											 // Limit how high they can go
				BoxGuy1.y -= 0x0400; // Move up 4 pixels (0x0400 = 1024 sub-pixels)
			}
		}
		if (pad2 & PAD_UP)
		{
			if ((BoxGuy2.y >> 8) > 160)
			{
				BoxGuy2.y -= 0x0400;
			}
		}
		if (pad3 & PAD_UP)
		{
			if ((BoxGuy3.y >> 8) > 160)
			{
				BoxGuy3.y -= 0x0400;
			}
		}
		if (pad4 & PAD_UP)
		{
			if ((BoxGuy4.y >> 8) > 160)
			{
				BoxGuy4.y -= 0x0400;
			}
		}

		// Handle start button hold logic
		if (pad1 & PAD_START || pad2 & PAD_START || pad3 & PAD_START || pad4 & PAD_START)
		{
			if (!start_held)
			{
				start_held = 1;
				start_hold_timer = 0;
			}
			start_hold_timer++;

			// Check if start has been held for 3 seconds (180 frames at 60fps)
			if (start_hold_timer >= 3)
			{
				init_options_loop();
				break;
			}
		}
		else
		{
			start_held = 0;
			start_hold_timer = 0;
		}

	}
}

void options_loop(void)
{
	while (1)
	{
		ppu_wait_nmi();
		frame_counter++;

		// Read all controllers for options screen
		read_controllers();

		// if (pad1 != prev_pad1 || pad2 != prev_pad2 || pad3 != prev_pad3 || pad4 != prev_pad4)
		// {

		// 	// Handle speed selection with left/right
		// 	if (pad1 & PAD_LEFT || pad2 & PAD_LEFT || pad3 & PAD_LEFT || pad4 & PAD_LEFT)
		// 	{
		// 		if (speed_option == SPEED_FAST)
		// 		{
		// 			speed_option = SPEED_REGULAR;  
		// 			force_redraw = 1;
		// 		} else if (speed_option == SPEED_REGULAR)
		// 		{
		// 			speed_option = SPEED_SLOW;
		// 			force_redraw = 1;
		// 		}
		// 	}

		// 	if (pad1 & PAD_RIGHT || pad2 & PAD_RIGHT || pad3 & PAD_RIGHT || pad4 & PAD_RIGHT)
		// 	{
		// 		if (speed_option == SPEED_SLOW)
		// 		{
		// 			speed_option = SPEED_REGULAR;
		// 			force_redraw = 1;
		// 		}
		// 		else if (speed_option == SPEED_REGULAR)
		// 		{
		// 			speed_option = SPEED_FAST;
		// 			force_redraw = 1;
		// 		}
		// 	}
		// }

		// if (force_redraw)
		// {
		// 	force_redraw = 0;
		// 	// Redraw the speed text
		// 	ppu_off();
		// 	clear_vram_buffer();

		// 	multi_vram_buffer_horz("SPEED:", 7, NTADR_A(8, 10));

		// 	if (speed_option == SPEED_SLOW)
		// 	{
		// 		multi_vram_buffer_horz("  SLOW ", 7, NTADR_A(11, 12));
		// 	}
		// 	else if (speed_option == SPEED_REGULAR)
		// 	{
		// 		multi_vram_buffer_horz("REGULAR", 7, NTADR_A(11, 12));
		// 	}
		// 	else
		// 	{
		// 		multi_vram_buffer_horz("  FAST ", 7, NTADR_A(11, 12));
		// 	}

		// 	multi_vram_buffer_horz("MAP:", 4, NTADR_A(8, 14));

		// 	multi_vram_buffer_horz("MUSIC:", 5, NTADR_A(8, 16));

		// 	ppu_on_all();
		// }

		// Handle start button hold logic
		if (pad1 & PAD_START || pad2 & PAD_START || pad3 & PAD_START || pad4 & PAD_START)
		{
			sfx_play(SFX_START, 0);
			init_game_loop();
			break;
		}
		
	}

	prev_pad1 = pad1;
	prev_pad2 = pad2;
	prev_pad3 = pad3;
	prev_pad4 = pad4;
}

void gameover_loop(void)
{

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

void start_round(void){

	load_room();
	update_hud();
	// clear score counts

	for (index = 0; index < 128; index++)
	{
		consumed_dots[index] = 0;
	}
	// move all players into starting positions based on configuration:
	map_positions = (frame_counter) % 5; //this could be round based.

	if (map_positions == START_POS_WIDE) {
	    BoxGuy1.x = 0x3800; BoxGuy1.y = 0xA800;
	    BoxGuy2.x = 0x5000; BoxGuy2.y = 0x2800;
	    BoxGuy3.x = 0xC000; BoxGuy3.y = 0xA800;
	    BoxGuy4.x = 0xA000; BoxGuy4.y = 0x2800;
	} else if (map_positions == START_POS_CLOSE) {
	    BoxGuy1.x = 0x5800; BoxGuy1.y = 0x3800;
	    BoxGuy2.x = 0x7000; BoxGuy2.y = 0x3000;
	    BoxGuy3.x = 0xA000; BoxGuy3.y = 0x3800;
	    BoxGuy4.x = 0x8800; BoxGuy4.y = 0x3000;
	} else if (map_positions == START_POS_CORNERS) {
	    BoxGuy1.x = 0x2800; BoxGuy1.y = 0x2000;
	    BoxGuy2.x = 0x2800; BoxGuy2.y = 0xd800;
	    BoxGuy3.x = 0xD800; BoxGuy3.y = 0x2000;
	    BoxGuy4.x = 0xD800; BoxGuy4.y = 0xD800;
	} else if (map_positions == START_POS_MIDDLE) {
	    BoxGuy1.x = 0x7800; BoxGuy1.y = 0x8000;
	    BoxGuy2.x = 0x7800; BoxGuy2.y = 0x2800;
	    BoxGuy3.x = 0x8000; BoxGuy3.y = 0x8000;
	    BoxGuy4.x = 0x8000; BoxGuy4.y = 0x2800;
			BoxGuy3.direction = DIR_RIGHT;
			BoxGuy4.direction = DIR_RIGHT;
	} else {  // START_POS_DEFAULT
	    BoxGuy1.x = 0x4800; BoxGuy1.y = 0x4000;
	    BoxGuy2.x = 0x7000; BoxGuy2.y = 0x2800;
	    BoxGuy3.x = 0xB000; BoxGuy3.y = 0x4000;
	    BoxGuy4.x = 0x8800; BoxGuy4.y = 0x2800;
			BoxGuy4.direction = DIR_RIGHT;
			BoxGuy1.direction = DIR_RIGHT;
	}
	team1_score=0;
	team2_score=0;
	powerup1 =1;
	powerup2 =1;
	powerup3 =1;
	powerup4 =1;
	game_timer = GAME_LENGTH;
	win_reason = WIN_DOTS; // default
	
	// Reset turbo counters for all players
	turbo_p1 = turbo_amount;
	turbo_p2 = turbo_amount;
	turbo_p3 = turbo_amount;
	turbo_p4 = turbo_amount;
	
	// Reset bigbee transformation timers
	bee1_bigbee_timer = 0;
	bee3_bigbee_timer = 0;
	

	ppu_wait_nmi();
	oam_clear();
	oam_meta_spr(116, 100, gamesprites_big3_data);
	sfx_play(SFX_TEAM1_DOT_COLLECT, 0);
	ppu_wait_nmi();
	delay(40);
	oam_clear();
	ppu_wait_nmi();
	oam_meta_spr(116, 100, gamesprites_big2_data);
	sfx_play(SFX_TEAM1_DOT_COLLECT, 0);
	delay(40);
	oam_clear();
	ppu_wait_nmi();
	oam_meta_spr(116, 100, gamesprites_big1_data);
	sfx_play(SFX_TEAM1_DOT_COLLECT, 0);
	delay(40);
	oam_clear();
	ppu_wait_nmi();
	sfx_play(SFX_START, 0);
	delay(10);
	ppu_wait_nmi();

	game_mode = MODE_GAME;
	song = SONG_MAIN_SONG;
	music_play(song);


}

void init_game_loop(void)
{
	clear_background();
	
	game_mode = MODE_GAME;
	// Initialize scores
	team1_wins = 0;
	team2_wins = 0;
	// load the palettes
	pal_bg(palette_bg_combmap);
	pal_spr(palette_sp);

	start_round();
}

void init_title_loop(void)
{
	delay(30);
	game_mode = MODE_TITLE;
	ppu_off(); // screen off
	// load the title palettes
	pal_bg(palette_title_bg);
	pal_spr(palette_sp);

	load_room(); //loads the title screen;

	// Initialize title screen sprites at bottom of screen
	BoxGuy1.x = 0x2800; // x = 40 (0x28)
	BoxGuy1.y = 0xC800; // y = 200 (0xC8)
	BoxGuy1.direction = DIR_NONE;

	BoxGuy2.x = 0x7800; // x = 120 (0x78)
	BoxGuy2.y = 0xC800; // y = 200 (0xC8)
	BoxGuy2.direction = DIR_NONE;

	BoxGuy3.x = 0xC800; // x = 200 (0xC8)
	BoxGuy3.y = 0xC800; // y = 200 (0xC8)
	BoxGuy3.direction = DIR_NONE;

	BoxGuy4.x = 0x1801; // x = 280 (0x118)
	BoxGuy4.y = 0xC800; // y = 200 (0xC8)
	BoxGuy4.direction = DIR_NONE;


	ppu_on_all(); // turn on screen
}

void init_options_loop(void)
{
	//DEBUG
	// init_game_loop();
	// return;

	//real code
	delay(30);
	prev_pad1 = 0;
	prev_pad2 = 0;
	prev_pad3 = 0;
	prev_pad4 = 0;

	clear_background();  
	game_mode = MODE_OPTIONS;
	ppu_off(); // screen off

	load_room(); // loads the options screen

	// load the title palettes
	pal_bg(palette_options_bg);
	pal_spr(palette_sp);

	multi_vram_buffer_horz("SPEED:", 7, NTADR_A(8, 10));

	// Show current speed selection
	if (speed_option == SPEED_SLOW)
	{
		multi_vram_buffer_horz("  SLOW ", 7, NTADR_A(11, 12));
	}
	else if (speed_option == SPEED_REGULAR)
	{
		multi_vram_buffer_horz("REGULAR", 7, NTADR_A(11, 12));
	}
	else
	{
		multi_vram_buffer_horz("  FAST ", 7, NTADR_A(11, 12));
	}

	multi_vram_buffer_horz("MAP:", 4, NTADR_A(8, 14));

	//options
	multi_vram_buffer_horz("HIVE", 4, NTADR_A(11, 16));

	multi_vram_buffer_horz("MUSIC:", 6, NTADR_A(8, 18));
	
	//options
	multi_vram_buffer_horz("FRENZY", 6, NTADR_A(11, 20));


	// Initialize start button variables for options
	start_hold_timer = 0;
	start_held = 0;

	ppu_on_all(); // turn on screen
}

void init_roundover(void){
	// increment the win count for the winning team
	game_mode = MODE_ROUNDOVER;
	music_stop();
	if (winner == ONETWO_WINNER)
	{
		team1_wins++;
	}
	else if (winner == THREEFOUR_WINNER)
	{
		team2_wins++;
	}
	update_hud();

	if (winner == ONETWO_WINNER)
	{
		multi_vram_buffer_horz("TEAM 1 WINS!", 12, NTADR_A(9, 12));
	}
	else if (winner == THREEFOUR_WINNER)
	{
		multi_vram_buffer_horz("TEAM 2 WINS!", 12, NTADR_A(9, 12));
	}


	if (win_reason == WIN_DOTS)
	{
		multi_vram_buffer_horz("COLLECTED 100 DOTS", 18, NTADR_A(6, 13));
	}
	else if (win_reason == WIN_FRIENDLY_FIRE)
	{
		multi_vram_buffer_horz("FRIENDLY BEE EATEN", 18, NTADR_A(6, 13));
	}
	else if (win_reason == WIN_ENEMY_KILL)
	{
		multi_vram_buffer_horz("ENEMY BEE EATEN", 15, NTADR_A(7, 13));
	}
	else if (win_reason == WIN_BIGBEE_EAT_DUCK)
	{
		multi_vram_buffer_horz("BIGBEE ATE DUCK!", 16, NTADR_A(7, 13));
	} else {
		multi_vram_buffer_horz("TIME UP!", 8, NTADR_A(11, 13));
	}
	
}

void init_gameover_loop(void)
{
	oam_clear();
	clear_background();
	music_stop();
	game_mode = MODE_GAMEOVER;
	ppu_off(); // screen off
	// load the title palettes
	clear_vram_buffer();
	pal_bg(palette_bg_combmap);
	pal_spr(palette_sp);

	
	// say who won
	if(team1_wins >= 3){
		multi_vram_buffer_horz("TEAM 1 WINS THE MATCH!", 21, NTADR_A(4, 12));
	} else if(team2_wins >= 3){
		multi_vram_buffer_horz("TEAM 2 WINS THE MATCH!", 21, NTADR_A(4, 12));
	} 
	// draw on screen, probably an unrle eventually for this game
	// probably lets the players select ai or player.
	multi_vram_buffer_horz("PRESS START", 11, NTADR_A(10, 24));

	ppu_on_all(); // turn on screen
	delay(10);
}

void init_system(void)
{
	ppu_off();
	clear_vram_buffer();
	set_vram_buffer();
	bank_spr(1);
	set_scroll_y(0xff); // shift the bg down one pixel

	// Initialize default speed option (regular speed)
	speed_option = SPEED_REGULAR;
	turbo_amount = TURBO_MEDIUM;
	
	// Initialize sprite rotation
	sprite_rotation = 0;
	
	// Initialize animation frames
	anim_frame_1 = 0;
	anim_frame_2 = 0;
	anim_frame_3 = 0;
	anim_frame_4 = 0;
	
	// Initialize bigbee transformation timers
	bee1_bigbee_timer = 0;
	bee3_bigbee_timer = 0;

	ppu_on_all(); // turn on screenxw
}

void clear_background(void)
{
	ppu_off(); // screen off
	// draw all 0x00 into the bg
	vram_adr(NAMETABLE_A);
	for (tempint = 0; tempint < 1024; ++tempint)
	{
		vram_put(0x00);
		flush_vram_update2();
	}
	ppu_on_all(); // turn on screen
}

void roundover_loop(void){
	while (1)
	{
		ppu_wait_nmi();
		pad1 = pad_poll(0); // read the first controller

		if (pad1 & PAD_START)
		{
			// Check if either team has reached 3 wins
			if (team1_wins >= 3)
			{
				// Team 1 wins the match
				// sfx_play(SFX_TEAM1_MATCH_WIN, 0);
				init_gameover_loop();
				break;
			}
			else if (team2_wins >= 3)
			{
				// Team 2 wins the match
				// sfx_play(SFX_TEAM2_MATCH_WIN, 0);
				init_gameover_loop();
				break;
			} 

			start_round();
			break;
		}
	}
}