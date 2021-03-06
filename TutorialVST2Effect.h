// This file is part of VSTGUI. It is subject to the license terms 
// in the LICENSE file found in the top-level directory of this
// distribution and at http://github.com/steinbergmedia/vstgui/LICENSE

#ifndef __TutorialVST2Effect__
#define __TutorialVST2Effect__

#include "public.sdk/source/vst2.x/audioeffectx.h"

#include "TutorialParameters.h"
#include "TutorialEditor.h"

namespace VSTGUI {

class TutorialVST2Effect : public AudioEffectX
{
public:
	TutorialVST2Effect (audioMasterCallback audioMaster);

	void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

	void setParameter (VstInt32 index, float value);
	float getParameter (VstInt32 index);
	void getParameterName (VstInt32 index, char* label);

protected:
	float parameters[kNumParameters];
	TutorialEditor* tutorialeditor;
};

} // VSTGUI

#endif // __TutorialVST2Effect__
