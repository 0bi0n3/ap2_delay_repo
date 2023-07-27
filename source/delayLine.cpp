//
//  delayLine.cpp
//  delay_one
//
//  Created by Oberon Day-West on 21/07/2023.
//
#include <iostream>
#include <array>
#include "delayLine.h"
#include <numeric>
#include <algorithm>

DelayLine::DelayLine( float sampleRate, float maxDelayTime) : sampleRate(sampleRate)
{
    buffer.resize( static_cast<int>( sampleRate * maxDelayTime ), 0.0f);
    writeIndex = 0;
    
    // Initialize tap gain
    taps[0] = 0.5f;
    taps[1] = 0.5f;
    taps[2] = 0.5f;
    taps[3] = 0.5f;
    
    // initialise tap mix
    tapMix[0] = 0.5f;
    tapMix[1] = 0.5f;
    tapMix[2] = 0.5f;
    tapMix[3] = 0.5f;
        
    // initialise feedback gain
    feedbackGains[0] = 0.2f;
    feedbackGains[1] = 0.0f;
    feedbackGains[2] = 0.0f;
    feedbackGains[3] = 0.0f;
    
    // initialise feedback
    feedbacks[0] = 0.1f;
    feedbacks[1] = 0.1f;
    feedbacks[2] = 0.1f;
    feedbacks[3] = 0.1f;
    
    // Initialize delayTimes with different values
    delayTimes[0] = 0.5f;
    delayTimes[1] = 0.4f;
    delayTimes[2] = 0.3f;
    delayTimes[3] = 0.2f;

    std::cout << "Sample rate: " << sampleRate << "\n";
}

void DelayLine::setDelayTime(int tapIndex, float delayTimeSeconds)
{
    if( tapIndex >= 0 && tapIndex < delayTimes.size() )
    {
        delayTimes[tapIndex] = delayTimeSeconds;
        std::cout << "Set delay time for tap " << tapIndex << ": " << delayTimeSeconds << " seconds\n";
    }
    else
    {
        std::cout << "Error: Invalid tap index. Please use a value between 0 and " << delayTimes.size()-1 << ".\n";
    }
}

void DelayLine::setTapGain(int tapIndex, float gain)
{
    if( tapIndex >= 0 && tapIndex < taps.size() )
    {
        taps[tapIndex] = gain;
    }
    else
    {
        std::cout << "Error: Invalid tap index. Please use a value between 0 and " << taps.size()-1 << ".\n";
    }

    float totalTapLevel = std::accumulate(taps.begin(), taps.end(), 0.0f);
    if(totalTapLevel == 0.0f)
    {
        std::cout << "Error: totalTapLevel is zero. Please set at least one tap gain greater than zero.\n";
    }
}

void DelayLine::setTapMix( int tapIndex, float mixLevel )
{
    if( tapIndex >= 0 && tapIndex < tapMix.size() )
    {
        tapMix[tapIndex] = mixLevel;
    }
    else
    {
        std::cout << "Error: Invalid tap index. Please use a value between 0 and " << tapMix.size()-1 << ".\n";
    }

    float totalTapMix = std::accumulate(tapMix.begin(), tapMix.end(), 0.0f);
    if(totalTapMix == 0.0f)
    {
        std::cout << "Error: totalTapMix is zero. Please set at least one tap mix level greater than zero.\n";
    }
}

void DelayLine::setFeedbackGain(int tapIndex, float feedbackGain)
{
    if (feedbackGain > MAX_FEEDBACK_GAIN)
    {
        feedbackGain = MAX_FEEDBACK_GAIN;
    }
    else if (feedbackGain < -MAX_FEEDBACK_GAIN)
    {
        feedbackGain = -MAX_FEEDBACK_GAIN;
    }

    if( tapIndex >= 0 && tapIndex < feedbackGains.size() )
    {
        feedbackGains[tapIndex] = feedbackGain;
    }
    else
    {
        std::cout << "Error: Invalid tap index. Please use a value between 0 and " << feedbackGains.size()-1 << ".\n";
    }
}

void DelayLine::processBlock(const std::vector<float>& input, std::vector<float>& output)
{
    // Ensure the output buffer has the same size as the input
    output.resize(input.size());

    // Process each sample in the block
    for (size_t i = 0; i < input.size(); ++i)
    {
        float currentSample = input[i];
        float outputSample = 0.0f;
        float totalTapLevel = 0.0f;

        for (int j = 0; j < 4; ++j)
        {
            // Calculate read index based on current delay time and write index
            long delaySamples = static_cast<int>( delayTimes[j] * sampleRate );
            long readIndex = (writeIndex - delaySamples + buffer.size()) % buffer.size();

            feedbacks[j] = buffer[readIndex] * feedbackGains[j] * 0.1;
            outputSample += buffer[readIndex] * tapMix[j];
            totalTapLevel += taps[j];
        }

        // Normalise output by totalTapLevel
        if (totalTapLevel > 0.0f)
        {
            outputSample /= totalTapLevel;
        }

        outputSample = std::max(-1.0f, std::min(1.0f, outputSample));

        // Write input to buffer, scaled by feedbacks
        float sumFeedbacks = std::accumulate(feedbacks.begin(), feedbacks.end(), 0.0f);
        buffer[writeIndex] = std::max(-1.0f, std::min(1.0f, currentSample + sumFeedbacks));

        // Increment the write index, wrapping around if necessary
        writeIndex = (writeIndex + 1) % buffer.size();

        // Assign the output sample to the output buffer
        output[i] = outputSample;
    }
}
