//
//  engine.cpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 19/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#include "engine.hpp"

Engine::Engine() : scene(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    paused = false;
}

void Engine::HandleEvent( SDL_Event &e )
{
    // Handling Game Over and End Game scenarios
    if (scene.IsGameOver() || scene.FinishedGame())
    {
        // Press <Enter> for restart
        if ( e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN )
        {
            scene.RestartGame();
        }
        return;
    }
    
    // Spacebar ( Pause / Unpause )
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE)
    {
        paused = !paused;
        if (paused)
        {
            scene.PauseMusic();
            scene.PauseTimer();
        }
        else
        {
            scene.ResumeMusic();
            scene.UnpauseTimer();
        }
    }

    if (paused) return;
    
    // Left mouse button (Make plays)
    if (e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_PRESSED)
    {
        //Get mouse position
        int click_position_x, click_position_y;
        SDL_GetMouseState( &click_position_x, &click_position_y );
        
        // Check if there is a valid play
        scene.CheckClick(click_position_x, click_position_y);
    }
    
    // Right mouse button (Create new wave)
    if (e .button.button == SDL_BUTTON_RIGHT && e.button.state == SDL_PRESSED)
    {
        scene.CreateWave();
    }
}

void Engine::Run()
{
    //Start up SDL and create window
    if ( !scene.InitSDL() )
    {
        printf( "Failed to initialize SDL!\n" );
    }
    
    else
    {
        //Load media
        if( !scene.LoadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            scene.StartMusic();
            
            //Main loop flag
            bool quit = false;
            SDL_Event event;
            
            //While application is running
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &event ) != 0 )
                {
                    //User requests quit
                    if( event.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    
                    HandleEvent( event );
                }
                
                if (paused)
                {
                    continue;
                }
                
                scene.Update();
                scene.Draw();
            }
        }
    }
    
    //Free resources and close SDL
    scene.Close();
}
