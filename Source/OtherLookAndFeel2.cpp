/*
  ==============================================================================

    OtherLookAndFeel2.cpp
    Created: 27 Feb 2024 11:41:26am
    Author:  arcsl

  ==============================================================================
*/

#include "OtherLookAndFeel2.h"

OtherLookAndFeel2::OtherLookAndFeel2()
{
}

OtherLookAndFeel2::~OtherLookAndFeel2()
{
}

void OtherLookAndFeel2::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    // Calculations for drawing the rotary slider
    float dia = jmin(width, height);
    float rad = dia * 0.5;
    float focusX = x + width * 0.5;
    float focusY = y + height * 0.5;
    float radX = focusX - rad;
    float radY = focusY - rad;
    float ang = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    // Get the colors set in setSlider function
    Colour gradientFillColour = slider.findColour(Slider::rotarySliderFillColourId);
    Colour gradientOutlineColour = slider.findColour(Slider::rotarySliderOutlineColourId);

    // Create a gradient for the rotary slider fill
    ColourGradient gradient(Colours::transparentBlack, focusX, focusY,
        gradientFillColour, focusX + rad, focusY + rad, true);

    // Fill
    g.setGradientFill(gradient);
    g.fillEllipse(radX, radY, dia, dia);

    // Outline
    g.setColour(gradientOutlineColour);
    g.drawEllipse(radX + 1.5f, radY + 1.5f, dia - 3.0f, dia - 3.0f, 2.0f);

    // Draw a white pointer
    g.setColour(Colours::white);
    Path pointer;
    pointer.addRectangle(0, -rad, 3.0f, rad / 1.7);
    g.fillPath(pointer, AffineTransform::rotation(ang).translated(focusX, focusY));
}

void OtherLookAndFeel2::setSlider(Slider& slider, bool isDeck1)
{
    // Remove any existing look and feel
    slider.setLookAndFeel(nullptr);

    if (isDeck1)
    {
        // Customize for Deck 1
        slider.setColour(Slider::rotarySliderFillColourId, Colours::orange);
        slider.setColour(Slider::rotarySliderOutlineColourId, Colours::darkorange);
    }
    else
    {
        // Customize for Deck 2
        slider.setColour(Slider::rotarySliderFillColourId, Colours::darkblue);
        slider.setColour(Slider::rotarySliderOutlineColourId, Colours::deepskyblue);
    }

    // Set the custom look and feel
    slider.setLookAndFeel(this);
}