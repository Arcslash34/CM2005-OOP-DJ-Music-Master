/*
  ==============================================================================

    OtherLookAndFeel2.h
    Created: 27 Feb 2024 11:41:26am
    Author:  arcsl

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/**
 * The OtherLookAndFeel2 class is a custom LookAndFeel_V4 implementation.
 * It provides a customized appearance for rotary sliders in the DJ application.
 * The drawRotarySlider method is overridden to render a rotary slider with a specific color scheme.
 * The setSlider method is provided to configure the color scheme based on the deck type.
 */
class OtherLookAndFeel2 : public LookAndFeel_V4
{
public:
    /**
     * Constructor for OtherLookAndFeel2.
     */
    OtherLookAndFeel2();

    /**
     * Destructor for OtherLookAndFeel2.
     */
    ~OtherLookAndFeel2();

    /**
     * Draw a rotary slider with a customized color scheme.
     *
     * @param g                 The Graphics context for rendering.
     * @param x                 The x-coordinate of the slider.
     * @param y                 The y-coordinate of the slider.
     * @param width             The width of the slider.
     * @param height            The height of the slider.
     * @param sliderPos         The position of the slider.
     * @param rotaryStartAngle  The start angle of the rotary slider.
     * @param rotaryEndAngle    The end angle of the rotary slider.
     * @param slider            The Slider object being rendered.
     */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider&) override;

    /**
     * Set the color scheme of the provided slider based on the deck type.
     *
     * @param slider   The Slider object for which to set the color scheme.
     * @param isDeck1  A boolean indicating whether the slider is associated with Deck 1.
     */
    void setSlider(Slider& slider, bool isDeck1);
};