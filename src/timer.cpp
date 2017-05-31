//
//  timer.cpp
//  Oreslike
//
//  Created by Pedro Cordeiro on 19/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#include "timer.hpp"

Timer::Timer() {
    this->start_time = 0;
}

void Timer::Start()
{
    this->start_time = SDL_GetTicks();
}

Uint32 Timer::GetTime()
{
    return SDL_GetTicks() - start_time;
}

void Timer::Pause()
{
    this->paused_time = SDL_GetTicks();
}

void Timer::Unpause()
{
    this->start_time += SDL_GetTicks() - paused_time;
}
