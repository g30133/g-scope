#include "ParamEdit.h"

namespace VSTGUI {

ParamEdit::ParamEdit (const VSTGUI::CRect& size, IControlListener* listener, int32_t tag) : CTextEdit (size, listener, tag)
{
	console = nullptr;
}

ParamEdit::ParamEdit (const VSTGUI::CRect& size, IControlListener* listener, int32_t tag, Console* c) : ParamEdit (size, listener, tag)
{
	console = c;
}

void ParamEdit::draw (CDrawContext* ctx)
{
	//console->print ("draw() " + std::to_string(getValue ()));
	ctx->setLineWidth (1);
	ctx->setFillColor (CColor (0, 0, 0, 225));
	ctx->setFrameColor (CColor (225, 225, 225, 225));

	ctx->drawRect (getViewSize(), kDrawFilledAndStroked);
	
	ctx->setFontColor (CColor (225, 225, 225, 225));
	float val = 0;

	int32_t mytag = getTag ();
	char buffer[50];
	int n = 0;

	switch (mytag)
	{
		case kHScaleTextParameter:
			val = 1 + 99 * getValue ();
			n = sprintf (buffer, "%1.2f", val);
			setDirty (true);
			break;
		case kVScaleTextParameter:
			val = 0.0025 + (0.25 - 0.0025) * getValue ();
			n = sprintf (buffer, "%1.4f", val);
			setDirty (true);
			break;
		case kVShiftTextParameter:
			val = 2 * getValue () - 1;
			n = sprintf (buffer, "%1.1f", val);
			setDirty (true);
			break;
		default:
			break;
	}

	UTF8String s = UTF8String (buffer); 
	ctx->drawString (s, getViewSize());
}

void ParamEdit::valueChanged() {
	console->print ("ParamEdit::valueChanged() " + std::to_string(getValue()));
}

void ParamEdit::controlChanged(int32_t tag, float value) {
	int32_t mytag = getTag ();
	switch (mytag)
	{
		case kHScaleTextParameter:
			if (tag == kHScaleParameter)
			{
				setValue (value);
				setDirty (true);
			}
			break;
		case kVScaleTextParameter:
			if (tag == kVScaleParameter)
			{
				setValue (value);
				setDirty (true);
			}
			break;
		case kVShiftTextParameter:
			if (tag == kVShiftParameter)
			{
				setValue (value);
				setDirty (true);
			}
			break;
	}
}

} // VSTGUI