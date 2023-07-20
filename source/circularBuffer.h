//
//  circularBuffer.h
//  delay_one
//
//  Created by Oberon Day-West on 20/07/2023.
//

#ifndef circularBuffer_h
#define circularBuffer_h

#include <algorithm>
#include <vector>
#include <iostream>

class CircularBuffer
{
public:
    CircularBuffer( int delay, int size ) : delay( delay ), buffer( size, 0.0f ), index( 0 ) {}
    
    float processBuffer( float input );
    
private:
    std::vector< float > buffer;
    int index;
    int delay;
};


#endif /* circularBuffer_h */
