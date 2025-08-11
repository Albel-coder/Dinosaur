#pragma once
#include <Windows.h>

struct Console
{
public:
	Console();

	// Setting minimum sizes (default values)
	void SetMinimumSize(short MinWidth, short MinHeight) noexcept;

	// Basic methods
	void UpdateSize() noexcept;
	bool IsSizeChanged() noexcept;
	bool IsSizeAcceptable() const noexcept;
	void SetCursorVisible(bool show, short size = 25) noexcept;

	// Getters(constant for security)
	short GetWidth() const noexcept;
	short GetHeight() const noexcept;

private:
	// Getting console handle (caching)
	HANDLE GetConsoleHandle() noexcept;

	// Current sizes
	short CurrentWidth = 0;
	short CurrentHeight = 0;

	// Minimum dimensions
	unsigned char MinWidth_ = 80;
	unsigned char MinHeight_ = 25;

	// Cached console handle
	HANDLE hConsole = nullptr;
};