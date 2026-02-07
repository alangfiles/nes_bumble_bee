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
	10.24 feedback:
	[x] draw last frame of the game
	[x] animate the winning team on the round over
	[x] put text under the scoreboard
	- keep stats and show mvp on match over screen
	- tie the speed to the clock
	- flash the screen on 70 honey?
	- draw 100 honey at end
	- change turbo to 3 boosts instead of time based?
	*/

	/*
	10.22 todo:
	[x] fix starting positions per map
	[] add new songs
	[x] handle settings menu
	[x] character hitboxes? (smaller quack)
	[] AI for less players
	[x] Quacks don't work outside of combs
	*/

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
		if (game_mode == MODE_ROUNDOVER)
		{
			roundover_loop();
		}
		//end of game loop
		if(HITBOX_DEBUG){
			gray_line();
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
		if (game_mode == MODE_TITLE)
		{
			vram_put(title[largeindex]);
		}
		else if (game_mode == MODE_OPTIONS)
		{
			vram_put(settings[largeindex]);
		}
		else
		{
			vram_put(map_ptr[largeindex]);
		}

		flush_vram_update2();
	}
	ppu_on_all();
}

void draw_sprites(void)
{
	// clear all sprites from sprite buffer
	oam_clear();

	// too many on a line potential, need to rotate through them.

	// draw powerups
	if (powerup1 == 1)
	{
		if (map == MAP_COMBS)
		{
			oam_meta_spr(COMBS_FLOWER_X[0], COMBS_FLOWER_Y[0], gamesprites_powerup_data);
		}
		else if (map == MAP_VINES)
		{
			oam_meta_spr(VINES_FLOWER_X[0], VINES_FLOWER_Y[0], gamesprites_powerup_data);
		}
		else if (map == MAP_OUTDOORS)
		{
			oam_meta_spr(OUTDOORS_FLOWER_X[0], OUTDOORS_FLOWER_Y[0], gamesprites_powerup_data);
		}
	}

	if (powerup2 == 1)
	{
		if (map == MAP_COMBS)
		{
			oam_meta_spr(COMBS_FLOWER_X[1], COMBS_FLOWER_Y[1], gamesprites_powerup_data);
		}
		else if (map == MAP_VINES)
		{
			oam_meta_spr(VINES_FLOWER_X[1], VINES_FLOWER_Y[1], gamesprites_powerup_data);
		}
		else if (map == MAP_OUTDOORS)
		{
			oam_meta_spr(OUTDOORS_FLOWER_X[1], OUTDOORS_FLOWER_Y[1], gamesprites_powerup_data);
		}
	}

	if (powerup3 == 1)
	{
		if (map == MAP_COMBS)
		{
			oam_meta_spr(COMBS_FLOWER_X[2], COMBS_FLOWER_Y[2], gamesprites_powerup_data);
		}
		else if (map == MAP_VINES)
		{
			oam_meta_spr(VINES_FLOWER_X[2], VINES_FLOWER_Y[2], gamesprites_powerup_data);
		}
		else if (map == MAP_OUTDOORS)
		{
			oam_meta_spr(OUTDOORS_FLOWER_X[2], OUTDOORS_FLOWER_Y[2], gamesprites_powerup_data);
		}
	}

	if (powerup4 == 1)
	{
		if (map == MAP_COMBS)
		{
			oam_meta_spr(COMBS_FLOWER_X[3], COMBS_FLOWER_Y[3], gamesprites_powerup_data);
		}
		else if (map == MAP_VINES)
		{
			oam_meta_spr(VINES_FLOWER_X[3], VINES_FLOWER_Y[3], gamesprites_powerup_data);
		}
		else if (map == MAP_OUTDOORS)
		{
			oam_meta_spr(OUTDOORS_FLOWER_X[3], OUTDOORS_FLOWER_Y[3], gamesprites_powerup_data);
		}
	}

	if (quack2.moving)
	{
		temp_x = quack2.x >> 8;
		temp_y = quack2.y >> 8;
		if (quack2.direction == DIR_LEFT)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_quackleft_data);
		}
		else if (quack2.direction == DIR_RIGHT)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_quackright_data);
		}
		else if (quack2.direction == DIR_DOWN)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_quackdown_data);
		}
		else if (quack2.direction == DIR_UP)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_quackup_data);
		}
	}

	if (quack4.moving)
	{
		temp_x = quack4.x >> 8;
		temp_y = quack4.y >> 8;
		if (quack4.direction == DIR_LEFT)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_quackleft_data);
		}
		else if (quack4.direction == DIR_RIGHT)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_quackright_data);
		}
		else if (quack4.direction == DIR_DOWN)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_quackdown_data);
		}
		else if (quack4.direction == DIR_UP)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_quackup_data);
		}
	}

	// Cycle through different drawing orders based on sprite_rotation
	// This prevents sprites from always being drawn in the same order
	switch (sprite_rotation & 0x03)
	{				// Use only bottom 2 bits for 4 different orders
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

	if (HITBOX_DEBUG)
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_box1_data);
		return;
	}

	// if the game is over and you didn't win, don't animate
	if (winner == THREEFOUR_WINNER)
	{
		if (BoxGuy1.direction == DIR_LEFT)
		{
			if (bee1_bigbee_timer > 0)
			{
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft0_data);
			}
			else
			{
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft0_data);
			}
		}
		else
		{ // if (BoxGuy1.direction == DIR_RIGHT)
			if (bee1_bigbee_timer > 0)
			{
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeright0_data);
			}
			else
			{
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright0_data);
			}
		}
		return;
	}

	if (stun_p1 > 0 && frame_counter % 2 == 0 && winner == UNDEFINED_WINNER)
	{
		return; // flash when stunned (only if game is still ongoing)
	}

	// Update animation frame every 10 frames
	if ((frame_counter % 10) == 0)
	{
		anim_frame_1 = (anim_frame_1 + 1) % 3;
	}

	// Check if player is in bigbee form
	if (bee1_bigbee_timer > 0)
	{

		if (bee1_bigbee_timer < BIGBEE_FLICKER_START)
		{
			// Flicker effect when bigbee is about to expire
			if ((frame_counter % 4) < 2 && winner == UNDEFINED_WINNER)
			{
				return; // Skip drawing this frame
			}
		}
		// Draw bigbee sprites
		if (BoxGuy1.direction == DIR_LEFT)
		{
			switch (anim_frame_1)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft2_data);
				break;
			}
		}
		else if (BoxGuy1.direction == DIR_RIGHT)
		{
			switch (anim_frame_1)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeright0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeright1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeright2_data);
				break;
			}
		}
		else
		{
			// Default to left-facing sprite for UP/DOWN/NONE
			switch (anim_frame_1)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbeeleft2_data);
				break;
			}
		}
	}
	else
	{

		// Draw normal small bee sprites
		if (BoxGuy1.direction == DIR_LEFT)
		{
			switch (anim_frame_1)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft2_data);
				break;
			}
		}
		else if (BoxGuy1.direction == DIR_RIGHT)
		{
			switch (anim_frame_1)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright2_data);
				break;
			}
		}
		else
		{
			// Default to left-facing sprite for UP/DOWN/NONE
			switch (anim_frame_1)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeleft2_data);
				break;
			}
		}
	}
}

void draw_player_2(void)
{
	temp_x = BoxGuy2.x >> 8;
	temp_y = BoxGuy2.y >> 8;

	if (HITBOX_DEBUG)
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_box2_data);
		return;
	}

	// if the other team one, just draw the first frame.
	if (winner == THREEFOUR_WINNER)
	{
		if (BoxGuy2.direction == DIR_LEFT)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft0_data);
		}
		else
		{ // if (BoxGuy2.direction == DIR_RIGHT)
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckright0_data);
		}
		return;
	}

	if (stun_p2 > 0 && frame_counter % 2 == 0 && winner == UNDEFINED_WINNER)
	{
		return; // flash when stunned
	}

	// Update animation frame every 10 frames, but only if player is moving
	if ((frame_counter % 10) == 0 && BoxGuy2.moving)
	{
		anim_frame_2 = (anim_frame_2 + 1) % 3;
	}

	// Choose sprite based on direction and animation frame
	// Note: Duck sprites have limited right-facing variants, so we'll use left-facing for most cases
	if (BoxGuy2.direction == DIR_LEFT)
	{
		switch (anim_frame_2)
		{

		case 0:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft0_data);
			break;
		case 1:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft1_data);
			break;
		case 2:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft2_data);
			break;
		}
	}
	else if (BoxGuy2.direction == DIR_RIGHT)
	{
		// Use left-facing sprites with horizontal flip for right direction
		switch (anim_frame_2)
		{
		case 0:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckright0_data);
			break;
		case 1:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckright1_data);
			break;
		case 2:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckright2_data);
			break;
		}
	}
	else
	{
		// Default to left-facing sprite for UP/DOWN/NONE
		switch (anim_frame_2)
		{
		case 0:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft0_data);
			break;
		case 1:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft1_data);
			break;
		case 2:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduckleft2_data);
			break;
		}
	}
}

