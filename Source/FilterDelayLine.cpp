/*
  ==============================================================================

    FilterDelayLine.cpp
    Created: 6 Sep 2020 1:23:44pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#include "FilterDelayLine.h"

FilterDelayLine::FilterDelayLine()
{
    size = 3;
    w_pos = size - 1;
    makeReadPositions();
    makeData();
}

FilterDelayLine::FilterDelayLine(int len)
{
    size = len;
    w_pos = size - 1;
    makeReadPositions();
    makeData();
}

void FilterDelayLine::write(float value)
{
    // WRITE(FLOAT)
    //
    //  Write a value to the delay line.
    
    data.set(w_pos, value);
}

float FilterDelayLine::read(int position)
{
    // READ(INT)
    //
    //  Read written to the delay line. This is the function most
    //  useful for constructing the difference equation in the parent
    //  processor.
    
    return data[r_pos[position]];
}

void FilterDelayLine::advance()
{
    // ADVANCE()
    //
    //  Update read and write positions.
    
    w_pos = (w_pos + 1) % size;
    advanceReadPositions();
}

int FilterDelayLine::getSize()
{
    return size;
}

void FilterDelayLine::makeReadPositions()
{
    // MAKEREADPOSITIONS()
    //
    //  Initialize read positions (r_pos) as array of length size.
    
    // init
    r_pos = Array<int>();
    
    for (int i = 0; i < size; i++)
    {
        r_pos.add(w_pos - i);
    }
}

void FilterDelayLine::advanceReadPositions()
{
    // ADVANCEREADPOSITIONS()
    //
    //  Update read positions (r_pos) array by incrementing and
    //  taking modulus size.
    
    for (int i = 0; i < size; i++)
    {
        r_pos.set(i, (r_pos[i] + 1) % size);
    }
}

void FilterDelayLine::makeData()
{
    // MAKEDATA()
    //
    //  Initialize empty array as delay line.
    
    // init
    data = Array<float>();
    
    for (int i = 0; i < size; i++)
    {
        data.add(0.0f);
    }
}
