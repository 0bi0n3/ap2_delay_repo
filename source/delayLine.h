//
//  delayLine.h
//  ALL_BUILD
//
//  Created by Oberon Day-West on 21/07/2023.
//

#ifndef delayLine_h
#define delayLine_h

#include <array>

class DelayLine
{
public:
    DelayLine();
    void setDelayTime( int tapIndex, float delayTimeSeconds, float sampleRate );
    void setTapMix(int tapIndex, float mixLevel );
    float processBlock( float input );
    
private:
    std::array<float, 44100> buffer;
    int writeIndex;
    std::array<int, 4> readIndex;
    std::array<float, 4> taps;
};


#endif /* delayLine_h */
