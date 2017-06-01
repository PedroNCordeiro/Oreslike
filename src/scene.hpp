//
//  scene.hpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 20/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#ifndef scene_hpp
#define scene_hpp

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>

#include "common.h"
#include "board.hpp"
#include "timer.hpp"
#include "texture.hpp"
#include "block.hpp"

class Scene
{
public:
    // Initializing variables, creating timers and filling the board
    Scene(int w, int h);
    
    void InitVariables();
    
    //Start up SDL and create window
    bool InitSDL();
    
    // Load textures, fonts and music/sound effects
    bool LoadMedia();
    
    void PauseTimer();
    void UnpauseTimer();
    
    void CreateWave(); // Create a new column on the righmost position
    void CheckForWave(); // Checks how much time to the next wave
    void CreateTimer(); // Creates the timer responsible to track the time to the next wave

    // Called every frame by the Engine to check if the game is over or if the player has passed to the next level
    void Update();
    
    
    // Checks if the player click was inside the board and if so if there is a valid play
    void CheckClick(int clicked_pos_x, int clicked_pos_y);
    
    void ConvertScreenToBoardCoordinates(int *x_pos, int *y_pos);
    
    // Generic Function that calls all the other rendering functions
    void Draw();
    void RenderBackground(); // Renders two background images to make the parallax effect
    void RenderProgressBar(float percent); // The progress bar will move from left to right
    void RenderBlock(int color, int board_x, int board_y, bool colored); // Render the block at column <board_x> and row <board_y> depending on its value (given by <color>). If the block is empty we still draw a box ouline
    void RenderBoard();
    void RenderLevel(); // The level progress
    void RenderScore(); // The score progress
    void RenderUI();    // Displays the keys the user can press during play
    void RenderGameOver();
    void RenderEndGame(); // End game message
    
    void StartMusic();
    void PauseMusic();
    void ResumeMusic();
    void PlayBlockSoundEffect(); // Sound is played when the player finds a block match
    void PlayWaveSoundEffect(); // Sound is played when a wave is created
    void PlayGameOverSoundEffect();
    
    void NextLevel();
    bool IsGameOver() const;
    void RestartGame(); // Resets the board
    
    bool FinishedGame() const; // Has player finished the game?
    
    void Close(); // Deallocates resources. Called by the Engine at the end of the program when the payer quits

private:
    //Disallow copying and assignment.
    Scene(const Scene&);
    Scene& operator=(const Scene&);

    const int width;
    const int height;
    
    Timer wave_timer;
    float wave_progress;
    
    Board board; // Our game board
        
    // Distance between progress bar and the top of the board
    const int progress_bar_offset_x = 0;
    const int progress_bar_offset_y = 20;
    const int progress_bar_width = 70;
    const int progress_bar_height = 20;

    SDL_Window* window; // THE window
    SDL_Renderer* renderer; // THE screen renderer
    
    // Background variables
    Texture background;
    int scrolling_offset = 0;
    Mix_Music* bg_music;
    
    // Block resources
    Block blocks[MAXIMUM_COLORS];
    //Texture block_textures[MAXIMUM_COLORS];
    Mix_Chunk* block_sound[TOTAL_SOUNDS];
    
    // Score display
    Texture score_texture;
    TTF_Font* score_font;
    
    // Level display
    Texture level_texture;
    TTF_Font* level_font;
    
    // Game Over display
    Texture game_over_texture;
    TTF_Font* game_over_font;
    Mix_Chunk* game_over_sound;
    
    // End game display
    Texture end_game_texture;
    
    // UI
    Texture ui_texture;
    
    // New wave sound
    Mix_Chunk* wave_sound;
    
    int score; // player score
    int level; // player level
    bool update_score; // Should the score display be updated?
    bool update_level; // Should the level display be updated?
    bool game_over_sound_played; // Keeps track of whether the gameoversound has been played
    bool finished;      // Has the player finished the game?
};

#endif /* scene_hpp */
