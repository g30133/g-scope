#include "Console.h"

namespace VSTGUI {

Console::Console (const VSTGUI::CRect& size) : CMultiLineTextLabel (size)
{
	setTextTruncateMode (CTextLabel::kTruncateHead);
	setLineLayout (LineLayout::wrap);
	setAutoHeight (false);
	setHoriAlign (CHoriTxtAlign::kLeftText);
	setText ("Console");
}

void Console::print (std::string s)
{
	std::string oldText = getText ();
	//setText (oldText + "\n" + s);
	setText (oldText + " | " + s);
}

} // VSTGUI