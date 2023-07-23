//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#include "controller.h"
#include "cids.h"

/*
 WANT
 
 - parameter for controlling the first delay time (seconds)
 - parameter for controllering the other three delay times (Milliseconds)
 - dry/wet control for each tap
 - gain for each tap
 - feedback knob for each tap
 - overall gain knob
 
 */


using namespace Steinberg;

namespace delay_sfx {

//------------------------------------------------------------------------
// delay_oneController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// Here you could register some parameters
    parameters.addParameter( STR16( "Gain" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamGainId, 0 );
    
    parameters.addParameter( STR16( "Delay 1" ), STR16( "sec" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamDelayTime1Id, 0 );
    
    parameters.addParameter( STR16( "Delay 2" ), STR16( "sec" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamDelayTime2Id, 0 );
    
    parameters.addParameter( STR16( "Delay 3" ), STR16( "sec" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamDelayTime3Id, 0 );
    
    parameters.addParameter( STR16( "Delay 4" ), STR16( "sec" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamDelayTime4Id, 0 );
    
    parameters.addParameter( STR16( "Tap 1 Gain" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamTapGain1Id, 0 );
    
    parameters.addParameter( STR16( "Tap 2 Gain" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamTapGain2Id, 0 );
    
    parameters.addParameter( STR16( "Tap 3 Gain" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamTapGain3Id, 0 );
    
    parameters.addParameter( STR16( "Tap 4 Gain" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamTapGain4Id, 0 );
    
    parameters.addParameter( STR16( "Feedback Gain 1" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamFeedbackGain1Id, 0 );
    
    parameters.addParameter( STR16( "Feedback Gain 2" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamFeedbackGain2Id, 0 );
    
    parameters.addParameter( STR16( "Feedback Gain 3" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamFeedbackGain3Id, 0 );
    
    parameters.addParameter( STR16( "Feedback Gain 4" ), STR16( "dB" ), 0, 0.5, Vst::ParameterInfo::kCanAutomate, AudioParams::kParamFeedbackGain4Id, 0 );

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneController::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneController::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API delay_oneController::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
        return nullptr;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneController::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	// called by host to update your parameters
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneController::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	// called by host to get a string for given normalized value of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay_oneController::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	// called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
} // namespace delay_sfx
