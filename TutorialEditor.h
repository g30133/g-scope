// This file is part of VSTGUI. It is subject to the license terms 
// in the LICENSE file found in the top-level directory of this
// distribution and at http://github.com/steinbergmedia/vstgui/LICENSE

#ifndef __TutorialEditor__
#define __TutorialEditor__

#include "../../plugin-bindings/aeffguieditor.h"
#include "TutorialParameters.h"
#include "DataView.h"
#include "Console.h"

namespace VSTGUI {

class TutorialEditor : public AEffGUIEditor, public IControlListener
{
public:
	TutorialEditor (void*);
	
	// from AEffGUIEditor
	bool open (void* ptr);
	void close ();
	void setParameter (VstInt32 index, float value);

	// from CControlListener
	void valueChanged (CControl* pControl);
	//CWaveFormView* getWaveFormView ();

	DataView* dataview;
	Console* console;

protected:
	CControl* controls[kNumParameters];
};
    
} // VSTGUI

#endif // __TutorialEditor__

