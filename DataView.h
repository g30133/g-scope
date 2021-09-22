#ifndef __DataView__
#define __DataView__

#include "../../vstgui.h"
#include "TutorialParameters.h"
#include "Console.h"
//#include "TutorialEditor.h"
//#include <pluginterfaces\vst2.x\aeffect.h>
//#include "./TutorialEditor.h"

namespace VSTGUI {
const int SAMPLING_RATE = 44100;
const int DATAVIEW_BUFLEN = SAMPLING_RATE * 10;

class DataView : public CView
{
public:
	DataView (const CRect& size);
	DataView (const CRect& size, Console* console);
	virtual void draw (CDrawContext* pContext);
	void addData (float** inputs, int sampleFrames);

	unsigned int count;
	float hScale;
	float vScale;
	float vShift;
	float lChannel;
	float rChannel;

	Console* console;


private:
	void drawGrid (CDrawContext* pContext);
	void drawTimeMark (CDrawContext* pContext);
	void drawValueMark (CDrawContext* pContext);
	void drawDebug (CDrawContext* pContext);
	CRect loc;
	float bufL[DATAVIEW_BUFLEN];
	float bufR[DATAVIEW_BUFLEN];
	int bufLHead;
	int bufLTail;
	int bufRHead;
	int bufRTail;
};

} // VSTGUI

#endif // __DataView__