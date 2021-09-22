#ifndef __PARAMEDIT__
#define __PARAMEDIT__

#include "../../vstgui.h"
#include "Console.h"
#include "TutorialParameters.h"

namespace VSTGUI {

class ParamEdit : public CTextEdit
{
public:
	ParamEdit (const CRect& size, IControlListener* listener, int32_t tag);
	ParamEdit (const CRect& size, IControlListener* listener, int32_t tag, Console* console);
	void controlChanged (int32_t tag, float value);
	virtual void draw (CDrawContext* ctx);
	virtual void valueChanged ();

private:
	Console* console;
};

} // VSTGUI

#endif // __PARAMEDIT__
