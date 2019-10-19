#include "Private/Linux/TeLinuxWindow.h"
#include "Private/Linux/TeLinuxPlatform.h"

namespace te
{
    struct LinuxWindow::Pimpl
	{
		::Window XWindow = 0;

		INT32 X, Y;
		UINT32 Width, Height;
		bool HasTitleBar = true;
		bool ResizeDisabled = false;
		bool IsExternal = false;
    };

    LinuxWindow::LinuxWindow(const WINDOW_DESC &desc)
	{
		_data = te_new<Pimpl>(); 

		TE_PRINT("CREATE LINUX WINDOW")
		
		// TODO
    }

    LinuxWindow::~LinuxWindow()
	{
		if(_data->XWindow != 0)
		{
			XUnmapWindow(LinuxPlatform::GetXDisplay(), _data->XWindow);
			XSync(LinuxPlatform::GetXDisplay(), 0);

			XDestroyWindow(LinuxPlatform::GetXDisplay(), _data->XWindow);
			XSync(LinuxPlatform::GetXDisplay(), 0);

			_data->XWindow = 0;
		}

		te_delete(_data);
	}

	void LinuxWindow::Move(INT32 x, INT32 y)
	{
		// TODO
	}

	void LinuxWindow::Resize(UINT32 width, UINT32 height)
	{
		// TODO
	}

	void LinuxWindow::Hide()
	{
		// TODO
	}

	void LinuxWindow::Show()
	{
		// TODO
	}

	void LinuxWindow::Maximize()
	{
		// TODO
	}

	void LinuxWindow::Minimize()
	{
		// TODO
	}

	void LinuxWindow::Restore()
	{
		// TODO
	}

	INT32 LinuxWindow::GetLeft() const
	{
		return 0; // TODO
	}

	INT32 LinuxWindow::GetTop() const
	{
		return 0; // TODO
	}

	UINT32 LinuxWindow::GetWidth() const
	{
		return 0; // TODO
	}

	UINT32 LinuxWindow::GetHeight() const
	{
		return 0; // TODO
	}

	Vector2I LinuxWindow::WindowToScreenPos(const Vector2I& windowPos) const
	{
		return Vector2I(); // TODO
	}

	Vector2I LinuxWindow::ScreenToWindowPos(const Vector2I& screenPos) const
	{
		return Vector2I(); // TODO
	}

	::Window LinuxWindow::_getXWindow() const
	{
		return _data->XWindow;
	}
}