void draw_player_3(void)
{

	if (HITBOX_DEBUG)
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_box3_data);
		return;
	}
	temp_x = BoxGuy3.x >> 8;
	temp_y = BoxGuy3.y >> 8;

	// if the game is over and you didn't win, don't animate
	if (winner == ONETWO_WINNER)
	{
		if (BoxGuy3.direction == DIR_LEFT)
		{
			if (bee3_bigbee_timer > 0)
			{
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left0_data);
			}
			else
			{
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left0_data);
			}
		}
		else
		{ // if (BoxGuy3.direction == DIR_RIGHT)
			if (bee3_bigbee_timer > 0)
			{
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2right0_data);
			}
			else
			{
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2right0_data);
			}
		}
		return;
	}

	if (stun_p3 > 0 && frame_counter % 2 == 0 && winner == UNDEFINED_WINNER)
	{
		return; // flash when stunned
	}

	// Update animation frame every 10 frames
	if ((frame_counter % 10) == 0)
	{
		anim_frame_3 = (anim_frame_3 + 1) % 3;
	}

	// Check if player is in bigbee form
	if (bee3_bigbee_timer > 0)
	{

		if (bee3_bigbee_timer < BIGBEE_FLICKER_START)
		{
			// Flicker effect when bigbee is about to expire
			if ((frame_counter % 4) < 2 && winner == UNDEFINED_WINNER)
			{
				return; // Skip drawing this frame
			}
		}

		// Draw bigbee sprites (using palette 1 for player 3)
		if (BoxGuy3.direction == DIR_LEFT)
		{
			switch (anim_frame_3)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left2_data);
				break;
			}
		}
		else if (BoxGuy3.direction == DIR_RIGHT)
		{
			switch (anim_frame_3)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2right0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2right1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2right2_data);
				break;
			}
		}
		else
		{
			// Default to left-facing sprite for UP/DOWN/NONE
			switch (anim_frame_3)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_bigbee2left2_data);
				break;
			}
		}
	}
	else
	{
		// Draw normal small bee sprites
		if (BoxGuy3.direction == DIR_LEFT)
		{
			switch (anim_frame_3)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left2_data);
				break;
			}
		}
		else if (BoxGuy3.direction == DIR_RIGHT)
		{
			switch (anim_frame_3)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2right0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2right1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2right2_data);
				break;
			}
		}
		else
		{
			// Default to left-facing sprite for UP/DOWN/NONE
			switch (anim_frame_3)
			{
			case 0:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left0_data);
				break;
			case 1:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left1_data);
				break;
			case 2:
				oam_meta_spr(temp_x, temp_y, gamesprites_smallbee2left2_data);
				break;
			}
		}
	}
}

void draw_player_4(void)
{
	temp_x = BoxGuy4.x >> 8;
	temp_y = BoxGuy4.y >> 8;

	if (HITBOX_DEBUG)
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_box4_data);
		return;
	}

	// if the other team one, just draw the first frame.
	if (winner == ONETWO_WINNER)
	{
		if (BoxGuy4.direction == DIR_LEFT)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left0_data);
		}
		else
		{ // if (BoxGuy4.direction == DIR_RIGHT)
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2right0_data);
		}
		return;
	}

	if (stun_p4 > 0 && frame_counter % 2 == 0 && winner == UNDEFINED_WINNER)
	{
		return; // flash when stunned
	}

	// Update animation frame every 10 frames, but only if player is moving
	if ((frame_counter % 10) == 0 && BoxGuy4.moving)
	{
		anim_frame_4 = (anim_frame_4 + 1) % 3;
	}

	// Choose sprite based on direction and animation frame
	// Note: Duck sprites have limited right-facing variants, so we'll use left-facing for most cases
	if (BoxGuy4.direction == DIR_LEFT)
	{
		switch (anim_frame_4)
		{
		case 0:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left0_data);
			break;
		case 1:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left1_data);
			break;
		case 2:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left2_data);
			break;
		}
	}
	else if (BoxGuy4.direction == DIR_RIGHT)
	{
		// Use left-facing sprites with horizontal flip for right direction
		switch (anim_frame_4)
		{
		case 0:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2right0_data);
			break;
		case 1:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2right1_data);
			break;
		case 2:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2right2_data);
			break;
		}
	}
	else
	{
		// Default to left-facing sprite for UP/DOWN/NONE
		switch (anim_frame_4)
		{
		case 0:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left0_data);
			break;
		case 1:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left1_data);
			break;
		case 2:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallduck2left2_data);
			break;
		}
	}
}
void draw_win_round_sprite(void)
{
	if (temp_win_reason == WIN_HONEY_COLLECTED)
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_win_icon_honey);
		return;
	}
	else if (temp_win_reason == WIN_MOST_HONEY_COLLECTED)
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_win_icon_time);
		return;
	}
	else if (temp_win_reason == WIN_BEE_EATEN)
	{
		if (temp_win_team == 1)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_win_redduck);
			return;
		}
		else
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_win_blueduck);
			return;
		}
	}
	else if (temp_win_reason == WIN_DUCK_EATEN)
	{
		if (temp_win_team == 1)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_win_redbee);
			return;
		}
		else
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_win_bluebee);
			return;
		}
	}
	else if (temp_win_reason == WIN_FRIENDLY_FIRE_BEE_EATEN)
	{
		if (temp_win_team == 1)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_win_blueduck);
			return;
		}
		else
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_win_redduck);
			return;
		}
	}
	else if (temp_win_reason == WIN_FRIENDLY_FIRE_DUCK_EATEN)
	{
		if (temp_win_team == 1)
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_win_bluebee);
			return;
		}
		else
		{
			oam_meta_spr(temp_x, temp_y, gamesprites_win_redbee);
			return;
		}
	}
}

void update_hud(void)
{
	// draw the icons for winning
	temp_x = 102;
	temp_y = 8;
	if (team1_win1 != 0xFF)
	{

		temp_win_team = 1;
		temp_win_reason = team1_win1;
		draw_win_round_sprite();
	}
	else
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_win_unwon);
	}
	temp_x = 92;
	temp_y = 8;
	if (team1_win2 != 0xFF)
	{

		temp_win_team = 1;
		temp_win_reason = team1_win2;
		draw_win_round_sprite();
	}
	else
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_win_unwon);
	}
	temp_x = 82;
	temp_y = 8;
	if (team1_win3 != 0xFF)
	{

		temp_win_team = 1;
		temp_win_reason = team1_win3;
		draw_win_round_sprite();
	}
	else
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_win_unwon);
	}
	temp_x = 146;
	temp_y = 8;
	if (team2_win1 != 0xFF)
	{

		temp_win_team = 2;
		temp_win_reason = team2_win1;
		draw_win_round_sprite();
	}
	else
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_win_unwon);
	}

	temp_x = 156;
	temp_y = 8;
	if (team2_win2 != 0xFF)
	{

		temp_win_team = 2;
		temp_win_reason = team2_win2;
		draw_win_round_sprite();
	}
	else
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_win_unwon);
	}

	temp_x = 166;
	temp_y = 8;
	if (team2_win3 != 0xFF)
	{

		temp_win_team = 2;
		temp_win_reason = team2_win3;
		draw_win_round_sprite();
	}
	else
	{
		oam_meta_spr(temp_x, temp_y, gamesprites_win_unwon);
	}
}

// Fast single-tile collision check for quack projectiles.
// Returns 1 if the tile at pixel (px,py) is solid (i.e. quack should stop),
// 0 otherwise. This is cheaper than calling the 4-direction bg_coll_* helpers.
char quack_tile_solid()
{
	if (temp_y2 >= 0xf0)
		return 0;
	largeindex = ((temp_y2 >> 3) << 5) + (temp_x2 >> 3);
	temp = map_ptr[largeindex];
	// don't collide with pellets or blank tiles
	if (temp == 0xc6 || temp == 0xc7 || temp == 0xc8 || temp == 0xc9 || temp == 0xca || temp == 0xa5)
	{
		return 0;
	}
	else if (temp == 0xb6 || temp == 0xb7 || temp == 0xb8 || temp == 0xb9 || temp == 0xba || temp == 0x00 || temp == 0x69 || temp == 0x6a || temp == 0x6b)
	{
		return 0;
	}
	return 1;
}

