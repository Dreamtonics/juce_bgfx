//
//  Copyright (C) 2022 Arthur Benilov <arthur.benilov@gmail.com>
//

#pragma once

#include <JuceHeader.h>

namespace juce {
    class NSViewComponent;
}

/**
    Bgfx context is a low-level binding between bgfx
    and the native window system. The implementation is
    platform dependent.

    @note Due to bgfx limitations currently there can be only
          one bgfx context within the application. Creating another
          context will most likely fail.
*/
class BgfxContext final : public juce::ComponentListener
{
public:

    /** Overlay component that will have bgfx attached to it.

        The overlay component will pe placed over the component
        that has bgfx context attached. It will forward mouse events
        down to its peer underneath.
    */
    class Overlay final : public juce::Component
    {
    public:
        Overlay();

        void setForwardComponent (juce::Component*) noexcept;

        // juce::Component
        void mouseMove        (const juce::MouseEvent&) override;
        void mouseEnter       (const juce::MouseEvent&) override;
        void mouseExit        (const juce::MouseEvent&) override;
        void mouseDown        (const juce::MouseEvent&) override;
        void mouseDrag        (const juce::MouseEvent&) override;
        void mouseUp          (const juce::MouseEvent&) override;
        void mouseDoubleClick (const juce::MouseEvent&) override;
        void mouseWheelMove   (const juce::MouseEvent&, const juce::MouseWheelDetails&) override;
        void mouseMagnify     (const juce::MouseEvent&, float scaleFactor) override;

    private:

        juce::ComponentPeer* beginForward();
        void endForward();
        void forwardMouseEvent (const juce::MouseEvent&);
        void forwardMouseWheelEvent (const juce::MouseEvent&, const juce::MouseWheelDetails&);
        void forwardMouseMagnifyEvent (const juce::MouseEvent&, float scaleFactor);

        juce::Component* forwardComponent {nullptr};
        bool forwarding {false};
    };

    //==========================================================================

    BgfxContext();
    ~BgfxContext();

    /** Initialize the context.

        This will create the bgfx context and create the overlay
        component if required.
    */
    void init (float scaleFactor = 1.0f, bool showRenderStats = false);

    /** Attach the context to the given component. */
    void attachTo (juce::Component* component);

    /** Detach the context from the currently attached component. */
    void detach();

    /** Assign the background colour.

        The background colour is used upon bgfx context creation and
        re-creation. Once created the backround colour does not change.
    */
    void setBackgroundColour (const juce::Colour& c);

    bool isInitialised() const noexcept { return initialised; }
    float getScaleFactor() const noexcept { return scale; }

    void reset();

    // juce::ComponentListener
    void componentMovedOrResized (juce::Component& component, bool wasMoved, bool wasResized) override;
    void componentBeingDeleted (juce::Component& component) override;

    void repaintPeer();

private:

    void trackOverlay (bool moved, bool resized);

#if JUCE_WINDOWS
    void updateWindowPosition (juce::Rectangle<int> bounds);
#endif

    bool initialised {false};
    float scale {1.0f};

    juce::Component* attachedComponent {nullptr};

    Overlay overlay{};

#if JUCE_MAC
    std::unique_ptr<juce::NSViewComponent> embeddedView{nullptr};
#elif JUCE_WINDOWS
    std::unique_ptr<juce::ComponentPeer> nativeWindow {nullptr};
#else
#   error Unsupported platform
#endif

    uint32_t backgroundColour {0x000000ff};
};
