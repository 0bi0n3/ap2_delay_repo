//
//  delayLine.h
//  ALL_BUILD
//
//  Created by Oberon Day-West on 21/07/2023.
//

#ifndef delayLine_h
#define delayLine_h

#include <array>
#include <vector>

class DelayLine
{
public:
    DelayLine(float sampleRate, float maxDelayTime);

    float getSampleRate( float sampleRate );
    void setDelayTime(int tapIndex, float delayTimeSeconds);
    void setTapGain(int tapIndex, float gain);
    void setTapMix(int tapIndex, float mixLevel);
    void setFeedbackGain(int tapIndex, float feedbackGain);
    float getInterpolatedSample(float delaySamples);
    void processBlock(const std::vector<float>& input, std::vector<float>& output);

private:
    const float MAX_FEEDBACK_GAIN = 0.5f;

    float sampleRate;
    int writeIndex;
    std::array<float, 4> delayTimes;
    std::array<float, 4> taps;
    std::array<float, 4> tapMix;
    std::array<float, 4> feedbackGains;
    std::array<float, 4> feedbacks;
    std::vector<float> buffer;
};

#endif /* delayLine_h */
