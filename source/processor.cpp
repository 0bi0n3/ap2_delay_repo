//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#include "processor.h"
#include "cids.h"
#include <iostream>

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
: delayLine(48000, 1.0f)
{
	//--- set the wanted controller for our processor
	setControllerClass (kdelay_oneControllerUID);
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
                    case AudioParams::kParamGainId:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mGain = value;
                            denormalisedMasterGain = (mGain * 40.0) - 20.0; // denormalised gain value
                        }
                    break;
                        
                    // Delay time parameters
                        
                    case AudioParams::kParamDelayTime1Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mDelay1 = value;
                            denormalisedDelay1 = (mDelay1 * 2.0);
                        }
                    break;
                        
                    case AudioParams::kParamDelayTime2Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mDelay2 = value;
                            denormalisedDelay2 = (mDelay2 * 2.0);
                        }
                    break;

                    case AudioParams::kParamDelayTime3Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mDelay3 = value;
                            denormalisedDelay3 = (mDelay3 * 2.0);
                        }
                    break;

                    case AudioParams::kParamDelayTime4Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mDelay4 = value;
                            denormalisedDelay4 = (mDelay4 * 2.0);
                        }
                    break;

                    // Tap gain parameters

                    case AudioParams::kParamTapGain1Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mTapGain1 = value;
                            denormalisedTapGain1 = (mTapGain1 * 40.0) - 20.0;
                        }
                    break;
                        
                    case AudioParams::kParamTapGain2Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mTapGain2 = value;
                            denormalisedTapGain2 = (mTapGain2 * 40.0) - 20.0;
                        }
                    break;
                        
                    case AudioParams::kParamTapGain3Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mTapGain3 = value;
                            denormalisedTapGain3 = (mTapGain3 * 40.0) - 20.0;
                        }
                    break;
                        
                    case AudioParams::kParamTapGain4Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mTapGain4 = value;
                            denormalisedTapGain4 = (mTapGain4 * 40.0) - 20.0;
                        }
                    break;

                    // Feedback gain parameters

                    case AudioParams::kParamFeedbackGain1Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mFeedbackGain1 = value;
                            denormalisedFeedbackGain1 = (mFeedbackGain1 * 40.0) - 20.0;
                        }
                    break;
                        
                    case AudioParams::kParamFeedbackGain2Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mFeedbackGain2 = value;
                            denormalisedFeedbackGain2 = (mFeedbackGain2 * 40.0) - 20.0;
                        }
                    break;
                        
                    case AudioParams::kParamFeedbackGain3Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mFeedbackGain3 = value;
                            denormalisedFeedbackGain3 = (mFeedbackGain3 * 40.0) - 20.0;
                        }
                    break;
                        
                    case AudioParams::kParamFeedbackGain4Id:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            mFeedbackGain4 = value;
                            denormalisedFeedbackGain4 = (mFeedbackGain4 * 40.0) - 20.0;
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
    
    // Update delay times and feedback gains
    delayLine.setDelayTime(0, denormalisedDelay1);
    delayLine.setTapMix(0, denormalisedTapGain1);
    delayLine.setFeedbackGain(0, denormalisedFeedbackGain1);

    delayLine.setDelayTime(1, denormalisedDelay2);
    delayLine.setTapMix(1, denormalisedTapGain2);
    delayLine.setFeedbackGain(1, denormalisedFeedbackGain2);

    delayLine.setDelayTime(2, denormalisedDelay3);
    delayLine.setTapMix(2, denormalisedTapGain3);
    delayLine.setFeedbackGain(2, denormalisedFeedbackGain3);

    delayLine.setDelayTime(3, denormalisedDelay4);
    delayLine.setTapMix(3, denormalisedTapGain4);
    delayLine.setFeedbackGain(3, denormalisedFeedbackGain4);

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
        std::vector<float> inputBlock;
        std::vector<float> outputBlock;

        int32 samples = data.numSamples;
        Vst::Sample32* ptrIn = (Vst::Sample32*)in[i];
        Vst::Sample32* ptrOut = (Vst::Sample32*)out[i];

        // Gather all samples into the input block
        while (--samples >= 0)
        {
            float input = static_cast<float>(*ptrIn++);
            inputBlock.push_back(input);
        }

        // Process the entire block
        delayLine.processBlock(inputBlock, outputBlock);
        
        // Test line for audio pass through
//        outputBlock = inputBlock;
        float avg = 0.0f;
        for (auto val : inputBlock)
            avg += std::abs(val);
        avg /= inputBlock.size();
        std::cout << "Average input level: " << avg << "\n";
        
        float avgOut = 0.0f;
        for (auto val : outputBlock)
            avgOut += std::abs(val);
        avgOut /= outputBlock.size();
        std::cout << "Average output level: " << avgOut << "\n";

        // Write the processed samples to the output
        for (float output : outputBlock)
        {
            (*ptrOut++) = output * gain;
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
    
    // Reconfigure delayLine with the actual sampleRate
       delayLine.setDelayTime(0, 0.5f);
    
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
