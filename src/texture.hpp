//
//  texture.hpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 22/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>

class Texture
{
public:
    // Initialize variables
    Texture();

    // Calls Free()
    ~Texture();

    //Deallocates texture
    void Free();

    //Loads image at specified path
    bool LoadFromFile( std::string path );
    
    //Creates image from font string
    bool LoadFromRenderedText( std::string texture_text, SDL_Color text_color, TTF_Font* font );    
    
    //Renders texture at given point
    void Render( int x, int y, SDL_Rect* clip = NULL );
    
    //Gets image dimensions
    int GetWidth() const;
    int GetHeight() const;
    
    // Receive a reference to the screen_renderer (called by the game Scene)
    void SetScreenRenderer(SDL_Renderer* renderer);
    
private:
    // Our texture
    SDL_Texture* sdl_texture;
    
    // Our screen renderer
    SDL_Renderer* screen_renderer;
    
    // image dimensions
    int width;
    int height;
};


#endif /* texture_hpp */
