//
//  scene.cpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 20/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#include "scene.hpp"

Scene::Scene(int w, int h) : board(BOARD_WIDTH, BOARD_HEIGHT, FIRST_BOARD_COLUMN, TOTAL_COLORS), width(w), height(h)
{
    window = NULL;
    renderer = NULL;
    for (int i = 0; i < TOTAL_SOUNDS; i++ )
    {
        block_sound[i] = NULL;
    }
    bg_music = NULL;
    wave_sound = NULL;
    game_over_sound = NULL;
    score_font = NULL;
    level_font = NULL;
    game_over_font = NULL;
    
    CreateTimer();
    board.FillBoard();
    board.SetMaxLevel(MAX_LEVELS);
    
    InitVariables();
}

void Scene::InitVariables()
{
    score = 0;
    level = 1;
    update_score = true;
    update_level = true;
    game_over_sound_played = false;
    finished = false;
}

bool Scene::InitSDL()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        //Create window
        window = SDL_CreateWindow( "Oreslike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( renderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                //Initialize PNG loading
                int img_flags = IMG_INIT_PNG;
                if( !( IMG_Init( img_flags ) & img_flags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                
                //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
                
                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
            }
        }
    }
    
    return success;
}

bool Scene::LoadMedia()
{
    //Loading success flag
    bool success = true;
    
    background.SetScreenRenderer(renderer);
    if (!background.LoadFromFile("assets/images/background.png"))
    {
        printf( "Failed to load background texture!\n" );
        success = false;
    }

    //Load PNG block textures
    std::string color_name;
    for (int i = 0; i < MAXIMUM_COLORS; i++) {
        block_textures[i].SetScreenRenderer(renderer);
        
        color_name = "assets/images/" + POSSIBLE_COLORS[i] + ".png";
        if (!block_textures[i].LoadFromFile(color_name))
        {
            printf( "Failed to load block texture!\n" );
            success = false;
        }
    }
   
    //Load sound effects
    std::string sound_name;
    for (int i = 0; i < TOTAL_SOUNDS; i++) {
        sound_name = "assets/sound/sci_fi/" + POSSIBLE_SOUNDS[i] + ".wav";
        block_sound[i] = Mix_LoadWAV( sound_name.c_str() );
        if( block_sound[i] == NULL )
        {
            printf( "Failed to load block sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    }
    
    // Load new wave sound effect
    wave_sound = Mix_LoadWAV ( "assets/sound/sci_fi/teleport.wav" );
    if (wave_sound == NULL)
    {
        printf( "Failed to load new wave sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    
    // Load background music
    bg_music = Mix_LoadMUS( "assets/sound/caketown.mp3" );
    if( bg_music == NULL )
    {
        printf( "Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    // Load the font for the score
    score_font = TTF_OpenFont( "assets/fonts/leadcoat.ttf", 42 );
    if( score_font == NULL )
    {
        printf( "Failed to load score font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    // Set score_texture renderer
    score_texture.SetScreenRenderer(renderer);

        
    // Load UI Texture
    ui_texture.SetScreenRenderer(renderer);
    if (!ui_texture.LoadFromFile("assets/images/ui.png"))
    {
        printf( "Failed to load UI texture!\n" );
        success = false;
    }
    
    // Load the font for the level
    level_font = TTF_OpenFont( "assets/fonts/leadcoat.ttf", 52 );
    if( level_font == NULL )
    {
        printf( "Failed to load level font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    // Set level renderer
    level_texture.SetScreenRenderer(renderer);

    
    // Load the font for Game Over
    game_over_font = TTF_OpenFont( "assets/fonts/game_over.ttf", 120 );
    if( game_over_font == NULL )
    {
        printf( "Failed to load gameOver font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    // Set renderer
    game_over_texture.SetScreenRenderer(renderer);

    
    // Load gameOver sound effect
    game_over_sound = Mix_LoadWAV ( "assets/sound/sci_fi/gameover.wav" );
    if (game_over_sound == NULL)
    {
        printf( "Failed to load gameOver sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    // End Game renderer
    end_game_texture.SetScreenRenderer(renderer);
    if ( !end_game_texture.LoadFromFile("assets/images/end_game.png") )
    {
        printf( "Failed to load end game message texture!\n" );
        success = false;
    }

    
    return success;
}

void Scene::PauseTimer()
{
    wave_timer.Pause();
}

void Scene::UnpauseTimer()
{
    wave_timer.Unpause();
}

void Scene::CreateWave()
{
    PlayWaveSoundEffect();
    
    wave_progress = 0.0f;
    board.CreateWave();
    wave_timer.Start();
}

void Scene::CheckForWave()
{
    // Create a new wave of columns temporarily
    float time_since_last_wave = wave_timer.GetTime() / 1000.0f;
    wave_progress = time_since_last_wave / WAVE_TIME;
    
    if (time_since_last_wave >= WAVE_TIME)
    {
        board.CreateWave();

        PlayWaveSoundEffect();
        
        // Restart timer
        wave_timer.Start();
    }
}

void Scene::CreateTimer()
{
    wave_timer = Timer();
    wave_timer.Start();
}

void Scene::Update()
{
    if (board.IsGameOver())
    {   if (game_over_sound_played) return;
        PlayGameOverSoundEffect();
        game_over_sound_played = true; // Avoid playing the sound multiples times
    }
    
    if (score >= MAX_SCORE)
    {
        // The player finished the game
        if (level >= MAX_LEVELS)
        {
            finished = true;
            return;
        }
        
        level++;
        update_level = true;
        NextLevel();
        board.FillBoard();
        score = 0;
    }
    
    CheckForWave();
}

void Scene::CheckClick(int clicked_pos_x, int clicked_pos_y)
{
    int click_x = clicked_pos_x;
    int click_y = clicked_pos_y;
    
    ConvertScreenToBoardCoordinates( &click_x, &click_y );
    
    //Check if the click was inside of the board
    if ( click_x <= BOARD_WIDTH - 1 && click_x >= 0 && click_y <= BOARD_HEIGHT - 1 && click_y >=0 )
    {
        // If there is a valid play
        if (board.CheckPlay(click_x, click_y))
        {
            update_score = true;
            score = board.GetScore();
            
            PlayBlockSoundEffect();
        }
    }
    
}

void Scene::ConvertScreenToBoardCoordinates(int *x_pos, int *y_pos)
{
    *y_pos = (height - BOARD_POSITION_Y - *y_pos) / BLOCK_HEIGHT;
    *x_pos = (*x_pos - BOARD_POSITION_X) / BLOCK_WIDTH;
}

void Scene::RenderBlock(int color, int board_x, int board_y, bool colored)
{
    int lowest_block_y_position = height - BLOCK_HEIGHT - BOARD_POSITION_Y;
    
    // This will store the x, y, width and height of each block
    // Note: The blocks are drawn from left to right, from below to above
    SDL_Rect block_rect = { BOARD_POSITION_X + board_x * BLOCK_WIDTH, lowest_block_y_position - board_y * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT };
    
    if (colored)
    {
        //Render texture to screen
        block_textures[color - 1].Render(block_rect.x, block_rect.y);
    }
    // Draw black outline
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderDrawRect(renderer, &block_rect );
}

void Scene::RenderBoard()
{
    int board_width = board.GetWidth();
    int board_height = board.GetHeight();
    
    int block_color;
    bool colored = true;
    
    for (int col = 0; col < board_width; col ++) {
        for (int row = 0; row < board_height; row ++) {
            block_color = board.GetBlockColor(col, row, colored);
            RenderBlock(block_color, col, row, colored);
        }
    }
}

void Scene::RenderProgressBar (float percent)
{
    // Positioning
    int position_x = BOARD_POSITION_X + BOARD_WIDTH * BLOCK_WIDTH - progress_bar_width;
    int position_y = this->height - BOARD_POSITION_Y - BOARD_HEIGHT * BLOCK_HEIGHT - progress_bar_offset_y - progress_bar_height;
    
    // Background
    SDL_Color bg_color = { 0x00, 0x00, 0x00, 0xFF }; // black
    SDL_Rect bg_rect = { position_x, position_y, progress_bar_width, progress_bar_height };
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderFillRect(renderer, &bg_rect);
    
    // Foreground
    SDL_Color fg_color = { 0x00, 0xFF, 0x00, 0xFF }; // green
    int p_width = (int)((float)progress_bar_width * percent);
    SDL_Rect fg_rect = { position_x, position_y, p_width, progress_bar_height };
    SDL_SetRenderDrawColor(renderer, fg_color.r, fg_color.g, fg_color.b, fg_color.a);
    SDL_RenderFillRect(renderer, &fg_rect);
}

void Scene::RenderBackground()
{
    // We render two equal images to simulate the parallax effect
    scrolling_offset -= 1;
    if( scrolling_offset < -background.GetWidth() )
    {
        scrolling_offset = 0;
    }
    
    background.Render(scrolling_offset, 0);
    background.Render(scrolling_offset + background.GetWidth(), 0);
}

void Scene::RenderScore()
{
    // Only load a new texture when the score has changed
    if (update_score)
    {
        SDL_Color text_color = { 0x00, 0x00, 0x00, 0xFF }; //black
        std::string score_text = "Score: " + std::to_string(score) + " / " + std::to_string(MAX_SCORE);
        if (!score_texture.LoadFromRenderedText(score_text, text_color, score_font))
        {
            printf( "Failed to load score texture!\n" );
        }
    }
    score_texture.Render(SCORE_POS_X, SCORE_POS_Y);
        
    update_score = false;
}

void Scene::RenderLevel()
{
    // Only load a new texture when the level has changed
    if (update_level)
    {
        SDL_Color text_color = { 0x40, 0x40, 0x40, 0xFF }; //gray
        std::string level_text = "Level: " + std::to_string(level) + " / " + std::to_string(MAX_LEVELS);
        if (!level_texture.LoadFromRenderedText(level_text, text_color, level_font))
        {
            printf( "Failed to load level texture!\n" );
        }
    }
    level_texture.Render(LEVEL_POS_X, LEVEL_POS_Y);
    
    update_level = false;
}

void Scene::RenderUI()
{
    ui_texture.Render(UI_POS_X, UI_POS_Y);
}

void Scene::RenderGameOver()
{
    SDL_Color text_color = { 0xFF, 0x00, 0x00, 0xFF }; // red
    std::string game_over_text = "Press >Enter< to Restart";
    if (!game_over_texture.LoadFromRenderedText(game_over_text, text_color, game_over_font))
    {
        printf( "Failed to load gameOver texture!\n" );
    }
    game_over_texture.Render(SCORE_POS_X, 20);
}

void Scene::RenderEndGame()
{
    end_game_texture.Render(0, 0);
}

void Scene::Draw()
{
    //Clear screen
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( renderer );
    
    RenderBackground();
    
    // Check for end game
    if (FinishedGame())
    {
        RenderEndGame();
        //Update screen
        SDL_RenderPresent( renderer );
        return;
    }
    
    RenderBoard();
    RenderLevel();
    
    // Check for game Over
    if (board.IsGameOver())
    {
        RenderGameOver();
    }
    else    // We hide the Score, the UI and the progress bar when the Game Over texture appears
    {
        RenderScore();
        RenderUI();
        RenderProgressBar(wave_progress);
    }
    
    //Update screen
    SDL_RenderPresent( renderer );
}

void Scene::StartMusic()
{
    Mix_PlayMusic( bg_music, -1 );
}

void Scene::PauseMusic()
{
    Mix_PauseMusic();
}

void Scene::ResumeMusic()
{
    Mix_ResumeMusic();
}

void Scene::PlayBlockSoundEffect()
{
    int rnd_sound = rand() % TOTAL_SOUNDS;
    Mix_Volume( -1, MIX_MAX_VOLUME/4 ); // Reduce volume to 25%
    Mix_PlayChannel( -1, block_sound[rnd_sound], 0 );
}

void Scene::PlayWaveSoundEffect()
{
    Mix_Volume( -1, MIX_MAX_VOLUME/2 ); // Reduce volume to 50%
    Mix_PlayChannel( -1, wave_sound, 0 );
}

void Scene::PlayGameOverSoundEffect()
{
    Mix_HaltMusic(); // Stop background music
    Mix_PlayChannel( 1, game_over_sound, -1 );
}

void Scene::NextLevel()
{
    board.NextLevel();
}

bool Scene::IsGameOver() const
{
    return board.IsGameOver();
}

void Scene::RestartGame()
{
    InitVariables();
    StartMusic();
    Mix_HaltChannel(1);
    board.Reset(FIRST_BOARD_COLUMN, TOTAL_COLORS);
}

bool Scene::FinishedGame() const
{
    return this->finished;
}

void Scene::Close()
{
    //Free the sound effects
    for (int i = 0; i < TOTAL_SOUNDS; i++ )
    {
        Mix_FreeChunk( block_sound[i] );
        block_sound[i] = NULL;
    }
    Mix_FreeChunk( wave_sound );
    wave_sound = NULL;
    Mix_FreeChunk( game_over_sound );
    game_over_sound = NULL;
    
    // Free background music
    Mix_FreeMusic( bg_music );
    bg_music = NULL;
    
    // Free textures
    background.Free();
    ui_texture.Free();
    game_over_texture.Free();
    end_game_texture.Free();
    score_texture.Free();
    level_texture.Free();
    for (int i = 0; i < MAXIMUM_COLORS; i ++)
    {
        block_textures[i].Free();
    }
    
    
    // Free fonts
    TTF_CloseFont( score_font );
    TTF_CloseFont( level_font );
    TTF_CloseFont( game_over_font );
    score_font = NULL;
    level_font = NULL;
    game_over_font = NULL;
    
    
    //Destroy window and renderer
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    window = NULL;
    renderer = NULL;
    
    //Quit SDL subsystems
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