void quack_movement(void)
{
	// Move the quacks and disappear if collision with bg
	if (quack2.moving)
	{
		if (quack2.direction == DIR_LEFT)
		{
			quack2.x -= (speed_option + SPEED_QUACK);
		}
		else if (quack2.direction == DIR_RIGHT)
		{
			quack2.x += (speed_option + SPEED_QUACK);
		}
		else if (quack2.direction == DIR_UP)
		{
			quack2.y -= (speed_option + SPEED_QUACK);
		}
		else if (quack2.direction == DIR_DOWN)
		{
			quack2.y += (speed_option + SPEED_QUACK);
		}
		// single cheap tile check at quack center
		temp_x2 = quack2.x >> 8;
		temp_y2 = quack2.y >> 8;
		if (quack_tile_solid())
		{
			quack2.moving = 0;
		}
		// check collision with players
		// check collision with players
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;
		temp_x2 = quack2.x >> 8;
		temp_y2 = quack2.y >> 8;
		if (sprite_collision())
		{
			if (bee1_bigbee_timer == 0)
			{
				stun_p1 = STUN_DURATION;
			}
			quack2.moving = 0;
		}
		temp_x = BoxGuy2.x >> 8;
		temp_y = BoxGuy2.y >> 8;
		if (sprite_collision())
		{
			stun_p2 = STUN_DURATION;
			quack2.moving = 0;
		}
		temp_x = BoxGuy3.x >> 8;
		temp_y = BoxGuy3.y >> 8;
		if (sprite_collision())
		{
			if (bee3_bigbee_timer == 0)
			{
				stun_p3 = STUN_DURATION;
			}
			quack2.moving = 0;
		}
		temp_x = BoxGuy4.x >> 8;
		temp_y = BoxGuy4.y >> 8;
		if (sprite_collision())
		{
			stun_p4 = STUN_DURATION;
			quack2.moving = 0;
		}
	}

	if (quack4.moving)
	{
		if (quack4.direction == DIR_LEFT)
		{
			quack4.x -= (speed_option + SPEED_QUACK);
		}
		else if (quack4.direction == DIR_RIGHT)
		{
			quack4.x += (speed_option + SPEED_QUACK);
		}
		else if (quack4.direction == DIR_UP)
		{
			quack4.y -= (speed_option + SPEED_QUACK);
		}
		else if (quack4.direction == DIR_DOWN)
		{
			quack4.y += (speed_option + SPEED_QUACK);
		}
		// single cheap tile check at quack center
		temp_x2 = quack4.x >> 8;
		temp_y2 = quack4.y >> 8;
		if (quack_tile_solid())
		{
			quack4.moving = 0;
		}
		// check collision with players
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;
		temp_x2 = quack4.x >> 8;
		temp_y2 = quack4.y >> 8;
		if (sprite_collision())
		{
			if (bee1_bigbee_timer == 0)
			{
				stun_p1 = STUN_DURATION;
			}
			quack4.moving = 0;
		}
		temp_x = BoxGuy2.x >> 8;
		temp_y = BoxGuy2.y >> 8;
		if (sprite_collision())
		{
			stun_p2 = STUN_DURATION;
			quack4.moving = 0;
		}
		temp_x = BoxGuy3.x >> 8;
		temp_y = BoxGuy3.y >> 8;
		if (sprite_collision())
		{
			if (bee3_bigbee_timer == 0)
			{
				stun_p3 = STUN_DURATION;
			}
			quack4.moving = 0;
		}
		temp_x = BoxGuy4.x >> 8;
		temp_y = BoxGuy4.y >> 8;
		if (sprite_collision())
		{
			stun_p4 = STUN_DURATION;
			quack4.moving = 0;
		}
	}
}

void check_powerup(void)
{
	if (powerup1 == 1)
	{

		if (map == MAP_COMBS)
		{
			temp_x2 = COMBS_FLOWER_X[0];
			temp_y2 = COMBS_FLOWER_Y[0];
		}
		else if (map == MAP_VINES)
		{
			temp_x2 = VINES_FLOWER_X[0];
			temp_y2 = VINES_FLOWER_Y[0];
		}
		else if (map == MAP_OUTDOORS)
		{
			temp_x2 = OUTDOORS_FLOWER_X[0];
			temp_y2 = OUTDOORS_FLOWER_Y[0];
		}

		if (check_powerup_collision())
		{
			powerup1 = 0; // Remove powerup
			if (temp == 1)
			{
				bee1_bigbee_timer = POWERUP_TIMER;
			}
			else if (temp == 3)
			{
				bee3_bigbee_timer = POWERUP_TIMER;
			}
			sfx_play(SFX_POWERUP, 0);
		}
	}
	// Check powerup2 (top-right)
	if (powerup2 == 1)
	{
		if (map == MAP_COMBS)
		{
			temp_x2 = COMBS_FLOWER_X[1];
			temp_y2 = COMBS_FLOWER_Y[1];
		}
		else if (map == MAP_VINES)
		{
			temp_x2 = VINES_FLOWER_X[1];
			temp_y2 = VINES_FLOWER_Y[1];
		}
		else if (map == MAP_OUTDOORS)
		{
			temp_x2 = OUTDOORS_FLOWER_X[1];
			temp_y2 = OUTDOORS_FLOWER_Y[1];
		}

		if (check_powerup_collision())
		{
			powerup2 = 0; // Remove powerup
			if (temp == 1)
			{
				bee1_bigbee_timer = POWERUP_TIMER;
			}
			else if (temp == 3)
			{
				bee3_bigbee_timer = POWERUP_TIMER;
			}
			sfx_play(SFX_POWERUP, 0);
		}
	}
	// Check powerup3 (bottom-left)
	if (powerup3 == 1)
	{
		if (map == MAP_COMBS)
		{
			temp_x2 = COMBS_FLOWER_X[2];
			temp_y2 = COMBS_FLOWER_Y[2];
		}
		else if (map == MAP_VINES)
		{
			temp_x2 = VINES_FLOWER_X[2];
			temp_y2 = VINES_FLOWER_Y[2];
		}
		else if (map == MAP_OUTDOORS)
		{
			temp_x2 = OUTDOORS_FLOWER_X[2];
			temp_y2 = OUTDOORS_FLOWER_Y[2];
		}

		if (check_powerup_collision())
		{
			powerup3 = 0; // Remove powerup
			if (temp == 1)
			{
				bee1_bigbee_timer = POWERUP_TIMER;
			}
			else if (temp == 3)
			{
				bee3_bigbee_timer = POWERUP_TIMER;
			}
			sfx_play(SFX_POWERUP, 0);
		}
	}

	// Check powerup4 (bottom-right)
	if (powerup4 == 1)
	{
		if (map == MAP_COMBS)
		{
			temp_x2 = COMBS_FLOWER_X[3];
			temp_y2 = COMBS_FLOWER_Y[3];
		}
		else if (map == MAP_VINES)
		{
			temp_x2 = VINES_FLOWER_X[3];
			temp_y2 = VINES_FLOWER_Y[3];
		}
		else if (map == MAP_OUTDOORS)
		{
			temp_x2 = OUTDOORS_FLOWER_X[3];
			temp_y2 = OUTDOORS_FLOWER_Y[3];
		}

		if (check_powerup_collision())
		{
			powerup4 = 0; // Remove powerup
			if (temp == 1)
			{
				bee1_bigbee_timer = POWERUP_TIMER;
			}
			else if (temp == 3)
			{
				bee3_bigbee_timer = POWERUP_TIMER;
			}
			sfx_play(SFX_POWERUP, 0);
		}
	}
}

