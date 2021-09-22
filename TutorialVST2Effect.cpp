// This file is part of VSTGUI. It is subject to the license terms 
// in the LICENSE file found in the top-level directory of this
// distribution and at http://github.com/steinbergmedia/vstgui/LICENSE

#include "TutorialVST2Effect.h"
#include "../../plugin-bindings/aeffguieditor.h"

#include "DataView.h"

//------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new VSTGUI::TutorialVST2Effect (audioMaster);
}

namespace VSTGUI {
    
//------------------------------------------------------------------------------------
TutorialVST2Effect::TutorialVST2Effect (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParameters)
{
	setUniqueID (CCONST('G', 'U', 'I', '0'));
	setNumInputs (2);
	setNumOutputs (2);

	parameters[kLChannelParameter] = 1.0f;
	parameters[kRChannelParameter] = 1.0f;

	parameters[kHScaleParameter] = 1.0f;
	parameters[kVScaleParameter] = 1.0f;
	parameters[kVShiftParameter] = 0.5f;

	parameters[kHScaleTextParameter] = 1.0f;
	parameters[kVScaleTextParameter] = 1.0f;
	parameters[kVShiftTextParameter] = 0.5f;

	//count = 0;
	tutorialeditor = nullptr;

    //extern VSTGUI::AEffGUIEditor* createEditor (AudioEffectX*);
	extern VSTGUI::TutorialEditor* createEditor (AudioEffectX*);

	//setEditor (createEditor (this));
	tutorialeditor = createEditor (this);
	setEditor ((AEffEditor *)tutorialeditor);
}

//------------------------------------------------------------------------------------
void TutorialVST2Effect::setParameter (VstInt32 index, float value)
{
	//std::string s = tutorialeditor->console->getText ();
	//tutorialeditor->console->setText (s + " | backend setParameter(" + std::to_string(index) + ", " + std::to_string(value) + ")");
	if (index < kNumParameters)
	{
		parameters[index] = value;
		if (editor)
            ((VSTGUI::AEffGUIEditor*)editor)->setParameter (index, value);
	}
}

//------------------------------------------------------------------------------------
float TutorialVST2Effect::getParameter (VstInt32 index)
{
	if (index < kNumParameters)
		return parameters[index];
	return 0.f;
}

void TutorialVST2Effect::getParameterName(VstInt32 index, char *label)
{
	switch (index)
	{
/*		case kLeftVolumeParameter:
			strcpy (label, "Left Volume");
			break;
		case kRightVolumeParameter: 
			strcpy (label, "Right Volume");
			break;
			*/
		case kLChannelParameter:
			strcpy (label, "Left Channel OnOff");
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------------
void TutorialVST2Effect::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	//count++;
	if (tutorialeditor == nullptr || tutorialeditor->dataview == nullptr)
		return;

	if (sampleFrames == 0)
		return;

	//tutorialeditor->dataview->count++;
	//tutorialeditor->dataview->frames = sampleFrames;
	//tutorialeditor->dataview->inputs = inputs;
	//tutorialeditor->dataview->invalid ();	

	tutorialeditor->dataview->addData (inputs, sampleFrames);
	//tutorialeditor->dataview->invalid ();

	for (VstInt32 i = 0; i < sampleFrames; i++)
	{
		outputs[0][i] = inputs[0][i];
		outputs[1][i] = inputs[1][i];
	}
}

} // VSTGUI
