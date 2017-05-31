//
//  engine.hpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 19/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#ifndef engine_hpp
#define engine_hpp

#include <SDL2/SDL.h>
#include <stdio.h>

#include "scene.hpp"

class Engine
{
public:
    // The constructor initializes the screen
    Engine();
    
    // Main engine function
    void Run();
    
    void GameOver();
    
    // Handles player input
    void HandleEvent( SDL_Event& e );

private:
    //Disallow copying and assignment.
    Engine(const Engine&);
    Engine& operator=(const Engine&);

    // The main game scene
    Scene scene;
    
    //The window we'll be rendering to
    SDL_Window* window;
    
    //The window renderer
    SDL_Renderer* window_renderer;
    
    // Saves the state of the game (paused/unpaused)
    bool paused;
    
    bool game_over;
};

#endif /* engine_hpp */
