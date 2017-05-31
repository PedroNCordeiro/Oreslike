//
//  board.cpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 19/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#include "board.hpp"

Board::Board(int w, int h, int first_col, int total_col) : width(w), height(h), first_column(first_col), total_colors(total_col)
{
    // Allocate memory
    board = new int* [width];
    for (int i = 0; i < width; i++) {
        board[i] = new int [height];
    }
    
    number_blocks = new int[width];
    
    InitVariables();
}

// Deallocate our board
Board::~Board()
{
    for (int i = 0; i < width; i++) {
        delete [] board[i];
    }
    delete [] board;

    delete [] number_blocks;
}

void Board::InitVariables()
{
    game_over = false;
    score = 0;
    level = 1;
}

void Board::FillBoard()
{
    for (int col = 0; col < width; col ++) {
        for (int row = 0; row < height; row ++) {
            
            // We will leave some columns empty to prevent the player from losing too soon
            if (col < first_column)
            {
                board[col][row] = no_color;
            }
            else
            {
                // Let's give a random color to each block
                // Since the color represented by 0 is <no_color> we add 1 to avoid creating empty blocks
                board[col][row] = 1 + rand() % (total_colors);
            }
        }
        
        // Update the number of blocks per column
        number_blocks[col] = (col < first_column) ? 0 : height;
    }
}

bool Board::CheckPlay (int clicked_block_x, int clicked_block_y)
{
    // Check if the click was on a colored block
    if ( board[clicked_block_x][clicked_block_y] == no_color)
    {
        return false;
    }
    
    int clicked_color = board[clicked_block_x][clicked_block_y];
    
    // Checks if there is a possible play
    // I.e. if there is an adjacent block of the same color as the one clicked by the player
    possible_play = CheckAllNeighbors(clicked_color, clicked_block_x, clicked_block_y);
    
    if (possible_play)
    {
        ApplyBoardGravity();
        first_column += CollapseBoard();
    }
    
    return possible_play;
}

int Board::GetBlockColor(int col, int row, bool& colored)
{
    colored = (board[col][row] == no_color) ? false : true;
    
    return board[col][row];
}

bool Board::CheckNeighbor(int color, int pos_x, int pos_y, int direction)
{
    int neighbor_pos_x = pos_x;
    int neighbor_pos_y = pos_y;
    
    switch (direction) {
        case left:
            if (pos_x <= 0) // Already at the leftmost position
            {
                return false;
            }
            neighbor_pos_x --;
            break;
        case right:
            if (pos_x >= width - 1) // Already at the rightmost position
            {
                return false;
            }
            neighbor_pos_x ++;
            break;
        case up:
            if (pos_y >= height - 1) // Already at the topmost position
            {
                return false;
            }
            neighbor_pos_y ++;
            break;
        case down:
            if (pos_y <= 0) // Already at the lowest position
            {
                return false;
            }
            neighbor_pos_y --;
            break;
        default:
            break;
    }
    
    if (board[neighbor_pos_x][neighbor_pos_y] == color)
    {
        // Both blocks are matched, they shall disappear
        board[pos_x][pos_y] = no_color;
        board[neighbor_pos_x][neighbor_pos_y] = no_color;
        score += match_points;
        
        // Check all neighbors, except the original one
        if (direction != down)
        {
            CheckNeighbor(color, neighbor_pos_x, neighbor_pos_y, up);
        }
        
        if (direction != up)
        {
            CheckNeighbor(color, neighbor_pos_x, neighbor_pos_y, down);
        }
        
        if (direction != left)
        {
            CheckNeighbor(color, neighbor_pos_x, neighbor_pos_y, right);
        }
        
        if (direction != right)
        {
            CheckNeighbor(color, neighbor_pos_x, neighbor_pos_y, left);
        }
        
        return true;
    }
    
    return false;
}

bool Board::CheckAllNeighbors(int color, int pos_x, int pos_y)
{
    // Will we get a block match?
    bool blocks_match = false;
    
    blocks_match |= CheckNeighbor(color, pos_x, pos_y, down);
    blocks_match |= CheckNeighbor(color, pos_x, pos_y, up);
    blocks_match |= CheckNeighbor(color, pos_x, pos_y, left);
    blocks_match |= CheckNeighbor(color, pos_x, pos_y, right);
    
    return blocks_match;
}

