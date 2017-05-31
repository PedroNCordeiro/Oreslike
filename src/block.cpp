//
//  block.cpp
//  oreslike2
//
//  Created by Pedro Cordeiro on 31/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#include "block.hpp"

Block::Block(std::shared_ptr<Texture> t)
{
    this->texture = t;
}

std::shared_ptr<Texture> Block::GetTexture() const
{
    return this->texture;
}
