#pragma once

#include "Prerequisites/TePrerequisitesUtility.h"
#include "Math/TeVector2I.h"
#include "Image/TePixelData.h"
#include <X11/X.h>
#include <X11/Xutil.h>

namespace te
{
    class RenderWindow;

    /** Descriptor used for creating a platform specific native window. */
    struct WINDOW_DESC
    {
        INT32 X, Y;
        UINT32 Width, Height;
        UINT32 Screen;
        String Title;
        bool ShowDecorations;
        bool AllowResize;
        bool Hidden;
        ::Window External;
        XVisualInfo VisualInfo;
    };

    /** Represents a X11 window. */
    class TE_UTILITY_EXPORT LinuxWindow
    {
    public:
        LinuxWindow(const WINDOW_DESC& desc);
        ~LinuxWindow();

        /** Returns position of the left-most border of the window, relative to the screen. */
        INT32 GetLeft() const;

        /** Returns position of the top-most border of the window, relative to the screen. */
        INT32 GetTop() const;

        /** Returns width of the window in pixels. */
        UINT32 GetWidth() const;

        /** Returns height of the window in pixels. */
        UINT32 GetHeight() const;

        /** Hides the window. */
        void Hide();

        /** Shows (unhides) the window. */
        void Show();

        /** Minimizes the window. */
        void Minimize();

        /** Maximizes the window over the entire current screen. */
        void Maximize();

        /** Restores the window to original position and size if it is minimized or maximized. */
        void Restore();

        /** Change the size of the window. */
        void Resize(UINT32 width, UINT32 height);

        /** Reposition the window. */
        void Move(INT32 left, INT32 top);

        /** Sets the icon to display for the window. */
        void SetIcon(const PixelData& icon);

        /** Converts screen position into window local position. */
        Vector2I ScreenToWindowPos(const Vector2I& screenPos) const;

        /** Converts window local position to screen position. */
        Vector2I WindowToScreenPos(const Vector2I& windowPos) const;

        /** Returns the internal X11 window handle. */
        ::Window GetXWindow() const;

        /** Attaches renderWindow that can later be retrieved through _getUserData(). */
        void SetRenderWindow(RenderWindow* renderWindow);

        /** Returns user data attached to the object when _setUserData was called. */
        RenderWindow* GetRenderWindow() const;

    private:
        /** Checks if the window is currently maximized. */
        bool IsMaximized() const;

        /** Checks if the window is currently minimized (iconified). */
        bool IsMinimized();

        /**
         * Maximizes a window if @p enable is true. If false restores the window to size/position before maximization
         * occurred.
         */
        void Maximize(bool enable);

        /**
         * Minimizes a window if @p enable is true. If false restores the window to size/position before minimization
         * occurred.
         */
        void Minimize(bool enable);

    private:
        struct Pimpl;
        Pimpl* _data;
    };
}
