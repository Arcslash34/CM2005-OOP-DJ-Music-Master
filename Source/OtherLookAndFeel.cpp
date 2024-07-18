/*
  ==============================================================================

    OtherLookAndFeel.cpp
    Created: 17 Feb 2024 6:47:05pm
    Author:  arcsl

  ==============================================================================
*/

#include "OtherLookAndFeel.h"

OtherLookAndFeel::OtherLookAndFeel() {

}

OtherLookAndFeel::~OtherLookAndFeel() {
}

void OtherLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    auto angle = MathConstants<float>::pi;
    slider.setRotaryParameters(angle, (angle * 2) + angle, true);


    float dia = jmin(width, height);
    float rad = dia * 0.5;
    float focusX = x + width * 0.5;
    float focusY = y + height * 0.5;
    float ang = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    AffineTransform rot = AffineTransform::rotation(ang).translated(focusX, focusY);

    auto djImg = ImageCache::getFromMemory(BinaryData::disc_png, BinaryData::disc_pngSize);

    g.addTransform(AffineTransform::rotation(rotAngle, focusX, focusY));

    g.drawImageWithin(djImg, x, y, width, height, RectanglePlacement::centred, false);
}

void OtherLookAndFeel::rotateDisc(double ang)
{
    rotAngle = ang;
}