//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  sprite.cc
//
//  Sprite classes:
//   - sprite:      base class for the guys and enemies in zelda.cc
//   - movingblock: the moving block class
//   - sprite_list: main container class for different groups of sprites
//   - item:        items class
//
//--------------------------------------------------------

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "precompiled.h" //always first

#include "sprite.h"

int fadeclk=-1;
int frame=8;
bool BSZ=false;
int conveyclk=0;
bool freeze_guys=false;

#define SLMAX 255

void sprite::check_conveyor()
{
    return;
}

void movingblock::push(fix bx,fix by,int d2,int f)
{
    //these are here to bypass compiler warnings about unused arguments
    bx=bx;
    by=by;
    d2=d2;
    f=f;
    
    return;
}

bool movingblock::animate(int index)
{
    //these are here to bypass compiler warnings about unused arguments
    index=index;
    
    return false;
}

