//
//  block.hpp
//  oreslike2
//
//  Created by Pedro Cordeiro on 31/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#ifndef block_hpp
#define block_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <memory>

#include "texture.hpp"

class Block
{
public:
    Block(std::shared_ptr<Texture> t);
    std::shared_ptr<Texture> GetTexture() const;
private:
    std::shared_ptr<Texture> texture;
};


#endif /* block_hpp */
