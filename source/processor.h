//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#pragma once
#include <vector>

#include "public.sdk/source/vst/vstaudioeffect.h"

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
    
//------------------------------------------------------------------------
protected:
    const int maximumDelayTime = 2; // seconds
    int sampleRate; // Hz, to be initialized in setupProcessing method

    std::vector<float> delayBuffer;

    int writePos;
    int readPos;

    float tapDelay[ 4 ];
    float tapGain[ 4 ];
    float feedbackGain[ 4 ];
};

//------------------------------------------------------------------------
} // namespace delay_sfx