void movement(void)
{
	// reset collision state for this movement step
	collision = 0;
	GenericBoxGuy.collision = 0;

	// stunned players can't move
	if ((current_player == 1 && stun_p1 > 0) ||
			(current_player == 2 && stun_p2 > 0) ||
			(current_player == 3 && stun_p3 > 0) ||
			(current_player == 4 && stun_p4 > 0))
	{
		// Player is stunned, skip movement processing
		switch (current_player)
		{
		case 1:
			BoxGuy1 = GenericBoxGuy;
			break;
		case 2:
			BoxGuy2 = GenericBoxGuy;
			break;
		case 3:
			BoxGuy3 = GenericBoxGuy;
			break;
		case 4:
			BoxGuy4 = GenericBoxGuy;
			break;
		}
		return;
	}

	// handle x
	old_x = GenericBoxGuy.x;

	// QUACK BUTTON
	if (generic_pad & PAD_B)
	{
		if (current_player == 2 && quack2.moving == 0 && quack2_cooldown == 0)
		{
			quack2.x = GenericBoxGuy.x;
			quack2.y = GenericBoxGuy.y;
			quack2.direction = GenericBoxGuy.direction;
			quack2_cooldown = QUACK_COOLDOWN;
			if (quack2.direction == DIR_LEFT)
			{
				quack2.x -= 0x0800; // move it 1 block to the left;
			}
			else if (quack2.direction == DIR_RIGHT)
			{
				quack2.x += 0x0800;
			}
			else if (quack2.direction == DIR_UP)
			{
				quack2.y -= 0x0800;
			}
			else if (quack2.direction == DIR_DOWN)
			{
				quack2.y += 0x0800;
			}
			quack2.moving = 1;
		}
		if (current_player == 4 && quack4.moving == 0 && quack4_cooldown == 0)
		{
			quack4.x = GenericBoxGuy.x;
			quack4.y = GenericBoxGuy.y;
			quack4.direction = GenericBoxGuy.direction;
			quack4.moving = 1;
			quack4_cooldown = QUACK_COOLDOWN;
			if (quack4.direction == DIR_LEFT)
			{
				quack4.x -= 0x0800; // move it 1 block to the left;
			}
			else if (quack4.direction == DIR_RIGHT)
			{
				quack4.x += 0x0800;
			}
			else if (quack4.direction == DIR_UP)
			{
				quack4.y -= 0x0800;
			}
			else if (quack4.direction == DIR_DOWN)
			{
				quack4.y += 0x0800;
			}
		}
	}

	// TURBO BUTTON
	if (generic_pad & PAD_A)
	{
		// Check which player and if they have turbo available
		if ((current_player == 1 && turbo_p1 > 0 && bee1_bigbee_timer == 0) ||
				(current_player == 2 && turbo_p2 > 0) ||
				(current_player == 3 && turbo_p3 > 0 && bee3_bigbee_timer == 0) ||
				(current_player == 4 && turbo_p4 > 0))
		{

			use_turbo = 1;
			switch (current_player)
			{
			case 1:
				turbo_p1--;
				break;
			case 2:
				turbo_p2--;
				break;
			case 3:
				turbo_p3--;
				break;
			case 4:
				turbo_p4--;
				break;
			}
		}
	}

	if (ducks_go_faster_over_time)
	{
		if (current_player == 2 || current_player == 4)
		{
			current_speed = speed_option;
			if (game_timer > 80)
			{
				current_speed -= DUCK_SPEED_LOW;
			}
			else if (game_timer > 20)
			{
				current_speed = speed_option; // no nothing here
			}
			else if (game_timer > 10)
			{
				current_speed += DUCK_SPEED_HIGH;
			}
			else if (game_timer > 0)
			{
				current_speed += DUCK_SPEED_MAX;
			}
		}
		else
		{
			current_speed = speed_option; // bees stay the same
		}
	}
	else
	{
		current_speed = speed_option; // feature disabled, all normal speed
	}

	if (bee1_bigbee_timer > 0 && current_player == 1)
	{
		current_speed += (current_speed >> 2); // big bee boost
	}

	if (bee3_bigbee_timer > 0 && current_player == 3)
	{
		current_speed += (current_speed >> 2); // big bee boost
	}

	if (generic_pad & PAD_LEFT)
	{
		hero_velocity_x = -current_speed;
		if (use_turbo == 1)
		{
			hero_velocity_x -= SPEED_TURBO_BOOST; // add turbo boost
			use_turbo = 0;												// reset turbo flag
		}
		GenericBoxGuy.direction = DIR_LEFT;
	}
	else if (generic_pad & PAD_RIGHT)
	{
		hero_velocity_x = current_speed;
		if (use_turbo == 1)
		{
			hero_velocity_x += SPEED_TURBO_BOOST; // add turbo boost
			use_turbo = 0;												// reset turbo flag
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
			collision = 1;
			GenericBoxGuy.collision = 1;
		}
	}
	else if (hero_velocity_x > 0)
	{ // going right
		if (bg_coll_R())
		{													 // check collision right
			GenericBoxGuy.x = old_x; // revert to old position
			collision = 1;
			GenericBoxGuy.collision = 1;
		}
	}
	// else 0, skip it

	// handle y
	old_y = GenericBoxGuy.y;

	if (generic_pad & PAD_UP)
	{
		hero_velocity_y = -current_speed;
		if (use_turbo == 1)
		{
			hero_velocity_y -= SPEED_TURBO_BOOST; // add turbo boost
			use_turbo = 0;												// reset turbo flag
		}
		GenericBoxGuy.direction = DIR_UP; // entually we'll set up and down but not now.
	}
	else if (generic_pad & PAD_DOWN)
	{
		hero_velocity_y = current_speed;
		if (use_turbo == 1)
		{
			hero_velocity_y += SPEED_TURBO_BOOST; // add turbo boost
			use_turbo = 0;												// reset turbo flag
		}
		GenericBoxGuy.direction = DIR_DOWN;
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
			collision = 1;
			GenericBoxGuy.collision = 1;
		}
	}
	else if (hero_velocity_y > 0)
	{ // going down
		if (bg_coll_D())
		{													 // check collision down
			GenericBoxGuy.y = old_y; // revert to old position
			collision = 1;
			GenericBoxGuy.collision = 1;
		}
	}
	// else 0, skip it

	// player has moved, now collect any pellets
	check_tile_and_collect();

	// move updated generic to the specific player
	switch (current_player)
	{
	case 1:
		BoxGuy1 = GenericBoxGuy;
		break;
	case 2:
		BoxGuy2 = GenericBoxGuy;
		break;
	case 3:
		BoxGuy3 = GenericBoxGuy;
		break;
	case 4:
		BoxGuy4 = GenericBoxGuy;
		break;
	}
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

const unsigned char pellet_tiles[6] = {
		0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xa5};

const unsigned char blank_tiles[9] = {
		0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0x00, 0x69, 0x6a, 0x6b};

