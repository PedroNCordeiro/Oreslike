//
//  timer.hpp - Our main timer
//  Oreslike
//
//  Created by Pedro Cordeiro on 19/05/2017.
//  Copyright © 2017 Pedro Cordeiro. All rights reserved.
//

#ifndef timer_hpp
#define timer_hpp

#include <SDL2/SDL.h>

class Timer
{
public:
    //Initializes variables
    Timer();
    
    void Start();
    void Pause();
    void Unpause();
    Uint32 GetTime();
    
private:
    //The clock time when the timer started
    Uint32 start_time;
    //The clock time when the timer paused
    Uint32 paused_time;
};

#endif /* timer_hpp */
