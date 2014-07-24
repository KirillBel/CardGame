#pragma once
#define GGUIID_INPUT 1
#define DIM_LBUTTON 0
#define DIM_RBUTTON 1
#define DIM_WBUTTON 2

typedef bool (*mouseMoveCallback)(int, int, int, int);
typedef bool (*mouseWheelCallback)(int, int, long);
typedef bool (*mouseButtonCallback)(unsigned char,int, int, bool);
typedef bool (*buttonCallback)(unsigned char, bool);


class GInput
{
public:
	GInput(void);
	~GInput(void);

	
	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Release();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& x, int& y);
	bool GetKeyState(unsigned char KeyCode);
	bool GetMouseKeyState(unsigned char KeyCode);

	void addMouseMoveCallback(mouseMoveCallback value) {mMoveCallbacks.push_back(value);};
	void addMouseWheelCallback(mouseWheelCallback value) {mWheelCallbacks.push_back(value);};
	void addMouseButtonCallback(mouseButtonCallback value) {mButtonCallbacks.push_back(value);};
	void addButtonCallback(buttonCallback value) {kButtonCallbacks.push_back(value);};
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	std::list<mouseMoveCallback> mMoveCallbacks;
	std::list<mouseWheelCallback> mWheelCallbacks;
	std::list<mouseButtonCallback> mButtonCallbacks;
	std::list<buttonCallback> kButtonCallbacks;

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	unsigned char m_prevKeyboardState[256];

	BYTE m_prevMouseButtonsState[4];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	RECT m_screenBorders;
	int m_mouseX, m_mouseY;
	HWND m_hWnd;
	RECT WndRect;
};

