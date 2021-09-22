#include "DataView.h"

namespace VSTGUI {

DataView::DataView(const VSTGUI::CRect& size) : CView(size) {
	count = 0;
	loc = size;
	bufLHead = 0;
	bufRHead = 0;
	bufLTail = 0;
	bufRTail = 0;
	memset (bufL, 0, sizeof (float) * DATAVIEW_BUFLEN);
	memset (bufR, 0, sizeof (float) * DATAVIEW_BUFLEN);

	hScale = 1000;
	vScale = 0.25;
	vShift = 0;
	lChannel = 1.0;
	rChannel = 1.0;

	console = nullptr;
}

DataView::DataView (const VSTGUI::CRect& size, Console* c) : CView (size)
{
	count = 0;
	loc = size;
	bufLHead = 0;
	bufRHead = 0;
	bufLTail = 0;
	bufRTail = 0;
	memset (bufL, 0, sizeof (float) * DATAVIEW_BUFLEN);
	memset (bufR, 0, sizeof (float) * DATAVIEW_BUFLEN);

	hScale = 1000;
	vScale = 0.25;
	vShift = 0;
	console = c;

}



void DataView::draw(CDrawContext* ctx) {
	//std::string s = console->getText ();
	//console->setText (s + "DataView draw() | ");

	ctx->setLineWidth (1);
	ctx->setFillColor (CColor(225, 225, 225, 225));
	ctx->setFrameColor (CColor (225, 225, 225, 225));

	ctx->drawRect (loc, kDrawFilledAndStroked);

	drawGrid (ctx);
	drawTimeMark (ctx);
	drawValueMark (ctx);

	if (bufLHead == bufLTail)
	{
		//console->print ("Empty!!!");
		return;
	}

	CPoint prevL(630, 330);
	CPoint currL(630, 330);

	CPoint prevR (630, 330);
	CPoint currR (630, 330);

	int newestIx = -1;
	int oldestIx = -1;

	if (bufLHead == 0)
	{
		newestIx = DATAVIEW_BUFLEN - 1;
	}
	else
	{
		newestIx = bufLHead - 1;
	}


	bool done = false;
	int count = 0;
	float jumpFactor = (441.0f / 600.0f) * hScale;
	int sampleIx = 0;
	int viewIx = 0;


	for (viewIx = 629; viewIx >= 30; viewIx--)
	{
		prevL.x = currL.x;
		prevL.y = currL.y;

		currL.x = viewIx;

		prevR.x = currR.x;
		prevR.y = currR.y;

		currR.x = viewIx;


		sampleIx = newestIx - int (jumpFactor * count);

		//count++;
		if (rChannel != 0)
		{
			if (bufRTail < bufRHead)
			{
				if (sampleIx < bufRTail)
				{
					done = true;
					break;
				}
			}
			else
			{
				if (sampleIx < 0)
				{
					break;
				}
			}

			currR.y = bufR[sampleIx] * (-240) / (4 * vScale) + 330 + (240 * vShift);
			ctx->setFrameColor (kRedCColor);
			ctx->drawLine (prevR, currR);
		}

		if (lChannel != 0)
		{
			if (bufLTail < bufLHead)
			{
				if (sampleIx < bufLTail)
				{
					done = true;
					break;
				}
			}
			else
			{
				if (sampleIx < 0)
				{
					break;
				}
			}

			currL.y = bufL[sampleIx] * (-240) / (4 * vScale) + 330 + (240 * vShift);
			ctx->setFrameColor (kBlueCColor);
			ctx->drawLine (prevL, currL);

		}
		count++;		
	}

	if (done == false)
	{
		//console->print ("rChannel:" + std::to_string (rChannel));
		//console->print ("viewIx:" + std::to_string(viewIx));
		for (; viewIx >= 30; viewIx--)
		{

			if (rChannel != 0)
			{
				prevR.x = currR.x;
				prevR.y = currR.y;

				currR.x = viewIx;
				sampleIx = newestIx - int (jumpFactor * count) + DATAVIEW_BUFLEN;

				// count++;

				if (sampleIx < bufRTail)
				{
					break;
				}

				currR.y = bufR[sampleIx] * (-240) / (4 * vScale) + 330 + (240 * vShift);
				ctx->setFrameColor (kRedCColor);
				ctx->drawLine (prevR, currR);
			}

			if (lChannel != 0)
			{
				prevL.x = currL.x;
				prevL.y = currL.y;

				currL.x = viewIx;
				sampleIx = newestIx - int (jumpFactor * count) + DATAVIEW_BUFLEN;

				// count++;

				if (sampleIx < bufLTail)
				{
					break;
				}

				currL.y = bufL[sampleIx] * (-240) / (4 * vScale) + 330 + (240 * vShift);
				ctx->setFrameColor (kBlueCColor);
				ctx->drawLine (prevL, currL);			
			}
			count++;
		}
	}
	
/*	{
		std::string ct = " | count: " + std::to_string (count);
		std::string dt = " | dist: " + std::to_string (bufLHead - bufLTail);
		std::string bh = " | head: " + std::to_string (bufLHead);
		std::string bt = " | tail: " + std::to_string (bufLTail);


		UTF8String s = UTF8String (ct + dt + bh + bt);
		ctx->drawString (s, CPoint (90, 570));
	}*/

	setDirty (false);
}

void DataView::addData (float** inputs, int sampleFrames)
{
	for (int i = 0; i < sampleFrames; i++)
	{
		bufL[bufLHead] = inputs[0][i];
		bufR[bufRHead] = inputs[1][i];
		
		bufLHead = (bufLHead + 1) % DATAVIEW_BUFLEN;
		bufRHead = (bufRHead + 1) % DATAVIEW_BUFLEN;

		if (bufLHead == bufLTail)
		{
			bufLTail = (bufLTail + 1) % DATAVIEW_BUFLEN;
		}

		if (bufRHead == bufRTail)
		{
			bufRTail = (bufRTail + 1) % DATAVIEW_BUFLEN;
		}
	}

	setDirty (true);
}

void DataView::drawGrid (CDrawContext* ctx)
{
	ctx->setLineWidth (1);
	ctx->setFrameColor (CColor (100, 100, 100, 225));

	for (int i = 1; i < 10; i++)
	{
		int xval = 30 + (i * 60);
		ctx->drawLine (CPoint (xval, 90), CPoint (xval, 570));
	}

	for (int i = 1; i < 10; i++)
	{
		int yval = 30 + (i * 60);
		ctx->drawLine (CPoint (30, yval), CPoint (630, yval));
	}
}

void DataView::drawTimeMark (CDrawContext* ctx)
{
	//std::string s = console->getText ();
	//console->setText (s + " drawTimeMark() ");
	ctx->setFontColor (CColor (0, 0, 0, 225));

	float realVal = hScale;
	//int val = 10;
	for (int i = 9; i > 0; i--)
	{
		int displayVal = int (realVal);
		UTF8String s = UTF8String (std::to_string (displayVal) + " ms");
		int x = 15 + 60 * i;
		ctx->drawString (s, CPoint (x, 102));
		realVal += hScale;
	}
}

void DataView::drawValueMark (CDrawContext* ctx) {
	//console->print ("drawValueMark()");

	ctx->setFontColor (CColor (0, 0, 0, 225));
	char buffer[50];

	for (int i = 3; i >= -3; i--)
	{
		float displayVal = vShift + vScale*i;
		int n = sprintf (buffer, "%1.4f", displayVal);
		UTF8String s = UTF8String (buffer);
		int y = -60 * i + 330;
		ctx->drawString (s, CPoint (35, y));		
	}

	//console->print (buffer);

	//std::string ss = std::format ("Hello {}!\n", "world");
}

void DataView::drawDebug(CDrawContext* ctx) {

}

}