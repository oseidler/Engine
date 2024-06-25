#pragma once
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Rgba8.hpp"


//forward declarations
class Renderer;
class Camera;
class BitmapFont;
class Stopwatch;
struct AABB2;

class DevConsole;
extern DevConsole* g_theDevConsole;


//support structs
struct DevConsoleLine
{
	Rgba8 m_color;
	std::string m_text;
};

struct DevConsoleConfig
{
	Renderer* m_renderer = nullptr;
	Camera* m_camera = nullptr;
	std::string m_fontName = "SquirrelFixedFont";
	float m_fontAspect = 0.7f;
	float m_linesOnScreen = 40.0f;
	int m_maxCommandHistory = 128;
};


class DevConsole
{
//public member functions
public:
	//constructor and destructor
	DevConsole(DevConsoleConfig const& config);
	~DevConsole();

	//game flow functions
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	void Render(AABB2 const& bounds);

	//dev console utilities
	void Execute(std::string const& consoleCommandText, bool echoLine = true);
	void AddLine(Rgba8 const& color, std::string const& text);
	void ToggleOpen();

	//xml functions
	void ExecuteXmlCommandScriptNode(XmlElement const& commandScriptXmlElement);
	void ExecuteXmlCommandScriptFile(std::string const& commandScriptXmlFilePath);

	//accessors
	bool IsOpen() const;

	//event functions
	static bool Event_KeyPressed(EventArgs& args);
	static bool Event_CharInput(EventArgs& args);
	static bool Command_Clear(EventArgs& args);
	static bool Command_Help(EventArgs& args);
	static bool Command_Echo(EventArgs& args);

//public member variables
public:
	//static color variables
	static Rgba8 const COLOR_ERROR;
	static Rgba8 const COLOR_WARNING;
	static Rgba8 const COLOR_INFO_MAJOR;
	static Rgba8 const COLOR_INFO_MINOR;
	static Rgba8 const COLOR_COMMAND_ECHO;
	static Rgba8 const COLOR_INPUT_TEXT;
	static Rgba8 const COLOR_INPUT_CARET;
	static Rgba8 const COLOR_NETWORK_ECHO;

//protected member variables
protected:
	DevConsoleConfig m_config;

	bool m_isOpen = false;

	std::vector<DevConsoleLine> m_lines;
	std::mutex					m_linesMutex;

	std::string m_inputText;

	int m_caretPosition = 0;
	bool m_caretVisible = true;
	Stopwatch* m_caretStopwatch = nullptr;

	std::vector<std::string> m_commandHistory;
	std::mutex				 m_commandHistoryMutex;

	int m_historyIndex = -1;

	BitmapFont* devConsoleFont = nullptr;
};
