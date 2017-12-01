/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "ColorArray.h"
class ColorArray;

class AmusingSceneComponent;
class CustomLookAndFeel : public LookAndFeel_V4
{
	void drawRoundThumb(Graphics& g, const float x, const float y,
		const float diameter, const Colour& colour, float outlineThickness)
	{
		const Rectangle<float> a(x, y, diameter, diameter);
		const float halfThickness = outlineThickness * 0.5f;

		Path p;
		p.addEllipse(x + halfThickness, y + halfThickness, diameter - outlineThickness, diameter - outlineThickness);

		const DropShadow ds(Colours::black, 1, Point<int>(0, 0));
		ds.drawForPath(g, p);

		g.setColour(colour);
		g.fillPath(p);

		g.setColour(colour.brighter());
		g.strokePath(p, PathStrokeType(outlineThickness));
	}



	void drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override
	{
		const float sliderRadius = (float)(getSliderThumbRadius(slider) - 2);

		bool isDownOrDragging = slider.isEnabled() && (slider.isMouseOverOrDragging() || slider.isMouseButtonDown());
		Colour knobColour(slider.findColour(Slider::thumbColourId).withMultipliedSaturation((slider.hasKeyboardFocus(false) || isDownOrDragging) ? 1.3f : 0.9f)
			.withMultipliedAlpha(slider.isEnabled() ? 1.0f : 0.7f));

		if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
		{
			float kx, ky;

			if (style == Slider::LinearVertical)
			{
				kx = x + width * 0.5f;
				ky = sliderPos;
			}
			else
			{
				kx = sliderPos;
				ky = y + height * 0.5f;
			}

			const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

			drawRoundThumb(g,
				kx - sliderRadius,
				ky - sliderRadius,
				sliderRadius * 2.0f,
				knobColour, outlineThickness);
		}
		else
		{
			// Just call the base class for the demo
			LookAndFeel_V2::drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
		}
	}

	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override
	{
		g.fillAll(slider.findColour(Slider::backgroundColourId));

		if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
		{
			const float fx = (float)x, fy = (float)y, fw = (float)width, fh = (float)height;

			Path p;

			if (style == Slider::LinearBarVertical)
				p.addRectangle(fx, sliderPos, fw, 1.0f + fh - sliderPos);
			else
				p.addRectangle(fx, fy, sliderPos - fx, fh);


			Colour baseColour(slider.findColour(Slider::rotarySliderFillColourId)
				.withMultipliedSaturation(slider.isEnabled() ? 1.0f : 0.5f)
				.withMultipliedAlpha(0.8f));

			g.setColour(baseColour);
			g.fillPath(p);

			const float lineThickness = jmin(15.0f, jmin(width, height) * 0.45f) * 0.1f;
			g.drawRect(slider.getLocalBounds().toFloat(), lineThickness);
		}
		else
		{
			drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
			drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
		}
	}

	void drawLinearSliderBackground(Graphics& g, int x, int y, int width, int height,
		float /*sliderPos*/,
		float /*minSliderPos*/,
		float /*maxSliderPos*/,
		const Slider::SliderStyle /*style*/, Slider& slider) override
	{
		const float sliderRadius = getSliderThumbRadius(slider) - 5.0f;
		Path on, off;

		if (slider.isHorizontal())
		{
			const float iy = y + height * 0.5f - sliderRadius * 0.5f;
			Rectangle<float> r(x - sliderRadius * 0.5f, iy, width + sliderRadius, sliderRadius);
			const float onW = r.getWidth() * ((float)slider.valueToProportionOfLength(slider.getValue()));

			on.addRectangle(r.removeFromLeft(onW));
			off.addRectangle(r);
		}
		else
		{
			const float ix = x + width * 0.5f - sliderRadius * 0.5f;
			Rectangle<float> r(ix, y - sliderRadius * 0.5f, sliderRadius, height + sliderRadius);
			const float onH = r.getHeight() * ((float)slider.valueToProportionOfLength(slider.getValue()));

			on.addRectangle(r.removeFromBottom(onH));
			off.addRectangle(r);
		}

		g.setColour(slider.findColour(Slider::rotarySliderFillColourId));
		g.fillPath(on);

		g.setColour(slider.findColour(Slider::trackColourId));
		g.fillPath(off);

		g.setColour(Colours::white);
		//g.drawLine(x, y, x + width, y + height);
		if (showScaleMarking == true)
		{
			int minInt = (int)ceil(minValue);
			int maxInt = (int)floor(maxValue);

			for (int i = minInt; i <= maxInt; i++)
			{
				int posX = roundToInt((double)width * pow((((double)i - minValue)   / (maxValue - minValue)),skewFactor));
				g.fillRect(x+posX, y, 1, height);//(int)mySlider->getPositionOfValue(i)

			}
		}


	}
	public :
	void setScaleMarking(double _minValue, double _maxValue, double _skewFactor)
	{
		showScaleMarking = true;
		minValue = _minValue;
		maxValue = _maxValue;
		skewFactor = _skewFactor;
	}

	double minValue, maxValue;
	bool showScaleMarking = false;
	double skewFactor = 1;


};

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AreaOptions  : public Component,
                     public Slider::Listener
{
public:
    //==============================================================================
    AreaOptions ();
    ~AreaOptions();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void CompleteInitialization(AmusingSceneComponent* m_amusingSceneComponent);
	void setSpeedSliderValue(double speed);
	void setOctaveSlider(int currentOctave);
	void setVelocitySliderValue(double _velocity);
	void OnColorChanged(Colour newColour);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AmusingSceneComponent* amusingSceneComponent;
	CustomLookAndFeel customLook;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> baseNote;
    ScopedPointer<Slider> speed;
    ScopedPointer<Label> speedLabel;
    ScopedPointer<Slider> velocitySlider;
    ScopedPointer<ColorArray> colorArray;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AreaOptions)
};

//[EndFile] You can add extra defines here...



//[/EndFile]
