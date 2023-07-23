//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#pragma once
#include <vector>

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "delayLine.h"

namespace delay_sfx {

//------------------------------------------------------------------------
//  delay_oneProcessor
//------------------------------------------------------------------------
class delay_oneProcessor : public Steinberg::Vst::AudioEffect
{
public:
    delay_oneProcessor ();
    ~delay_oneProcessor () SMTG_OVERRIDE;
    
    // Create function
    static Steinberg::FUnknown* createInstance (void* /*context*/)
    {
        return (Steinberg::Vst::IAudioProcessor*)new delay_oneProcessor;
    }
    
    //--- ---------------------------------------------------------------------
    // AudioEffect overrides:
    //--- ---------------------------------------------------------------------
    /** Called at first after constructor */
    Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
    
    /** Called at the end before destructor */
    Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
    
    /** Switch the Plug-in on/off */
    Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;
    
    /** Will be called before any process call */
    Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
    
    /** Asks if a given sample size is supported see SymbolicSampleSizes. */
    Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;
    
    /** Here we go...the process call */
    Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
    
    /** For persistence */
    Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;
    
    double getSampleRate() const
    {
        return sampleRate;
    }
    
    //------------------------------------------------------------------------
protected:
    Steinberg::Vst::ParamValue mGain = 1.0;
    Steinberg::Vst::ParamValue mDelay1 = 1.0;
    Steinberg::Vst::ParamValue mDelay2 = 1.0;
    Steinberg::Vst::ParamValue mDelay3 = 1.0;
    Steinberg::Vst::ParamValue mDelay4 = 1.0;

    Steinberg::Vst::ParamValue mTapGain1 = 1.0;
    Steinberg::Vst::ParamValue mTapGain2 = 1.0;
    Steinberg::Vst::ParamValue mTapGain3 = 1.0;
    Steinberg::Vst::ParamValue mTapGain4 = 1.0;

    Steinberg::Vst::ParamValue mFeedbackGain1 = 1.0;
    Steinberg::Vst::ParamValue mFeedbackGain2 = 1.0;
    Steinberg::Vst::ParamValue mFeedbackGain3 = 1.0;
    Steinberg::Vst::ParamValue mFeedbackGain4 = 1.0;

    
    std::vector<float> delayBuffer;
    int writePos;
    int readPos;
    float tapDelay[ 4 ];
    float tapGain[ 4 ];
    float feedbackGain[ 4 ];
    
private:
    double sampleRate;
    DelayLine delayLine;
    
    double denormalisedMasterGain;
    
    double denormalisedDelay1;
    double denormalisedDelay2;
    double denormalisedDelay3;
    double denormalisedDelay4;
    
    double denormalisedTapGain1;
    double denormalisedTapGain2;
    double denormalisedTapGain3;
    double denormalisedTapGain4;
    
    double denormalisedFeedbackGain1;
    double denormalisedFeedbackGain2;
    double denormalisedFeedbackGain3;
    double denormalisedFeedbackGain4;

};

//------------------------------------------------------------------------
} // namespace delay_sfx
