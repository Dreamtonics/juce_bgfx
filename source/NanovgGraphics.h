//
//  Copyright (C) 2022 Arthur Benilov <arthur.benilov@gmail.com>
//

#pragma once

#include <JuceHeader.h>

#include <bgfx/bgfx.h>
#include <nanovg.h>

/**
    JUCE low level graphics context backed by nanovg.

    @note This is not a perfect translation of the JUCE
          graphics, but its still quite usable.
*/
class NanovgGraphicsContext : public juce::LowLevelGraphicsContext
{
public:
    NanovgGraphicsContext (NVGcontext* nanovgContext, int width, int height);
    ~NanovgGraphicsContext();

    // juce::LowLevelGraphicsContext
    bool isVectorDevice() const override;
    void setOrigin (juce::Point<int>) override;
    void addTransform (const juce::AffineTransform&) override;
    float getPhysicalPixelScaleFactor() override;

    bool clipToRectangle (const juce::Rectangle<int>&) override;
    bool clipToRectangleList (const juce::RectangleList<int>&) override;
    void excludeClipRectangle (const juce::Rectangle<int>&) override;
    void clipToPath (const juce::Path&, const juce::AffineTransform&) override;
    void clipToImageAlpha (const juce::Image&, const juce::AffineTransform&) override;

    bool clipRegionIntersects (const juce::Rectangle<int>&) override;
    juce::Rectangle<int> getClipBounds() const override;
    bool isClipEmpty() const override;

    void saveState() override;
    void restoreState() override;

    void beginTransparencyLayer (float opacity) override;
    void endTransparencyLayer() override;

    void setFill (const juce::FillType&) override;
    void setOpacity (float) override;
    void setInterpolationQuality (juce::Graphics::ResamplingQuality) override;

    void fillRect (const juce::Rectangle<int>&, bool) override;
    void fillRect (const juce::Rectangle<float>&) override;
    void fillRectList (const juce::RectangleList<float>&) override;
    void fillPath (const juce::Path&, const juce::AffineTransform&) override;
    void drawImage (const juce::Image&, const juce::AffineTransform&) override;
    void drawLine (const juce::Line<float>&) override;

    void setFont (const juce::Font&) override;
    const juce::Font& getFont() override;
    void drawGlyph (int glyphNumber, const juce::AffineTransform&) override;
    bool drawTextLayout (const juce::AttributedString&, const juce::Rectangle<float>&) override;

    void resized (int w, int h);

    void removeCachedImages();

    const static juce::String defaultTypefaceName;

    const static int imageCacheSize;

private:

    // bool loadFontFromResources (const juce::String& typefaceName);
    void applyFillType();
    void applyStrokeType();
    void applyFont();

    int getNvgImageId (const juce::Image& image);
    void reduceImageCache();

    NVGcontext* nvg{};

    int width{};
    int height{};

    juce::FillType fillType{};
    juce::Font font{};

    // Mapping glyph number to a character
    using GlyphToCharMap = std::map<int, wchar_t>;

    GlyphToCharMap getGlyphToCharMapForFont (const juce::Font& f);

    // Mapping font names to glyph-to-character tables
    std::map<juce::String, GlyphToCharMap> loadedFonts{};
    const GlyphToCharMap* currentGlyphToCharMap{};

    // Tracking images mapped tomtextures.
    struct NvgImage
    {
        int id {-1};            ///< Image/texture ID.
        int accessCounter {0};  ///< Usage counter.
    };

    std::map<juce::uint64, NvgImage> images;
};
