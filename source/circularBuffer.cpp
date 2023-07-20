//
//  circularBuffer.cpp
//  delay_one
//
//  Created by Oberon Day-West on 20/07/2023.
//

#include <stdio.h>
#include "circularBuffer.h"

float CircularBuffer::processBuffer( float input )
{
    index = (index + buffer.size()) % buffer.size();

    int lengthOfBuffer = buffer.size();
    int currentIndexPos = index % lengthOfBuffer;
    int delayedIndexPos = ( index - delay + lengthOfBuffer ) % lengthOfBuffer;
    
    float output = buffer[delayedIndexPos];
    
    buffer[currentIndexPos] = input;
    
    index = ( index + 1 ) % lengthOfBuffer;
    
    return output;
}
