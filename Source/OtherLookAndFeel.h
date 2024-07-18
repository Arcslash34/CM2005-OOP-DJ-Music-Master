/*
  ==============================================================================

    SetLookAndFeel.h
    Created: 17 Feb 2024 6:47:05pm
    Author:  arcsl

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/**
 * The OtherLookAndFeel class is a custom LookAndFeel_V4 implementation.
 * It provides a customized appearance for rotary sliders in the DJ application.
 * The drawRotarySlider method is overridden to render a rotary slider with a disc image
 * that can be rotated based on the rotation angle set by the rotateDisc method.
 */
class OtherLookAndFeel : public LookAndFeel_V4
{
public:
    /**
     * Constructor for OtherLookAndFeel.
     */
    OtherLookAndFeel();

    /**
     * Destructor for OtherLookAndFeel.
     */
    ~OtherLookAndFeel();

    /**
     * Draw a rotary slider with a customized appearance, including a rotating disc image.
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
     * Rotate the disc image of the rotary slider to the specified angle.
     *
     * @param ang   The angle at which to rotate the disc image.
     */
    void rotateDisc(double ang);

private:
    double rotAngle = 0.0;  /**< The rotation angle of the disc image. */
};