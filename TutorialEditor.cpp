// This file is part of VSTGUI. It is subject to the license terms 
// in the LICENSE file found in the top-level directory of this
// distribution and at http://github.com/steinbergmedia/vstgui/LICENSE

#include "TutorialEditor.h"

//VSTGUI::AEffGUIEditor* createEditor (AudioEffectX* effect)
//{
//	return new VSTGUI::TutorialEditor (effect);
//}

VSTGUI::TutorialEditor* createEditor (AudioEffectX* effect)
{
	return new VSTGUI::TutorialEditor (effect);
}

namespace VSTGUI {


//------------------------------------------------------------------------------------
//AEffGUIEditor* createEditor (AudioEffectX* effect)
//{
//	return new TutorialEditor (effect);
//}

//------------------------------------------------------------------------------------
TutorialEditor::TutorialEditor (void* ptr)
: AEffGUIEditor (ptr)
{
	dataview = nullptr;
}

//------------------------------------------------------------------------------------
bool TutorialEditor::open (void* ptr)
{
	//-- first we create the frame with a size of 300, 300 and set the background to white
	//CRect frameSize (0, 0, 900, 600);
	CRect frameSize (0, 0, 900, 700);
	CFrame* newFrame = new CFrame (frameSize, this);
    newFrame->open(ptr);
	newFrame->setBackgroundColor (kGreyCColor);


	CBitmap* button_red_png = new CBitmap ("Red_PC3_LED_Button_64x64_2f.png");
	CBitmap* button_blue_png = new CBitmap ("Blue_PC3_LED_Button_64x64_2f.png");
	CBitmap* knob_png = new CBitmap ("ST_knob_BC_49x49_127f.png");


	CRect titlePos (60, 30, 270, 60);
	CRect lChannelPos (660, 90, 750, 120);
	CRect rChannelPos (780, 90, 870, 120);
	CRect hScalePos (660, 210, 780, 240);
	CRect vScalePos (660, 300, 780, 330);
	CRect vShiftPos (660, 390, 780, 420);
	CRect consolePos (0, 600, 900, 700);

	CTextLabel* title = new CTextLabel (titlePos, "G-Scope");
	newFrame->addView (title);

	CTextLabel* lChannelLabel = new CTextLabel (lChannelPos, "CHL");
	newFrame->addView (lChannelLabel);

	CTextLabel* rChannelLabel = new CTextLabel (rChannelPos, "CHR");
	newFrame->addView (rChannelLabel);

	CTextLabel* hScaleLabel = new CTextLabel (hScalePos, "H-Scale");
	newFrame->addView (hScaleLabel);

	CTextLabel* vScaleLabel = new CTextLabel (vScalePos, "V-Scale");
	newFrame->addView (vScaleLabel);

	CTextLabel* vShiftLabel = new CTextLabel (vShiftPos, "V-Shift");
	newFrame->addView (vShiftLabel);

	//CTextLabel* console = new CTextLabel (consolePos, "console");
	//console->setTextTruncateMode (CTextLabel::kTruncateHead);
	//newFrame->addView (console);

	Console* console = new Console (consolePos);
	newFrame->addView (console);
	this->console = console;

	CRect lChannelButtonPos (660, 120, 724, 184);
	
	COnOffButton* lChannelSwitch = new COnOffButton (lChannelButtonPos, this, kLChannelParameter, button_blue_png);
	lChannelSwitch->setValue (1);
	newFrame->addView (lChannelSwitch);


	CRect rChannelButtonPos (780, 120, 844, 184);
	COnOffButton* rChannelSwitch = new COnOffButton (rChannelButtonPos, this, kRChannelParameter, button_red_png);
	rChannelSwitch->setValue (1);
	newFrame->addView (rChannelSwitch);

	CRect hScaleKnobPos (790, 200, 839, 249);
	CAnimKnob* hScaleKnob = new CAnimKnob (hScaleKnobPos, this, kHScaleParameter, 127, 49, knob_png, CPoint (0, 0));
	newFrame->addView (hScaleKnob);

	CRect hScaleTextPos (790, 260, 839, 280);
	CTextEdit* hScaleText = new CTextEdit (hScaleTextPos, this, kHScaleTextParameter, "hScale");
	hScaleText->setStringToValueFunction (
		[=] (UTF8StringPtr txt, float& result, CTextEdit* te) -> bool {
			//console->print ("CALLBACK!!!");
			float temp = strtof (txt, nullptr);
			result = (temp - 1) / 999.0f;
			controls[kHScaleParameter]->setValue (result);
			controls[kHScaleParameter]->setDirty (true);
			return true;
		});
	hScaleText->setValueToStringFunction (
		[=] (float value, char result[], CParamDisplay* display) -> bool {
			// result = std::to_string (value * 10);
			float temp = 1 + 999 * value;
			int n = sprintf (result, "%4.2f", temp);
			dataview->hScale = temp;
			dataview->setDirty (true);
			return true;
		});
	newFrame->addView (hScaleText);


	CRect vScaleKnobPos (790, 290, 839, 339);
	CAnimKnob* vScaleKnob = new CAnimKnob (vScaleKnobPos, this, kVScaleParameter, 127, 49, knob_png, CPoint (0, 0));
	newFrame->addView (vScaleKnob);


	CRect vScaleTextPos (790, 350, 839, 370);
	CTextEdit* vScaleText = new CTextEdit (vScaleTextPos, this, kVScaleTextParameter, "vScale");
	vScaleText->setStringToValueFunction (
		[=] (UTF8StringPtr txt, float& result, CTextEdit* te) -> bool {
			float temp = strtof (txt, nullptr);
			result = (temp - 0.0025) / (0.25 * 0.0025);
			controls[kVScaleParameter]->setValue (result);
			controls[kVScaleParameter]->setDirty (true);
			dataview->invalid();
			return true;
		});
	vScaleText->setValueToStringFunction (
		[=] (float value, char result[], CParamDisplay* display) -> bool {
			// result = std::to_string (value * 10);
			float temp = 0.0025 + (0.25 - 0.0025) * value;
			int n = sprintf (result, "%1.4f", temp);
			dataview->vScale = temp;
			dataview->setDirty (true);
			return true;
		});
	newFrame->addView (vScaleText);

	CRect vShiftKnobPos (790, 380, 839, 429);
	CAnimKnob* vShiftKnob = new CAnimKnob (vShiftKnobPos, this, kVShiftParameter, 127, 49, knob_png, CPoint (0, 0));
	newFrame->addView (vShiftKnob);

	CRect vShiftTextPos (790, 440, 839, 460);
	CTextEdit* vShiftText = new CTextEdit (vShiftTextPos, this, kVShiftTextParameter, "vShift");
	vShiftText->setStringToValueFunction (
		[=] (UTF8StringPtr txt, float& result, CTextEdit* te) -> bool {
			float temp = strtof (txt, nullptr);
			result = (temp + 1) / 2.0f;
			controls[kVShiftParameter]->setValue (result);
			controls[kVShiftParameter]->setDirty (true);
			return true;
		});
	vShiftText->setValueToStringFunction (
		[=] (float value, char result[], CParamDisplay* display) -> bool {
			// result = std::to_string (value * 10);
			float temp = 2 * value - 1;
			int n = sprintf (result, "%1.1f", temp);
			dataview->vShift = temp;
			dataview->setDirty (true);
			return true;
		});
	
	newFrame->addView (vShiftText);

	CRect dataViewPos (30, 90, 630, 570);
	DataView* dv = new DataView (dataViewPos, console);
	newFrame->addView (dv);

	dataview = dv;

	button_blue_png->forget ();
	button_red_png->forget ();
	knob_png->forget ();


	controls[kLChannelParameter] = lChannelSwitch;
	controls[kRChannelParameter] = rChannelSwitch;
	controls[kHScaleParameter] = hScaleKnob;
	controls[kVScaleParameter] = vScaleKnob;
	controls[kVShiftParameter] = vShiftKnob;

	controls[kHScaleTextParameter] = hScaleText;
	controls[kVScaleTextParameter] = vScaleText;
	controls[kVShiftTextParameter] = vShiftText;


	/*
	//-- load some bitmaps we need
	CBitmap* background = new CBitmap ("KnobBackground.png");
	CBitmap* handle = new CBitmap ("KnobHandle.png");
	CBitmap* png_knob1 = new CBitmap ("ST_knob_BC_49x49_127f.png");
	CBitmap* png_knob2 = new CBitmap ("maschine-edit_mediumlast20_96x96_7f.png");
	
	CBitmap* png_switch1 = new CBitmap ("3-D_Stainless_Switch_two_positions_x4_64x64_2f.png");
	CBitmap* png_switch2 = new CBitmap ("Orange_PC3_LED_Button_64x64_2f.png");



	if (background->getHeight () == 0 || handle->getHeight () == 0 ||
		png_knob1->getHeight () == 0 || png_knob2->getHeight () == 0 ||
		png_switch1->getHeight () == 0 || png_switch2->getHeight () == 0)
	{
		newFrame->setBackgroundColor (kRedCColor);
	}

	//-- creating a knob and adding it to the frame
	CRect r (0, 0, background->getWidth (), background->getHeight ());
	CKnob* knob01 = new CKnob (r, this, kLeftVolumeParameter, background, handle, CPoint (0, 0));
	newFrame->addView (knob01);	

	//-- creating another knob, we are offsetting the rect, so that the knob is next to the previous knob
	r.offset (background->getWidth () + 5, 0);
	CKnob* knob02 = new CKnob (r, this, kRightVolumeParameter, background, handle, CPoint (0, 0));
	newFrame->addView (knob02);

	CRect r1 (100, 0, 149, 49);
	CAnimKnob* knob1 = new CAnimKnob (r1, this, kKnob1Parameter, 127, 49, png_knob1, CPoint (0, 0));
	newFrame->addView (knob1);

	CRect r2 (200, 0, 296, 96);
	CAnimKnob* knob2 = new CAnimKnob (r2, this, kKnob2Parameter, 7, 96, png_knob2, CPoint (0, 0));
	newFrame->addView (knob2);

	CRect r3 (0, 100, 64, 164);
	COnOffButton* switch1 = new COnOffButton (r3, this, kSwitch1Parameter, png_switch1);
	newFrame->addView (switch1);


	CRect r4 (200, 100, 264, 164);
	COnOffButton* switch2 = new COnOffButton (r4, this, kSwitch2Parameter, png_switch2);
	newFrame->addView (switch2);


	CRect r5 (0, 200, 1000, 1000);
	dataview = new DataView (r5);
	newFrame->addView (dataview);

	//-- forget the bitmaps
	background->forget ();
	handle->forget ();
	png_knob1->forget ();
	png_knob2->forget ();
	png_switch1->forget ();
	png_switch2->forget ();

	//-- remember our controls so that we can sync them with the state of the effect
	controls[kLeftVolumeParameter] = knob01;
	controls[kRightVolumeParameter] = knob02;
	controls[kKnob1Parameter] = knob1;
	controls[kKnob2Parameter] = knob2;
	controls[kSwitch1Parameter] = switch1;
	controls[kSwitch2Parameter] = switch2;

	*/


	//-- set the member frame to the newly created frame
	//-- we do it this way because it is possible that the setParameter method is called 
	//-- in between of constructing the frame and it's controls
	frame = newFrame;

	//-- sync parameters
	for (int i = 0; i < kNumParameters; i++)
		setParameter (i, effect->getParameter (i));

	return true;
}

//------------------------------------------------------------------------------------
void TutorialEditor::close ()
{
	//-- on close we need to delete the frame object.
	//-- once again we make sure that the member frame variable is set to zero before we delete it
	//-- so that calls to setParameter won't crash.
	CFrame* oldFrame = frame;
	frame = 0;
	oldFrame->forget ();
}

//------------------------------------------------------------------------------------
void TutorialEditor::valueChanged (CControl* pControl)
{

	int32_t index = pControl->getTag ();
	float value = pControl->getValue ();
	console->print ("index:" + std::to_string(index) + " value:" + std::to_string(value));

	//-- valueChanged is called whenever the user changes one of the controls in the User Interface (UI)
	effect->setParameterAutomated (index, value);
	CTextEdit* te = nullptr;
	switch (index)
	{
		case kHScaleParameter:
			te = (CTextEdit*)controls[kHScaleTextParameter];
			te->setValue (value);
			te->setDirty (true);

			dataview->hScale = 1 + 999 * value;
			dataview->setDirty (true);
			break;
		case kVScaleParameter:
			te = (CTextEdit*)controls[kVScaleTextParameter];
			te->setValue (value);
			te->setDirty (true);

			dataview->vScale = 0.0025 + (0.25 - 0.0025) * value;
			dataview->setDirty (true);
			break;
		case kVShiftParameter:
			te = (CTextEdit*)controls[kVShiftTextParameter];
			te->setValue (value);
			te->setDirty (true);

			dataview->vShift = 2 * value - 1;
			dataview->setDirty (true);
			break;
		case kLChannelParameter:
			dataview->lChannel = value;
			dataview->setDirty (true);
			break;
		case kRChannelParameter:
			dataview->rChannel = value;
			dataview->setDirty (true);
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------------
void TutorialEditor::setParameter (VstInt32 index, float value)
{
	//std::string s = console->getText ();
	//console->setText (s + " | frontend setParameter(" + std::to_string (index) + ", " + std::to_string (value) + ")");

	//-- setParameter is called when the host automates one of the effects parameter.
	//-- The UI should reflect this state so we set the value of the control to the new value.
	//-- VSTGUI will automatically redraw changed controls in the next idle (as this call happens to be in the process thread).
	if (frame && index < kNumParameters)
	{
		controls[index]->setValue (value);
	}

}


//CWaveFormView* TutorialEditor::getWaveFormView()
//{
//	return wfv;
//}

} // VSTGUI
