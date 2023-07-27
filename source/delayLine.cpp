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
    taps[0] = 1.0f;
    taps[1] = 1.0f;
    taps[2] = 1.0f;
    taps[3] = 1.0f;
    
    // initialise tap mix
    tapMix[0] = 1.0f;
    tapMix[1] = 1.0f;
    tapMix[2] = 1.0f;
    tapMix[3] = 1.0f;
        
    // initialise feedback gain
    feedbackGains[0] = 0.02f;
    feedbackGains[1] = 0.0f;
    feedbackGains[2] = 0.0f;
    feedbackGains[3] = 0.0f;
    
    // initialise feedback
    feedbacks[0] = 0.0f;
    feedbacks[1] = 0.0f;
    feedbacks[2] = 0.0f;
    feedbacks[3] = 0.0f;
    
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

void DelayLine::setTapGain(int tapIndex, float gain_db)
{
    if( tapIndex >= 0 && tapIndex < taps.size() )
    {
        // Convert the gain value from dB to linear scale
        float gain = pow(10.0, gain_db / 20.0);
        
        // Ensure gain is in [0,1] range to prevent distortion
        gain = std::max(0.0f, std::min(1.0f, gain));
        taps[tapIndex] = gain;
    }
    else
    {
        std::cout << "Error: Invalid tap index. Please use a value between 0 and " << taps.size()-1 << ".\n";
    }

    float totalTapLevel = std::accumulate(taps.begin(), taps.end(), 0.0f);
    if(totalTapLevel == 0.0f)
    {
        totalTapLevel = 0.1f;
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
//    if (feedbackGain > MAX_FEEDBACK_GAIN)
//    {
//        feedbackGain = MAX_FEEDBACK_GAIN;
//    }
//    else if (feedbackGain < -MAX_FEEDBACK_GAIN)
//    {
//        feedbackGain = -MAX_FEEDBACK_GAIN;
//    }

    if( tapIndex >= 0 && tapIndex < feedbackGains.size() )
    {
        // Convert the gain value from dB to linear scale
        float gain = pow(10.0, feedbackGain / 20.0);
        
        // Ensure gain is in [0,1] range to prevent distortion
        gain = std::max(0.0f, std::min(1.0f, gain));
        feedbackGains[tapIndex] = gain;
    }
    else
    {
        std::cout << "Error: Invalid tap index. Please use a value between 0 and " << feedbackGains.size()-1 << ".\n";
    }
}

float DelayLine::getInterpolatedSample(float delaySamples)
{
    long delaySamplesInt = static_cast<long>(delaySamples);
    float delaySamplesFrac = delaySamples - delaySamplesInt;

    long readIndex1 = (writeIndex - delaySamplesInt + buffer.size()) % buffer.size();
    long readIndex2 = (readIndex1 + 1) % buffer.size();

    return (1.0f - delaySamplesFrac) * buffer[readIndex1] + delaySamplesFrac * buffer[readIndex2];
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
        float totalTapMix = 0.0f;

        for (int j = 0; j < 4; ++j)
        {
            // Calculate read index based on current delay time and write index
            float delaySamples = delayTimes[j] * sampleRate;
            float delayedSample = getInterpolatedSample(delaySamples);
            
            feedbacks[j] = delayedSample * feedbackGains[j];
            
            float tapGainLinear = taps[j];
            float tapMixLinear = tapMix[j];
            
            outputSample += delayedSample * tapGainLinear * tapMixLinear;
            totalTapMix += tapMixLinear;
        }

        // Normalise output by totalTapMix
        if (totalTapMix > 0.0f)
        {
            outputSample /= totalTapMix;
        }
        
        // Ensure outputSample is in [-1, 1] range to prevent distortion
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

