//
//  delayLine.cpp
//  delay_one
//
//  Created by Oberon Day-West on 21/07/2023.
//

#include <array>
#include "circularBuffer.h"

class DelayLine
{
public:
    DelayLine() { buffer.fill(0.f); readIndex.fill(0); taps.fill(0.0f); }
    
    void setDelayTime( int tapIndex, float delayTimeSeconds, float sampleRate )
    {
        int delaySamples = static_cast<int>( delayTimeSeconds * sampleRate );
        readIndex[tapIndex] = ( writeIndex - delaySamples + buffer.size() ) % buffer.size();
    }
    
    void setTapMix( int tapIndex, float mixLevel )
    {
        taps[tapIndex] = mixLevel;
    }
    
    float processBlock( float input )
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
    
private:
    std::array<float, 44100> buffer;
    int writeIndex = 0;
    std::array<int, 4> readIndex;
    std::array<float, 4> taps;
};
