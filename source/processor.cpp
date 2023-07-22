//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#include "processor.h"
#include "cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

/*
 TO DO
 - parse audio into the process block
 - parse audio a buffer
 - parse the buffer into a function which causes a delay
 
 */

using namespace Steinberg;

namespace delay_sfx {
//------------------------------------------------------------------------
// delay_oneProcessor
//------------------------------------------------------------------------
delay_oneProcessor::delay_oneProcessor ()
: delayLine(sampleRate, 1.0f)
{
	//--- set the wanted controller for our processor
	setControllerClass (kdelay_oneControllerUID);
    
    delayLine.setDelayTime(0, 0.5f, sampleRate);
    delayLine.setTapMix(0, 0.5f);
}

//------------------------------------------------------------------------
delay_oneProcessor::~delay_oneProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------
    if (data.inputParameterChanges)
    {
        // for each parameter defined by its ID
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            // for this parameter we could iterate the list of value changes (could 1 per audio block or more!)
            // in this example we get only the last value (getPointCount - 1)
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                switch (paramQueue->getParameterId ())
                {
                    case GainParams::kParamGainId:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mGain = value;
                        }
                    break;
				}
			}
		}
	}
	//--- Here you have to implement your processing

    //-- Flush case: we only need to update parameter, noprocessing possible
    if (data.numInputs == 0 || data.numSamples == 0)
        return kResultOk;

    //--- Here you have to implement your processing
    int32 numChannels = data.inputs[0].numChannels;

    //---get audio buffers using helper-functions(vstaudioprocessoralgo.h)-------------
    uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, data.numSamples);
    void** in = getChannelBuffersPointer (processSetup, data.inputs[0]);
    void** out = getChannelBuffersPointer (processSetup, data.outputs[0]);

    // Here could check the silent flags
    // now we will produce the output
    // mark our outputs has not silent
    data.outputs[0].silenceFlags = 0;

    float gain = mGain;
    // for each channel (left and right)
    for (int32 i = 0; i < numChannels; i++)
    {
        int32 samples = data.numSamples;
        Vst::Sample32* ptrIn = (Vst::Sample32*)in[i];
        Vst::Sample32* ptrOut = (Vst::Sample32*)out[i];
        Vst::Sample32 tmp;
        
        // for each sample in this channel
        while (--samples >= 0)
        {
            // process delay
           float input = static_cast<float>(*ptrIn++);
           float output = gain * delayLine.processBlock(input);
           (*ptrOut++) = output;
            // apply gain
//            tmp = (*ptrIn++) * gain;
//            (*ptrOut++) = tmp;
    
        }
    }
    // Here could check the silent flags
    //---check if silence---------------
    // normally we have to check each channel (simplification)
    if (data.inputs[0].silenceFlags != 0)
    {
        // mark output silence too
        data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
        
        // the plug-in has to be sure that if it sets the flags silence that the output buffer are clear
        for (int32 i = 0; i < numChannels; i++)
        {
            // do not need to be cleared if the buffers are the same (in this case input buffer are
            // already cleared by the host)
            if (in[i] != out[i])
            {
                memset (out[i], 0, sampleFramesSize);
            }
        }
    }

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
    
    sampleRate = newSetup.sampleRate;
    
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace delay_sfx
