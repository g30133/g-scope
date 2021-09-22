#ifndef __CONSOLE__
#define __CONSOLE__

#include "../../vstgui.h"

namespace VSTGUI {

class Console : public CMultiLineTextLabel
{
public:
	Console (const CRect& size);
	void print (std::string s);
};

} // VSTGUI

#endif // __CONSOLE__