bool Board::IsColumnEmpty(int column_idx)
{
    // Since this function is called after column gravity is applied
    // We just need to check if the first position is empty
    return board[column_idx][0] == no_color;
}

int Board::FindNextNonEmptyColumn(int column_idx)
{
    for (int col = column_idx - 1; col >= first_column; col --) {
        if (!IsColumnEmpty(col))
        {
            return col;
        }
    }
    
    return -1;
}

void Board::ApplyColumnGravity(int *column, int& num_blocks)
{
    int empty_blocks = 0;
    
    // We don't need to travel the entire column if its not full of blocks
    // We just need to go up to the highest one (given by <num_blocks>)
    for (int row = 0; row < num_blocks; row ++) {
        
        if (column[row] == no_color) // Found an empty block
        {
            empty_blocks ++;
            continue;
        }
        
        if (empty_blocks > 0)
        {
            column[row - empty_blocks] = column[row]; // Block moves to the lowest empty position
            column[row] = no_color;
        }
    }
    
    // Update the number of blocks for this column
    num_blocks -= empty_blocks;
}

void Board::ApplyBoardGravity()
{
    for (int col = first_column; col < width; col ++) {
        ApplyColumnGravity(board[col], number_blocks[col]);
    }
}

void Board::MoveColumn(int *dst_column, int *src_column)
{
    for (int row = 0; row < height; row ++) {
        dst_column[row] = src_column[row];
        src_column[row] = no_color;
    }
}

int Board::CollapseBoard()
{
    int empty_columns = 0;
    
    // Loop our columns from right to left
    for (int col = width - 1; col >= first_column; col --) {
        
        if (IsColumnEmpty(col)) // Found an empty column
        {
            empty_columns ++;
            continue;
        }
        
        if (empty_columns > 0)
        {
            MoveColumn(board[col + empty_columns], board[col]);
            
            // Update the number of blocks in both columns
            number_blocks[col + empty_columns] = number_blocks[col];
            number_blocks[col] = 0;
        }
    }
    
    return empty_columns;
}

void Board::ShiftBoardLeft()
{
    for (int col = first_column - 1; col < width - 1; col ++) {
        for (int row = 0; row < height; row ++) {
            // Copy each column to the left
            board[col][row] = board[col + 1][row];
        }
        
        // Update number of blocks per column
        number_blocks[col] = number_blocks[col + 1];
    }
    // Update the leftmost column
    first_column --;
}

int* Board::CreateColumn(int column_size)
{
    if (column_size < 1)
    {
        printf("Warning: Attempting to create a an empty column.\n");
        return NULL;
    }
    
    int* column = new int[column_size];
    
    for (int row = 0; row < column_size; row ++) {
        column[row] = 1 + rand() % (total_colors); // Randomise the block values
    }
    
    return column;
}

void Board::CreateWave()
{
    // We can't create any more columns since the board is full
    // It's game over
    if (first_column <= 0)
    {
        game_over = true;
        return;
    }
    
    ShiftBoardLeft();
    
    // Now we create the new column on the right
    board[width - 1] = CreateColumn(height);
    number_blocks[width - 1] = height;
}

int Board::GetFirstColumn() const
{
    return first_column;
}

int Board::GetWidth() const
{
    return this->width;
}

int Board::GetHeight() const
{
    return this->height;
}

int Board::GetScore() const
{
    return this->score;
}

void Board::NextLevel()
{
    if (level <= max_level)
    {
        level++;
        first_column--; // The next level starts with less empty columns
        total_colors++; // Incrementing the number of block colors to make it harder
        score = 0;
    }
}

void Board::SetMaxLevel(int lev)
{
    this->max_level = lev;
}

bool Board::IsGameOver() const
{
    return this->game_over;
}

void Board::Reset(int new_first_column, int new_total_colors)
{
    InitVariables();
    
    first_column = new_first_column;
    total_colors = new_total_colors;
    
    // Fill the board once again with the new values of the first column and the number of colors
    FillBoard();
}
