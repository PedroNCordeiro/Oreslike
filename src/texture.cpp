//
//  texture.cpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 22/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#include "texture.hpp"

Texture::Texture()
{
    sdl_texture = NULL;
    width = 0;
    height = 0;
}

Texture::~Texture()
{
    Free();
}

void Texture::Free()
{
    //Free texture if it exists
    if( sdl_texture != NULL )
    {
        SDL_DestroyTexture( sdl_texture );
        sdl_texture = NULL;
        width = 0;
        height = 0;
    }
}

bool Texture::LoadFromFile( std::string path )
{
    //Get rid of preexisting texture
    Free();
    
    //The final texture
    SDL_Texture* new_texture = NULL;
    
    //Load image at specified path
    SDL_Surface* loaded_surface = IMG_Load( path.c_str() );
    if( loaded_surface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loaded_surface, SDL_TRUE, SDL_MapRGB( loaded_surface->format, 0, 0xFF, 0xFF ) );
        
        //Create texture from surface pixels
        new_texture = SDL_CreateTextureFromSurface( screen_renderer, loaded_surface );
        if( new_texture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            width = loaded_surface->w;
            height = loaded_surface->h;
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loaded_surface );
    }
    
    //Return success
    sdl_texture = new_texture;
    return sdl_texture != NULL;
}

bool Texture::LoadFromRenderedText( std::string texture_text, SDL_Color text_color, TTF_Font* font )
{
    //Get rid of preexisting texture
    Free();
    
    //Render text surface
    SDL_Surface* text_surface = TTF_RenderText_Solid( font, texture_text.c_str(), text_color );
    if( text_surface != NULL )
    {
        //Create texture from surface pixels
        sdl_texture = SDL_CreateTextureFromSurface( screen_renderer, text_surface );
        if( sdl_texture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            width = text_surface->w;
            height = text_surface->h;
        }
        
        //Get rid of old surface
        SDL_FreeSurface( text_surface );
    }
    else
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    
    
    //Return success
    return sdl_texture != NULL;
}

void Texture::Render( int x, int y, SDL_Rect* clip)
{
    //Set rendering space and render to screen
    SDL_Rect render_quad = { x, y, width, height };
    
    //Set clip rendering dimensions
    if( clip != NULL )
    {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }
    
    //Render to screen
    SDL_RenderCopy( screen_renderer, sdl_texture, clip, &render_quad );
}

int Texture::GetWidth() const
{
    return width;
}

int Texture::GetHeight() const
{
    return height;
}

void Texture::SetScreenRenderer(SDL_Renderer* renderer)
{
    this->screen_renderer = renderer;
}
