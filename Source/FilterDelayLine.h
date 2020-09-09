/*
  ==============================================================================

    FilterDelayLine.h
    Created: 6 Sep 2020 1:23:44pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FilterDelayLine
{
public:
    
    FilterDelayLine();
    FilterDelayLine(int);
    
    void write(float);
    float read(int);
    void advance();
    
    int getSize();

    
private:
    int w_pos;
    Array<int> r_pos;
    Array<float> data;
    int size;
    
    void makeReadPositions();
    void advanceReadPositions();
    
    void makeData();
};
