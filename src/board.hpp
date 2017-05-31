//
//  board.hpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 19/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#ifndef board_hpp
#define board_hpp

#include <SDL2/SDL.h>
#include <stdio.h>

class Board
{
public:
    Board(int w, int h, int first_col, int total_col);
    ~Board();
    
    enum directions
    {
        left,
        right,
        down,
        up
    };

    void InitVariables();
    
    // Fill the board with integers from range [0, total_colors]
    // 0 means the board is empty at that location
    void FillBoard();
    
    // This function is called after the player clicked inside the game board
    // And will check if there is a valid play (a block match)
    bool CheckPlay (int clicked_block_x, int clicked_block_y);
    
    // Gets the board value at column <col>, row <row>
    int GetBlockColor(int col, int row, bool& colored);

    // Checks if the neighbor (in the direction given by <direction>) of the block at (pos_x, pos_y) has the color given by <color>
    bool CheckNeighbor(int color, int pos_x, int pos_y, int direction);
    
    // Checks all neighbors of the block at (pos_x, pos_y) for the color given by <color>
    bool CheckAllNeighbors(int color, int pos_x, int pos_y);
    
    bool IsColumnEmpty(int column_idx);

    // Find the next non-empty column to the left of column index given by <column_idx>
    // Returns -1 if there are only empty columns
    int FindNextNonEmptyColumn(int column_idx);

    // Apply gravity to the column given by <column> and update its number of blocks
    void ApplyColumnGravity (int* column, int& num_blocks);

    void ApplyBoardGravity();
    
    // Moves the column given by <src_column> to the column given by <dst_column>
    void MoveColumn(int *dst_column, int* src_column);
    
    // If there is a gap between columns, boxes will collapse towards the earlier columns
    // This function will return the number of empty columns found
    int CollapseBoard();

    // When a new column appears on the right, we first shift the board to the left
    void ShiftBoardLeft();

    int* CreateColumn(int column_size);

    // Temporarily, 1 (or more) columns appear on the board (a wave)
    void CreateWave();

    int GetFirstColumn() const;
    
    int GetWidth() const;
    
    int GetHeight() const;
    
    int GetScore() const;
    
    // Resets the score, increments the level and changes some variables
    // To make the next level a bit harder
    void NextLevel();
    
    // Receives information (from the Scene) about what the maximum level is
    void SetMaxLevel(int lev);
    
    bool IsGameOver() const;
    
    // Resets variables
    void Reset(int new_first_column, int new_total_colors);
private:
    //Disallow copying and assignment.
    Board(const Board&);
    Board& operator=(const Board&);
    
    const int width;
    const int height;
    
    int **board;
    
    const int no_color = 0;
    int total_colors;
    int first_column;
    
    int* number_blocks; // The number of blocks in each column
    
    bool possible_play = false;
    bool game_over;
    
    // Number of points per block match
    const int match_points = 5;
    int score;
    int level;
    int max_level;
};


#endif /* board_hpp */