char bg_collision_sub(void)
{
	if (temp_y >= 0xf0)
		return 0;
	// temp_x and temp_y are the coordinates to check the collision

	temp = map_ptr[((temp_y >> 3) << 5) + (temp_x >> 3)];
	// don't collide with pellets, but everything else
	if (temp == 0xc6 || temp == 0xc7 || temp == 0xc8 || temp == 0xc9 || temp == 0xca || temp == 0xa5)
	{
		return 0;
	}
	else if (temp == 0xb6 || temp == 0xb7 || temp == 0xb8 || temp == 0xb9 || temp == 0xba || temp == 0x00 || temp == 0x69 || temp == 0x6a || temp == 0x6b)
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
	temp = map_ptr[largeindex];

	for (index = 0; index < sizeof(pellet_tiles); index++)
	{
		if (temp == pellet_tiles[index])
		{
			// Check if this dot has already been consumed
			if (!is_dot_consumed(largeindex))
			{
				// Mark this dot as consumed
				mark_dot_consumed(largeindex);
				// Update the screen
				// one_vram_buffer(blank_tiles[frame_counter % 5], NTADR_A(temp_x, temp_y));
				one_vram_buffer(0x00, NTADR_A(temp_x, temp_y)); // TODO: this is just a blank tile, not as fun as the updated backgrounds

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
						win_reason = WIN_HONEY_COLLECTED;
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
						win_reason = WIN_HONEY_COLLECTED;
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
	if (temp_x < temp_x2 + collision_box_size && temp_x + collision_box_size > temp_x2 &&
			temp_y < temp_y2 + collision_box_size && temp_y + collision_box_size > temp_y2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// Check if a player collides with a powerup
char check_powerup_collision()
{
	// Powerup is 16x16 pixels, player is 8x8 pixels
	// Check if player's center is within powerup bounds
	if (temp_x + 4 >= temp_x2 && temp_x + 4 < temp_x2 + 16 &&
			temp_y + 4 >= temp_y2 && temp_y + 4 < temp_y2 + 16)
	{
		return 1;
	}
	return 0;
}

void draw_hud(void)
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
	one_vram_buffer(temp1, NTADR_A(24, 1));
	temp1 = (team2_score % 10) + 0x30;
	one_vram_buffer(temp1, NTADR_A(25, 1));
}

static void ai_dir_to_pad(void)
{
	if (ai_dir_work == DIR_UP)
	{
		ai_pad_work = PAD_UP;
	}
	else if (ai_dir_work == DIR_DOWN)
	{
		ai_pad_work = PAD_DOWN;
	}
	else if (ai_dir_work == DIR_LEFT)
	{
		ai_pad_work = PAD_LEFT;
	}
	else
	{
		ai_pad_work = PAD_RIGHT;
	}
}

static void ai_update(void)
{
	if (ai_collision_work)
	{
		ai_timer_work = 0;
		// pick a new direction on collision
		temp = ai_dir_work;
		do
		{
			ai_dir_work = rand8() & 0x03;
		} while (ai_dir_work == temp);
		ai_collision_work = 0;
	}
}

void player1_ai(void)
{
	ai_timer_work = ai_timer_p1;
	ai_dir_work = ai_dir_p1;
	ai_collision_work = BoxGuy1.collision;
	ai_update();
	ai_timer_p1 = ai_timer_work;
	ai_dir_p1 = ai_dir_work;
	ai_dir_to_pad();
	pad1 = ai_pad_work;
}

void player2_ai(void)
{
	ai_timer_work = ai_timer_p2;
	ai_dir_work = ai_dir_p2;
	ai_collision_work = BoxGuy2.collision;
	ai_update();
	ai_timer_p2 = ai_timer_work;
	ai_dir_p2 = ai_dir_work;
	ai_dir_to_pad();
	pad2 = ai_pad_work;
}

void player3_ai(void)
{
	ai_timer_work = ai_timer_p3;
	ai_dir_work = ai_dir_p3;
	ai_collision_work = BoxGuy3.collision;
	ai_update();
	ai_timer_p3 = ai_timer_work;
	ai_dir_p3 = ai_dir_work;
	ai_dir_to_pad();
	pad3 = ai_pad_work;
}

void player4_ai(void)
{
	ai_timer_work = ai_timer_p4;
	ai_dir_work = ai_dir_p4;
	ai_collision_work = BoxGuy4.collision;
	ai_update();
	ai_timer_p4 = ai_timer_work;
	ai_dir_p4 = ai_dir_work;
	ai_dir_to_pad();
	pad4 = ai_pad_work;
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

	if (game_mode == MODE_GAME)
	{
		if (use_ai_player_1)
		{
			player1_ai();
		}
		if (use_ai_player_2)
		{
			player2_ai();
		}
		if (use_ai_player_3)
		{
			player3_ai();
		}
		if (use_ai_player_4)
		{
			player4_ai();
		}
	}

	// set the new inputs (things that have changed since last frame)
	pad1_new = pad1 & (pad1 ^ prev_pad1);
	pad2_new = pad2 & (pad2 ^ prev_pad2);
	pad3_new = pad3 & (pad3 ^ prev_pad3);
	pad4_new = pad4 & (pad4 ^ prev_pad4);

	// save the previous state for next frame
	prev_pad1 = pad1;
	prev_pad2 = pad2;
	prev_pad3 = pad3;
	prev_pad4 = pad4;

	// seeker_ai();
	// chaser_ai();

	// debug just random values
	//  [0b00010000,0b00100000,0b01000000,0b10000000]
}

void game_loop(void)
{
	// wait till beginning of the frame
	ppu_wait_nmi();

	// 0. DEBUGGING CODE
	// debug_extras();
	// this should just move to the chr stuff

	// 1. INCREMENT GLOBAL COUNTERS
	game_counters();

	// 2.  READ CONTROLLER
	read_controllers();

	// 3. PLAYER MOVEMENT

	// player 1 is seeker, 2 is chaser
	// player 3 is seeker, 4 is chaser
	// Deal with movement for each player

	// move the enemies aka the quacks
	quack_movement();

	// setup generics for player 1 (seeker)
	current_player = 1;
	GenericBoxGuy = BoxGuy1;
	generic_pad = pad1;
	// call movement for generics
	movement(); // this assigns old_x and old_y
	temp_x = GenericBoxGuy.x >> 8;
	temp_y = GenericBoxGuy.y >> 8;
	temp_x2 = BoxGuy3.x >> 8;
	temp_y2 = BoxGuy3.y >> 8;
	if (sprite_collision()) // 1 and 3, both seekers
	{
		// players bounce off each other
		BoxGuy1.x = old_x;
		BoxGuy1.y = old_y;
		BoxGuy1.collision = 1;
		BoxGuy3.collision = 1;
	}

	// setup generics for player 2 (chaser)
	current_player = 2;
	GenericBoxGuy = BoxGuy2;
	generic_pad = pad2;
	movement();

	// For ducks, set direction to NONE if no input is pressed
	if (!(pad2 & (PAD_LEFT | PAD_RIGHT | PAD_UP | PAD_DOWN)))
	{
		BoxGuy2.moving = 0;
	}
	else
	{
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
		BoxGuy2.collision = 1;
		BoxGuy4.collision = 1;
	}

	// setup generics for player 3 (seeker)
	current_player = 3;
	GenericBoxGuy = BoxGuy3;
	generic_pad = pad3;
	movement();
	// player 1 blocks player 3
	temp_x = BoxGuy1.x >> 8;
	temp_y = BoxGuy1.y >> 8;
	temp_x2 = GenericBoxGuy.x >> 8;
	temp_y2 = GenericBoxGuy.y >> 8;
	if (sprite_collision()) // 1 blocks 3 (seekers)
	{
		// players bounce off each other
		BoxGuy3.x = old_x;
		BoxGuy3.y = old_y;
		BoxGuy3.collision = 1;
		BoxGuy1.collision = 1;
	}

	// setup generics for player 4 (chaser)
	current_player = 4;
	GenericBoxGuy = BoxGuy4;
	generic_pad = pad4;
	movement();

	// For ducks, set direction to NONE if no input is pressed
	if (!(pad4 & (PAD_LEFT | PAD_RIGHT | PAD_UP | PAD_DOWN)))
	{
		BoxGuy4.moving = 0;
	}
	else
	{
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
		BoxGuy4.collision = 1;
		BoxGuy2.collision = 1;
	}

	// 4. CHECK POWERUP COLLISIONS

	// Check if bee players (1 and 3) collide with powerups
	if (bee1_bigbee_timer == 0)
	{ // Only check if not already transformed
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;
		temp = 1; // which bee are we checking

		check_powerup();
	}

	if (bee3_bigbee_timer == 0)
	{ // Only check if not already transformed
		temp_x = BoxGuy3.x >> 8;
		temp_y = BoxGuy3.y >> 8;
		temp = 3;

		check_powerup();
	}

	// 5. CHECK COLLISON

	// 5. CHECK BIGBEE EATING DUCKS

	// Check if bigbee player 1 eats any duck
	if (bee1_bigbee_timer > 0)
	{
		temp_x = BoxGuy1.x >> 8;
		temp_y = BoxGuy1.y >> 8;

		// Check if bigbee player 1 eats friendly duck (player 2) - LOSE
		temp_x2 = BoxGuy2.x >> 8;
		temp_y2 = BoxGuy2.y >> 8;
		if (sprite_collision())
		{
			sfx_play(SFX_TEAM2_WIN, 0);
			winner = THREEFOUR_WINNER;
			win_reason = WIN_FRIENDLY_FIRE_DUCK_EATEN;
			init_roundover();
			return;
		}

		// Check if bigbee player 1 eats enemy duck (player 4) - WIN
		temp_x2 = BoxGuy4.x >> 8;
		temp_y2 = BoxGuy4.y >> 8;
		if (sprite_collision())
		{
			sfx_play(SFX_TEAM1_WIN, 0);
			winner = ONETWO_WINNER;
			win_reason = WIN_DUCK_EATEN;
			init_roundover();
			return;
		}
	}

	// Check if bigbee player 3 eats any duck
	if (bee3_bigbee_timer > 0)
	{
		temp_x = BoxGuy3.x >> 8;
		temp_y = BoxGuy3.y >> 8;

		// Check if bigbee player 3 eats friendly duck (player 4) - LOSE
		temp_x2 = BoxGuy4.x >> 8;
		temp_y2 = BoxGuy4.y >> 8;
		if (sprite_collision())
		{
			sfx_play(SFX_TEAM1_WIN, 0);
			winner = ONETWO_WINNER;
			win_reason = WIN_FRIENDLY_FIRE_DUCK_EATEN;
			init_roundover();
			return;
		}

		// Check if bigbee player 3 eats enemy duck (player 2) - WIN
		temp_x2 = BoxGuy2.x >> 8;
		temp_y2 = BoxGuy2.y >> 8;
		if (sprite_collision())
		{
			sfx_play(SFX_TEAM2_WIN, 0);
			winner = THREEFOUR_WINNER;
			win_reason = WIN_DUCK_EATEN;
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
		win_reason = WIN_FRIENDLY_FIRE_BEE_EATEN;
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
		win_reason = WIN_FRIENDLY_FIRE_BEE_EATEN;
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
		win_reason = WIN_BEE_EATEN;
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
		win_reason = WIN_BEE_EATEN;
		init_roundover();
		return;
	}

	// 6. DRAW SPRITES
	draw_sprites();
	update_hud();
	draw_hud();
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
		if (pad1_new & PAD_UP)
		{
			if ((BoxGuy1.y >> 8) > 160)
			{											 // Limit how high they can go
				BoxGuy1.y -= 0x0400; // Move up 4 pixels (0x0400 = 1024 sub-pixels)
			}
		}
		if (pad2_new & PAD_UP)
		{
			if ((BoxGuy2.y >> 8) > 160)
			{
				BoxGuy2.y -= 0x0400;
			}
		}
		if (pad3_new & PAD_UP)
		{
			if ((BoxGuy3.y >> 8) > 160)
			{
				BoxGuy3.y -= 0x0400;
			}
		}
		if (pad4_new & PAD_UP)
		{
			if ((BoxGuy4.y >> 8) > 160)
			{
				BoxGuy4.y -= 0x0400;
			}
		}

		// Handle start button hold logic
		if (pad1_new & PAD_START || pad2_new & PAD_START || pad3_new & PAD_START || pad4_new & PAD_START)
		{

			init_options_loop();
			break;
		}
	}
}

void options_loop(void)
{
	while (1)
	{
		ppu_wait_nmi();
		frame_counter++;

		// draw bee:
		oam_clear();
		if ((frame_counter % 10) == 0)
		{
			anim_frame_1 = (anim_frame_1 + 1) % 3;
			if (!use_ai_player_2)
			{
				anim_frame_2 = (anim_frame_2 + 1) % 3;
			}
			if (!use_ai_player_3)
			{
				anim_frame_3 = (anim_frame_3 + 1) % 3;
			}
			if (!use_ai_player_4)
			{
				anim_frame_4 = (anim_frame_4 + 1) % 3;
			}
		}
		switch (anim_frame_1)
		{
		case 0:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright0_data);
			break;
		case 1:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright1_data);
			break;
		case 2:
			oam_meta_spr(temp_x, temp_y, gamesprites_smallbeeright2_data);
			break;
		}

		// draw joinable player sprites at bottom

		// Player 1 (bee)
		if (use_ai_player_1)
		{
			oam_meta_spr(64, 200, gamesprites_smallbeeright0_data);
		}
		else
		{
			switch (anim_frame_1)
			{
			case 0:
				oam_meta_spr(64, 200, gamesprites_smallbeeright0_data);
				break;
			case 1:
				oam_meta_spr(64, 200, gamesprites_smallbeeright1_data);
				break;
			case 2:
				oam_meta_spr(64, 200, gamesprites_smallbeeright2_data);
				break;
			}
		}

		// Player 2 (duck)
		if (use_ai_player_2)
		{
			oam_meta_spr(104, 200, gamesprites_smallduckright0_data);
		}
		else
		{
			switch (anim_frame_2)
			{
			case 0:
				oam_meta_spr(104, 200, gamesprites_smallduckright0_data);
				break;
			case 1:
				oam_meta_spr(104, 200, gamesprites_smallduckright1_data);
				break;
			case 2:
				oam_meta_spr(104, 200, gamesprites_smallduckright2_data);
				break;
			}
		}

		// Player 3 (bee 2)
		if (use_ai_player_3)
		{
			oam_meta_spr(144, 200, gamesprites_smallbee2right0_data);
		}
		else
		{
			switch (anim_frame_3)
			{
			case 0:
				oam_meta_spr(144, 200, gamesprites_smallbee2right0_data);
				break;
			case 1:
				oam_meta_spr(144, 200, gamesprites_smallbee2right1_data);
				break;
			case 2:
				oam_meta_spr(144, 200, gamesprites_smallbee2right2_data);
				break;
			}
		}

		// Player 4 (duck 2)
		if (use_ai_player_4)
		{
			oam_meta_spr(184, 200, gamesprites_smallduck2right0_data);
		}
		else
		{
			switch (anim_frame_4)
			{
			case 0:
				oam_meta_spr(184, 200, gamesprites_smallduck2right0_data);
				break;
			case 1:
				oam_meta_spr(184, 200, gamesprites_smallduck2right1_data);
				break;
			case 2:
				oam_meta_spr(184, 200, gamesprites_smallduck2right2_data);
				break;
			}
		}

		// Read all controllers for options screen
		read_controllers();

		// Press select to join/unjoin (toggle AI)
		if (pad1_new & PAD_SELECT)
		{
			use_ai_player_1 ^= 1;
		}
		if (pad2_new & PAD_SELECT)
		{
			use_ai_player_2 ^= 1;
			if (use_ai_player_2)
			{
				anim_frame_2 = 0;
			}
		}
		if (pad3_new & PAD_SELECT)
		{
			use_ai_player_3 ^= 1;
			if (use_ai_player_3)
			{
				anim_frame_3 = 0;
			}
		}
		if (pad4_new & PAD_SELECT)
		{
			use_ai_player_4 ^= 1;
			if (use_ai_player_4)
			{
				anim_frame_4 = 0;
			}
		}

		if (pad1_new & PAD_DOWN)
		{
			if (current_settings_choice < SETTING_SONG)
			{
				current_settings_choice++;
				update_options_screen();
			}
		}

		if (pad1_new & PAD_UP)
		{
			if (current_settings_choice > 0)
			{
				current_settings_choice--;
				update_options_screen();
			}
		}

		if (pad1_new & PAD_RIGHT)
		{
			if (current_settings_choice == SETTING_MAP)
			{
				if (settings_map < MAP_RANDOM)
				{
					settings_map++;
				}
			}
			if (current_settings_choice == SETTING_SPEED)
			{
				if (settings_speed < GAME_FAST)
				{
					settings_speed++;
				}
			}
			if (current_settings_choice == SETTING_SONG)
			{
				if (settings_song < SONG_HONEY)
				{
					settings_song++;
				}
			}
			update_options_screen();
		}

		if (pad1_new & PAD_LEFT)
		{
			if (current_settings_choice == SETTING_MAP)
			{
				if (settings_map > 0)
				{
					settings_map--;
				}
			}
			if (current_settings_choice == SETTING_SPEED)
			{
				if (settings_speed > 0)
				{
					settings_speed--;
				}
			}
			if (current_settings_choice == SETTING_SONG)
			{
				if (settings_song > 0)
				{
					settings_song--;
				}
			}
			update_options_screen();
		}

		// Handle start button hold logic
		if (pad1_new & PAD_START || pad2_new & PAD_START || pad3_new & PAD_START || pad4_new & PAD_START)
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
		frame_counter++;
		oam_clear();
		if (winner == ONETWO_WINNER)
		{
			draw_player_1();
			draw_player_2();
		}
		else if (winner == THREEFOUR_WINNER)
		{
			draw_player_3();
			draw_player_4();
		}

		ppu_wait_nmi();
		read_controllers();

		if (pad1_new & PAD_START || pad2_new & PAD_START || pad3_new & PAD_START || pad4_new & PAD_START)
		{
			init_title_loop();
			break;
		}

	}
	
}

void start_round(void)
{

	if (settings_map == MAP_RANDOM)
	{
		if (frame_counter % 3 == 0)
		{
			map_ptr = combs;
			map = MAP_COMBS;
		}
		else if (frame_counter % 3 == 1)
		{
			map_ptr = vines;
			map = MAP_VINES;
		}
		else
		{
			map_ptr = outdoors;
			map = MAP_OUTDOORS;
		}
		load_bg_palette();
	}

	load_room();

	// clear score counts

	for (index = 0; index < 128; index++)
	{
		consumed_dots[index] = 0;
	}

	if (map == MAP_COMBS)
	{
		BoxGuy1.x = 0x3800;
		BoxGuy1.y = 0xA800;
		BoxGuy2.x = 0x5000;
		BoxGuy2.y = 0x2800;
		BoxGuy3.x = 0xC000;
		BoxGuy3.y = 0xA800;
		BoxGuy4.x = 0xA000;
		BoxGuy4.y = 0x2800;
	}
	else if (map == MAP_VINES)
	{
		BoxGuy1.x = 0x6000;
		BoxGuy1.y = 0xA000;
		BoxGuy2.x = 0x4800;
		BoxGuy2.y = 0x4800;
		BoxGuy3.x = 0x9800;
		BoxGuy3.y = 0xA000;
		BoxGuy4.x = 0xB000;
		BoxGuy4.y = 0x4800;
	}
	else if (map == MAP_OUTDOORS)
	{
		BoxGuy1.x = 0x2800;
		BoxGuy1.y = 0x2000;
		BoxGuy2.x = 0x2800;
		BoxGuy2.y = 0xd800;
		BoxGuy3.x = 0xD800;
		BoxGuy3.y = 0x2000;
		BoxGuy4.x = 0xD800;
		BoxGuy4.y = 0xD800;
	}

	// todo: not removing this yet as these might be useful
	//   else if (map_positions == START_POS_MIDDLE) {

	// 		BoxGuy3.direction = DIR_RIGHT;
	// 		BoxGuy4.direction = DIR_RIGHT;
	// } else {  // START_POS_DEFAULT
	//     BoxGuy1.x = 0x4800; BoxGuy1.y = 0x4000;
	//     BoxGuy2.x = 0x7000; BoxGuy2.y = 0x2800;
	//     BoxGuy3.x = 0xB000; BoxGuy3.y = 0x4000;
	//     BoxGuy4.x = 0x8800; BoxGuy4.y = 0x2800;
	// 		BoxGuy4.direction = DIR_RIGHT;
	// 		BoxGuy1.direction = DIR_RIGHT;
	// }
	team1_score = 0;
	team2_score = 0;
	powerup1 = 1;
	powerup2 = 1;
	powerup3 = 1;
	powerup4 = 1;
	game_timer = GAME_LENGTH;
	win_reason = WIN_HONEY_COLLECTED; // default
	winner = UNDEFINED_WINNER;

	// Reset turbo counters for all players
	turbo_p1 = turbo_amount;
	turbo_p2 = turbo_amount;
	turbo_p3 = turbo_amount;
	turbo_p4 = turbo_amount;

	// reset stun
	stun_p1 = 0;
	stun_p2 = 0;
	stun_p3 = 0;

	// Reset quacks
	quack2.moving = 0;
	quack4.moving = 0;
	quack2_cooldown = 0;
	quack4_cooldown = 0;

	// Reset bigbee transformation timers
	bee1_bigbee_timer = 0;
	bee3_bigbee_timer = 0;

	ppu_wait_nmi();
	oam_clear();

	draw_hud();
	update_hud();

	draw_player_1();
	draw_player_2();
	draw_player_3();
	draw_player_4();
	oam_meta_spr(116, 100, gamesprites_big3_data);
	sfx_play(SFX_TEAM1_DOT_COLLECT, 0);
	ppu_wait_nmi();
	delay(40);
	oam_clear();
	draw_player_1();
	draw_player_2();
	draw_player_3();
	draw_player_4();
	ppu_wait_nmi();
	oam_meta_spr(116, 100, gamesprites_big2_data);
	sfx_play(SFX_TEAM1_DOT_COLLECT, 0);
	delay(40);
	oam_clear();
	draw_player_1();
	draw_player_2();
	draw_player_3();
	draw_player_4();
	ppu_wait_nmi();
	oam_meta_spr(116, 100, gamesprites_big1_data);
	sfx_play(SFX_TEAM1_DOT_COLLECT, 0);
	delay(40);
	oam_clear();
	draw_player_1();
	draw_player_2();
	draw_player_3();
	draw_player_4();
	ppu_wait_nmi();
	sfx_play(SFX_START, 0);
	delay(10);
	ppu_wait_nmi();

	game_mode = MODE_GAME;

	if (settings_song == SONG_BEE)
		song = SONG_BEE;
	else if (settings_song == SONG_LAKE)
		song = SONG_LAKE;
	else if (settings_song == SONG_HONEY)
		song = SONG_HONEY;
	else if (settings_song == SONG_RANDOM)
	{
		if (frame_counter % 3 == 0)
		{
			song = SONG_BEE;
		}
		else if (frame_counter % 3 == 1)
		{
			song = SONG_LAKE;
		}
		else
		{
			song = SONG_HONEY;
		}
	}

	song = SONG_BEE; // TODO: add more songs then fix this
	music_play(song);
}

void init_game_loop(void)
{
	clear_background();

	game_mode = MODE_GAME;
	// Initialize scores
	// reset variables scores and rounds
	team1_wins = 0;
	team2_wins = 0;
	current_round = 1;
	round_1 = 0;
	round_2 = 0;
	round_3 = 0;
	round_4 = 0;
	round_5 = 0;
	round_1_winner = 0;
	round_2_winner = 0;
	round_3_winner = 0;
	round_4_winner = 0;
	round_5_winner = 0;
	team1_win1 = 0xFF;
	team1_win2 = 0xFF;
	team1_win3 = 0xFF;
	team2_win1 = 0xFF;
	team2_win2 = 0xFF;
	team2_win3 = 0xFF;

	tick_frequency = TIMER_TICK_FREQUENCY;
	if (settings_speed == GAME_SLOW)
	{
		tick_frequency += 8;
	}
	else if (settings_speed == GAME_FAST)
	{
		tick_frequency -= 12;
	};

	if (settings_map == MAP_COMBS)
	{
		map_ptr = combs;
		map = MAP_COMBS;
	}
	else if (settings_map == MAP_VINES)
	{
		map_ptr = vines;
		map = MAP_VINES;
	}
	else if (settings_map == MAP_OUTDOORS)
	{
		map_ptr = outdoors;
		map = MAP_OUTDOORS;
	}

	if (settings_speed == GAME_SLOW)
	{
		speed_option = SPEED_SLOW;
	}
	else if (settings_speed == GAME_REGULAR)
	{
		speed_option = SPEED_REGULAR;
	}
	else if (settings_speed == GAME_FAST)
	{
		speed_option = SPEED_FAST;
	}

	// load the palettes
	load_bg_palette();
	pal_spr(palette_sp);

	start_round();
}

void load_bg_palette(void)
{
	if (map == MAP_OUTDOORS)
	{
		pal_bg(palette_outdoors_bg);
	}
	else if (map == MAP_COMBS)
	{
		pal_bg(palette_combsnplants_bg);
	}
	else if (map == MAP_VINES)
	{
		pal_bg(palette_vineswoflowers_bg);
	}
}

void init_title_loop(void)
{
	oam_clear();
	delay(30);
	game_mode = MODE_TITLE;
	ppu_off(); // screen off
	// load the title palettes
	pal_bg(palette_title_bg);
	pal_spr(palette_sp);

	load_room(); // loads the title screen;

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

void update_options_screen(void)
{

	// draw a character on the options screen:
	temp_x = 64;
	if (current_settings_choice == SETTING_SPEED)
	{
		temp_y = 96;
	}
	else if (current_settings_choice == SETTING_MAP)
	{
		temp_y = 128;
	}
	else if (current_settings_choice == SETTING_SONG)
	{
		temp_y = 160;
	}
	// draws the values based on the settings:
	multi_vram_buffer_horz("SPEED:", 6, NTADR_A(8, 10));

	// Show current speed selection
	if (settings_speed == GAME_SLOW)
	{
		multi_vram_buffer_horz("SLOW   ", 7, NTADR_A(11, 12));
	}
	else if (settings_speed == GAME_REGULAR)
	{
		multi_vram_buffer_horz("REGULAR", 7, NTADR_A(11, 12));
	}
	else if (settings_speed == GAME_FAST)
	{
		multi_vram_buffer_horz("FAST   ", 7, NTADR_A(11, 12));
	}

	multi_vram_buffer_horz("MAP:", 4, NTADR_A(8, 14));

	if (settings_map == MAP_RANDOM)
	{
		multi_vram_buffer_horz("RANDOM  ", 8, NTADR_A(11, 16));
	}
	else if (settings_map == MAP_OUTDOORS)
	{
		multi_vram_buffer_horz("OUTDOORS", 8, NTADR_A(11, 16));
	}
	else if (settings_map == MAP_COMBS)
	{
		multi_vram_buffer_horz("COMBS   ", 8, NTADR_A(11, 16));
	}
	else if (settings_map == MAP_VINES)
	{
		multi_vram_buffer_horz("VINES   ", 8, NTADR_A(11, 16));
	}

	multi_vram_buffer_horz("MUSIC:", 6, NTADR_A(8, 18));
	if (settings_song == SONG_BEE)
	{
		multi_vram_buffer_horz("BEE  ", 5, NTADR_A(11, 20));
	}
	else if (settings_song == SONG_LAKE)
	{
		multi_vram_buffer_horz("LAKE ", 5, NTADR_A(11, 20));
	}
	else if (settings_song == SONG_HONEY)
	{
		multi_vram_buffer_horz("HONEY", 5, NTADR_A(11, 20));
	}

	multi_vram_buffer_horz("PRESS SELECT TO JOIN", 20, NTADR_A(6, 22));
}

void init_options_loop(void)
{
	// DEBUG
	//  init_game_loop();
	//  return;

	// real code
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

	update_options_screen();

	// Initialize start button variables for options
	start_hold_timer = 0;
	start_held = 0;

	ppu_on_all(); // turn on screen
}

void init_roundover(void)
{
	// increment the win count for the winning team
	// let's draw that last frame:

	// debug_extras(); // draws the timer at 0

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

	if (current_round == 1)
	{
		round_1 = win_reason;
		if (winner == ONETWO_WINNER)
			round_1_winner = TEAM1_WIN;
		else if (winner == THREEFOUR_WINNER)
			round_1_winner = TEAM2_WIN;
	}
	else if (current_round == 2)
	{
		round_2 = win_reason;
		if (winner == ONETWO_WINNER)
			round_2_winner = TEAM1_WIN;
		else if (winner == THREEFOUR_WINNER)
			round_2_winner = TEAM2_WIN;
	}
	else if (current_round == 3)
	{
		round_3 = win_reason;
		if (winner == ONETWO_WINNER)
			round_3_winner = TEAM1_WIN;
		else if (winner == THREEFOUR_WINNER)
			round_3_winner = TEAM2_WIN;
	}
	else if (current_round == 4)
	{
		round_4 = win_reason;
		if (winner == ONETWO_WINNER)
			round_4_winner = TEAM1_WIN;
		else if (winner == THREEFOUR_WINNER)
			round_4_winner = TEAM2_WIN;
	}
	else if (current_round == 5)
	{
		round_5 = win_reason;
		if (winner == ONETWO_WINNER)
			round_5_winner = TEAM1_WIN;
		else if (winner == THREEFOUR_WINNER)
			round_5_winner = TEAM2_WIN;
	}

	if (winner == ONETWO_WINNER)
	{
		if (team1_wins == 1)
			team1_win1 = win_reason;
		else if (team1_wins == 2)
			team1_win2 = win_reason;
		else if (team1_wins == 3)
			team1_win3 = win_reason;
	}
	else if (winner == THREEFOUR_WINNER)
	{
		if (team2_wins == 1)
			team2_win1 = win_reason;
		else if (team2_wins == 2)
			team2_win2 = win_reason;
		else if (team2_wins == 3)
			team2_win3 = win_reason;
	}

	current_round++;

	// if (winner == ONETWO_WINNER)
	// {
	// 	multi_vram_buffer_horz("TEAM 1 WINS!", 12, NTADR_A(9, 12));
	// }
	// else if (winner == THREEFOUR_WINNER)
	// {
	// 	multi_vram_buffer_horz("TEAM 2 WINS!", 12, NTADR_A(9, 12));
	// }

	multi_vram_buffer_horz("                      ", 22, NTADR_A(5, 3));
	if (win_reason == WIN_HONEY_COLLECTED)
	{
		multi_vram_buffer_horz("COLLECTED 100 HONEY", 19, NTADR_A(6, 3));
	}
	else if (win_reason == WIN_FRIENDLY_FIRE_BEE_EATEN)
	{
		multi_vram_buffer_horz("FRIENDLY BEE EATEN", 18, NTADR_A(7, 3));
	}
	else if (win_reason == WIN_FRIENDLY_FIRE_DUCK_EATEN)
	{
		multi_vram_buffer_horz("FRIENDLY DUCK EATEN", 19, NTADR_A(6, 3));
	}
	else if (win_reason == WIN_BEE_EATEN)
	{
		multi_vram_buffer_horz("ENEMY BEE EATEN", 15, NTADR_A(8, 3));
	}
	else if (win_reason == WIN_DUCK_EATEN)
	{
		multi_vram_buffer_horz("BIGBEE ATE DUCK!", 16, NTADR_A(8, 3));
	}
	else if (win_reason == WIN_MOST_HONEY_COLLECTED)
	{
		multi_vram_buffer_horz("COLLECTED MORE HONEY", 20, NTADR_A(6, 3));
	}
	else
	{
		multi_vram_buffer_horz("DRAW GAME!", 10, NTADR_A(11, 3));
	}
}

void display_round_summary(void)
{
	if (temp_winner == TEAM1_WIN)
	{
		multi_vram_buffer_horz("RED TEAM", 8, NTADR_A(16, temp_y));
	}
	else if (temp_winner == TEAM2_WIN)
	{
		multi_vram_buffer_horz("BLUE TEAM", 9, NTADR_A(16, temp_y));
	}
	// display the reason
	if ((temp_round == WIN_HONEY_COLLECTED))
	{
		multi_vram_buffer_horz("100 HONEY COLLECTED", 19, NTADR_A(7, temp_y + 1));
	}
	else if (temp_round == WIN_FRIENDLY_FIRE_BEE_EATEN)
	{
		multi_vram_buffer_horz("ENEMY ATE THEIR BEE", 19, NTADR_A(7, temp_y + 1));
	}
	else if (temp_round == WIN_FRIENDLY_FIRE_DUCK_EATEN)
	{
		multi_vram_buffer_horz("ENEMY ATE THEIR DUCK", 20, NTADR_A(7, temp_y + 1));
	}
	else if (temp_round == WIN_BEE_EATEN)
	{
		multi_vram_buffer_horz("ENEMY BEE EATEN", 15, NTADR_A(7, temp_y + 1));
	}
	else if (temp_round == WIN_DUCK_EATEN)
	{
		multi_vram_buffer_horz("ENEMY DUCK EATEN", 16, NTADR_A(7, temp_y + 1));
	}
	else if (temp_round == WIN_MOST_HONEY_COLLECTED)
	{
		multi_vram_buffer_horz("COLLECTED MORE HONEY", 20, NTADR_A(7, temp_y + 1));
	}

	ppu_wait_nmi();
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
	// don't need to load bg pallete
	pal_spr(palette_sp);

	// say who won
	if (team1_wins >= 3)
	{
		multi_vram_buffer_horz("RED TEAM WINS THE MATCH!", 24, NTADR_A(4, 4));
		winner = ONETWO_WINNER;
	}
	else if (team2_wins >= 3)
	{
		multi_vram_buffer_horz("BLUE TEAM WINS THE MATCH!", 25, NTADR_A(4, 4));
		winner = THREEFOUR_WINNER;
	}

	// position winning team at the top and hide the losers
	bee1_bigbee_timer = 0;
	bee3_bigbee_timer = 0;
	if (winner == ONETWO_WINNER)
	{
		BoxGuy1.x = 0x6E00;
		BoxGuy1.y = 0x1200;
		BoxGuy1.direction = DIR_RIGHT;
		BoxGuy2.x = 0x8100;
		BoxGuy2.y = 0x1200;
		BoxGuy2.direction = DIR_LEFT;
		BoxGuy2.moving = 1;

		BoxGuy3.y = 0xF000;
		BoxGuy4.y = 0xF000;
		BoxGuy3.moving = 0;
		BoxGuy4.moving = 0;
	}
	else if (winner == THREEFOUR_WINNER)
	{
		BoxGuy3.x = 0x6E00;
		BoxGuy3.y = 0x1200;
		BoxGuy3.direction = DIR_RIGHT;
		BoxGuy4.x = 0x8100;
		BoxGuy4.y = 0x1200;
		BoxGuy4.direction = DIR_LEFT;
		BoxGuy4.moving = 1;

		BoxGuy1.y = 0xF000;
		BoxGuy2.y = 0xF000;
		BoxGuy1.moving = 0;
		BoxGuy2.moving = 0;
	}

	// Summary of the rounds:

	temp_y = 8;
	multi_vram_buffer_horz("ROUND 1:", 9, NTADR_A(6, temp_y));
	temp_winner = round_1_winner;
	temp_round = round_1;
	display_round_summary();

	temp_y = 11;
	multi_vram_buffer_horz("ROUND 2:", 9, NTADR_A(6, temp_y));
	temp_winner = round_2_winner;
	temp_round = round_2;
	display_round_summary();

	temp_y = 14;
	multi_vram_buffer_horz("ROUND 3:", 9, NTADR_A(6, temp_y));
	temp_winner = round_3_winner;
	temp_round = round_3;
	display_round_summary();

	if (round_4 != 0)
	{
		temp_y = 17;
		multi_vram_buffer_horz("ROUND 4:", 9, NTADR_A(6, temp_y));
		temp_winner = round_4_winner;
		temp_round = round_4;
		display_round_summary();
	}

	if (round_5 != 0)
	{
		temp_y = 20;
		multi_vram_buffer_horz("ROUND 5:", 9, NTADR_A(6, temp_y));
		temp_winner = round_5_winner;
		temp_round = round_5;
		display_round_summary();
	}

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

	// Initialize default settings
	settings_speed = GAME_REGULAR;
	settings_map = MAP_RANDOM;
	settings_song = SONG_BEE;

	speed_option = SPEED_REGULAR;
	turbo_amount = TURBO_MEDIUM;
	ducks_go_faster_over_time = 1;
	collision_box_size = 6; // quacks and players are 6x6

	// Initialize sprite rotation
	sprite_rotation = 0;

	// Initialize animation frames
	anim_frame_1 = 0;
	anim_frame_2 = 0;
	anim_frame_3 = 0;
	anim_frame_4 = 0;

	// intialize pads
	prev_pad1 = 0;
	prev_pad2 = 0;
	prev_pad3 = 0;
	prev_pad4 = 0;

	use_ai_player_1 = 1;
	use_ai_player_2 = 1;
	use_ai_player_3 = 1;
	use_ai_player_4 = 1;
	ai_timer_p1 = 0;
	ai_timer_p2 = 0;
	ai_timer_p3 = 0;
	ai_timer_p4 = 0;
	ai_dir_p1 = DIR_UP;
	ai_dir_p2 = DIR_UP;
	ai_dir_p3 = DIR_UP;
	ai_dir_p4 = DIR_UP;

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

void roundover_loop(void)
{

	while (1)
	{
		++frame_counter;
		// draw the sprites
		draw_sprites();
		update_hud();
		draw_hud();
		// update to only draw the winners as animated

		ppu_wait_nmi();
		read_controllers();

		if (pad1_new & PAD_START || pad2_new & PAD_START || pad3_new & PAD_START || pad4_new & PAD_START)
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

void game_counters(void)
{
	frame_counter++;
	game_frame_timer++;
	// Update sprite rotation every frame
	sprite_rotation++;

	if (game_frame_timer >= tick_frequency)
	{
		game_frame_timer = 0; // reset the frame timer
		game_timer--;

		// end of round check
		if (game_timer == 0)
		{
			// time's up, check scores
			if (team1_score > team2_score)
			{
				winner = ONETWO_WINNER;
				win_reason = WIN_MOST_HONEY_COLLECTED;
				sfx_play(SFX_TEAM1_WIN, 0);
			}
			else if (team2_score > team1_score)
			{
				winner = THREEFOUR_WINNER;
				win_reason = WIN_MOST_HONEY_COLLECTED;
				sfx_play(SFX_TEAM2_WIN, 0);
			}
			else
			{
				winner = TIE_WINNER;
				win_reason = WIN_TIMEUP;
				sfx_play(SFX_DRAW_GAME, 0);
			}
			init_roundover();
		}

		// decrease quack counter on game_tick
		if (quack2_cooldown > 0)
		{
			quack2_cooldown--;
		}
		if (quack4_cooldown > 0)
		{
			quack4_cooldown--;
		}

		// decrease stun
		if (stun_p1 > 0)
		{
			stun_p1--;
		}
		if (stun_p2 > 0)
		{
			stun_p2--;
		}
		if (stun_p3 > 0)
		{
			stun_p3--;
		}
		if (stun_p4 > 0)
		{
			stun_p4--;
		}

		// Update bigbee transformation timers
		if (bee1_bigbee_timer > 0)
		{
			bee1_bigbee_timer--;
		}
		if (bee3_bigbee_timer > 0)
		{
			bee3_bigbee_timer--;
		}
	}
}