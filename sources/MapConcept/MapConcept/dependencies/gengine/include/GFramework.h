#pragma once

class GFramework
{
public:
	GFramework(void);
	virtual ~GFramework(void);

	/*static GFramework& GetFramework() 
	{
		return (GFramework&)GFramework::Instance();
	};*/
	void CreateFramework(int screenWidth, int screenHeight,HWND hwndParent, bool bWindowed, const char* AppName,int AntiAliasLevels=0);
	void ResizeWindow(int width, int height, bool bWindowed=true);

	void Release();
	bool Frame();
	bool isVisible();

	/*static GDevice* GetDevice()
	{
		return GetFramework().pDevice;
	};

	static GRenderer* GetRenderer()
	{
		return &GetFramework().renderer;
	};*/

	GInput& GetInput()
	{
		return input;
	};

	HWND GetWindow() {return m_hwnd;};

	LRESULT CALLBACK WindowMessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool createWindow(int screenWidth, int screenHeight,HWND hwndParent, bool bWindowed, const char* AppName);
	void releaseWindow();
	bool procWindowMessageCycle();
	void procWindowResize();
private:
	
	GInput input;

	LONG styleWindowed,styleFullScreen;

	bool bWindowed;
	const char* AppName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	Vec2i screenSize;
	bool bEnterSizeMove;
};



