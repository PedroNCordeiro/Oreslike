//
//  common.h
//  Oreslike
//
//  Created by Pedro Cordeiro on 21/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#ifndef common_h
#define common_h

#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;

// Block dimension constants
const int BLOCK_WIDTH = 50;
const int BLOCK_HEIGHT = 50;

//Board dimension constants
const int BOARD_WIDTH = 12;
const int BOARD_HEIGHT = 12;

// Board position
const int BOARD_POSITION_X = SCREEN_WIDTH - (BOARD_WIDTH * BLOCK_WIDTH) - 40;
const int BOARD_POSITION_Y = 10;

const float WAVE_TIME = 5.0f; // The time (in seconds) between waves (a wave corresponds to the new columns that appear on the board)

// We will have 3 empty columns on the left to avoid losing too quickly
const int FIRST_BOARD_COLUMN = 7;

// Total block colors to be used in the game
// Note: Shouldn't be greater than <maximum_colors>
const int TOTAL_COLORS = 3;

// Maximum number of block colors allowed in the game
const int MAXIMUM_COLORS = 7;
const std::string POSSIBLE_COLORS[MAXIMUM_COLORS] = {"red", "blue", "green", "yellow", "turkish", "grey", "purple"};

// Number of different .wav sounds for block matching
const int TOTAL_SOUNDS = 5;
const std::string POSSIBLE_SOUNDS[TOTAL_SOUNDS] = {"acid5", "acid6", "flagreturn", "flagdrop", "boing_x"};

// Level position
const int LEVEL_POS_X = 50;
const int LEVEL_POS_Y = SCREEN_HEIGHT - 100;

// Score position
const int SCORE_POS_X = BOARD_POSITION_X;
const int SCORE_POS_Y = SCREEN_HEIGHT - BOARD_POSITION_Y - BOARD_HEIGHT * BLOCK_HEIGHT - 50;


// Max score per level
const int MAX_SCORE = 750;

// UI position
const int UI_POS_X = 10;
const int UI_POS_Y = 40;

// number of levels
const int MAX_LEVELS = 3;

#endif /* common_h */
