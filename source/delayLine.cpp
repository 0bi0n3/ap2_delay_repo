//
//  delayLine.cpp
//  delay_one
//
//  Created by Oberon Day-West on 21/07/2023.
//

#include <array>
#include "circularBuffer.h"
#include "delayLine.h"

DelayLine::DelayLine( float sampleRate, float maxDelayTime)
{
    buffer.resize( static_cast<int>( sampleRate * maxDelayTime ), 0.0f);
    writeIndex = 0;
    readIndex.fill( 0 );
    taps.fill( 0.0f );
}

void DelayLine::setDelayTime( int tapIndex, float delayTimeSeconds, float sampleRate )
{
    int delaySamples = static_cast<int>( delayTimeSeconds * sampleRate );
    readIndex[tapIndex] = ( writeIndex - delaySamples + buffer.size() ) % buffer.size();
}
    
void DelayLine::setTapMix( int tapIndex, float mixLevel )
{
    taps[tapIndex] = mixLevel;
}

float DelayLine::processBlock( float input )
{
    buffer[writeIndex] = input;
    writeIndex = ( writeIndex + 1 ) % buffer.size();
    
    float output = 0.0f;
    for( int i = 0; i < 4; i++)
    {
        output += buffer[readIndex[i]] * taps[i];
        readIndex[i] = ( readIndex[i] + 1 ) % buffer.size();
    }
    return output;
}

