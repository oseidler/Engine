#include "Engine/Core/DevConsole.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


//global dev console variable
DevConsole* g_theDevConsole = nullptr;

//static color types
Rgba8 const DevConsole::COLOR_ERROR = Rgba8(255, 0, 0);
Rgba8 const DevConsole::COLOR_WARNING = Rgba8(255, 255, 0);
Rgba8 const DevConsole::COLOR_INFO_MAJOR = Rgba8(0, 255, 255);
Rgba8 const DevConsole::COLOR_INFO_MINOR = Rgba8(0, 127, 255);
Rgba8 const DevConsole::COLOR_COMMAND_ECHO = Rgba8(0, 255, 0);
Rgba8 const DevConsole::COLOR_INPUT_TEXT = Rgba8(255, 255, 255);
Rgba8 const DevConsole::COLOR_INPUT_CARET = Rgba8(255, 255, 255);
Rgba8 const DevConsole::COLOR_NETWORK_ECHO = Rgba8(0, 127, 0);


//
//constructor and destructor
//
DevConsole::DevConsole(DevConsoleConfig const& config)
	:m_config(config)
{
}


DevConsole::~DevConsole()
{
}


//
//public game flow functions
//
void DevConsole::Startup()
{
	//subscribe to key and char events
	SubscribeEventCallbackFunction("CharInput", Event_CharInput);
	SubscribeEventCallbackFunction("KeyPressed", Event_KeyPressed);
	SubscribeEventCallbackFunction("help", Command_Help);
	SubscribeEventCallbackFunction("clear", Command_Clear);
	SubscribeEventCallbackFunction("Echo", Command_Echo);

	m_caretStopwatch = new Stopwatch(0.5f);

	AddLine(DevConsole::COLOR_INFO_MINOR, "Type help for a list of commands.");
}


void DevConsole::Shutdown()
{
	if (m_caretStopwatch != nullptr)
	{
		delete m_caretStopwatch;
		m_caretStopwatch = nullptr;
	}
}


void DevConsole::BeginFrame()
{
}


void DevConsole::EndFrame()
{
}


void DevConsole::Render(AABB2 const& bounds)
{
	if (m_isOpen)
	{
		std::vector<Vertex_PCU> boxVerts;

		//add dev console transparent background
		AddVertsForAABB2(boxVerts, bounds, Rgba8(0, 0, 0, 169));

		m_config.m_renderer->BindTexture(nullptr);
		m_config.m_renderer->DrawVertexArray((int)boxVerts.size(), boxVerts.data());

		//create bitmap font
		std::string bitmapFontFilePath = "Data/Fonts/" + m_config.m_fontName;
		devConsoleFont = m_config.m_renderer->CreateOrGetBitmapFont(bitmapFontFilePath.c_str());

		//calculate line height
		float lineHeight = bounds.GetDimensions().y / m_config.m_linesOnScreen;

		//render input line
		std::vector<Vertex_PCU> inputVerts;
		AABB2 inputBox = AABB2(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.x, bounds.m_mins.y + lineHeight);

		devConsoleFont->AddVertsForTextInBox2D(inputVerts, inputBox, lineHeight, m_inputText, DevConsole::COLOR_INPUT_TEXT, m_config.m_fontAspect, Vec2(0.0f, 0.0f));

		m_config.m_renderer->BindTexture(&devConsoleFont->GetTexture());
		m_config.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
		m_config.m_renderer->DrawVertexArray((int)inputVerts.size(), inputVerts.data());

		//render all other lines
		std::vector<Vertex_PCU> historyVerts;

		m_linesMutex.lock();
		for (int lineIndex = 0; lineIndex < m_lines.size(); lineIndex++)
		{
			if (lineIndex < m_config.m_linesOnScreen - 1)
			{
				AABB2 historyBox = AABB2(bounds.m_mins.x, bounds.m_mins.y + lineHeight * (lineIndex + 1), bounds.m_maxs.x, bounds.m_mins.y + lineHeight * (lineIndex + 2));
				devConsoleFont->AddVertsForTextInBox2D(historyVerts, historyBox, lineHeight, m_lines[lineIndex].m_text, m_lines[lineIndex].m_color, m_config.m_fontAspect, Vec2(0.0f, 0.0f));
			}
		}
		m_linesMutex.unlock();
		
		m_config.m_renderer->DrawVertexArray((int)historyVerts.size(), historyVerts.data());
		
		//render caret
		while (m_caretStopwatch->DecrementDurationIfElapsed())
		{
			m_caretVisible = !m_caretVisible;
		}

		if (m_caretVisible)
		{
			float charWidth = lineHeight * m_config.m_fontAspect;
			float caretStartX = bounds.m_mins.x + charWidth * m_caretPosition;
			float caretStartY = bounds.m_mins.y;
			AABB2 caretBox = AABB2(caretStartX, caretStartY, caretStartX + (charWidth * 0.2f), caretStartY + lineHeight);

			std::vector<Vertex_PCU> caretVerts;

			AddVertsForAABB2(caretVerts, caretBox, DevConsole::COLOR_INPUT_CARET);

			m_config.m_renderer->BindTexture(nullptr);
			m_config.m_renderer->DrawVertexArray((int)caretVerts.size(), caretVerts.data());
		}
	}
}


//
//public dev console utilities
//
void DevConsole::Execute(std::string const& consoleCommandText, bool echoLine)
{
	std::string commandEvent;
	std::string commandArg;
	std::string commandParam;
	
	//Parse consoleCommandText into event name and args, then fire as an event
	Strings splitCommandText = SplitStringWithQuotes(consoleCommandText, ' ');
	int numCommandStrings = static_cast<int>(splitCommandText.size());

	if (numCommandStrings > 0)
	{
		commandEvent = splitCommandText[0];

		if (numCommandStrings > 1)
		{
			EventArgs args = EventArgs();
			
			for (int stringIndex = 1; stringIndex < numCommandStrings; stringIndex++)
			{
				Strings splitArgText = SplitStringWithQuotes(splitCommandText[stringIndex], '=');
				if (splitArgText.size() != 2)
				{
					AddLine(DevConsole::COLOR_ERROR, "Error: arguments must be passed in using the format arg=param");
					if (commandEvent != "Echo" && echoLine)
					{
						AddLine(DevConsole::COLOR_COMMAND_ECHO, consoleCommandText);
					}

					m_inputText.clear();
					m_caretPosition = 0;
					return;
				}

				//remove quotes from parameter string
				TrimString(splitArgText[1], '\"');

				args.SetValue(splitArgText[0], splitArgText[1]);
			}


			if (commandEvent != "Echo" && echoLine)
			{
				AddLine(DevConsole::COLOR_COMMAND_ECHO, consoleCommandText);
				
			}
			FireEvent(commandEvent, args);
		}
		else if (numCommandStrings == 1)
		{
			if (commandEvent != "Echo" && echoLine)
			{
				AddLine(DevConsole::COLOR_COMMAND_ECHO, consoleCommandText);
			}
			FireEvent(commandEvent);
		}
	}

	//clear input text
	m_inputText.clear();
	m_caretPosition = 0;
}


void DevConsole::AddLine(Rgba8 const& color, std::string const& text)
{
	DevConsoleLine newLine;
	newLine.m_color = color;
	newLine.m_text = text;
	m_linesMutex.lock();
	m_lines.insert(m_lines.begin(), newLine);
	m_linesMutex.unlock();
}


void DevConsole::ToggleOpen()
{
	m_isOpen = !m_isOpen;
	if (m_isOpen)
	{
		m_caretStopwatch->Start();
	}
	else
	{
		m_caretStopwatch->Stop();
	}
}


void DevConsole::ExecuteXmlCommandScriptNode(XmlElement const& commandScriptXmlElement)
{
	XmlElement const* commandElement = commandScriptXmlElement.FirstChildElement();
	while (commandElement != nullptr)
	{
		std::string commandString = commandElement->Name();
		XmlAttribute const* attribute = commandElement->FirstAttribute();
		while (attribute != nullptr)
		{
			char const* attributeName = attribute->Name();
			char const* attributeValue = attribute->Value();
			std::string attributeString = Stringf(" %s=\"%s\"", attributeName, attributeValue);
			commandString += attributeString;
			attribute = attribute->Next();
		}

		Execute(commandString);

		commandElement = commandElement->NextSiblingElement();
	}
}


void DevConsole::ExecuteXmlCommandScriptFile(std::string const& commandScriptXmlFilePath)
{
	XmlDocument scriptFileXml;
	XmlError result = scriptFileXml.LoadFile(commandScriptXmlFilePath.c_str());
	GUARANTEE_OR_DIE(result == tinyxml2::XML_SUCCESS, "Failed to open script file!");
	XmlElement* rootElement = scriptFileXml.RootElement();
	GUARANTEE_OR_DIE(rootElement != nullptr, "Failed to read script file root element!");
	g_theDevConsole->ExecuteXmlCommandScriptNode(*rootElement);
}


//
//public accessors
//
bool DevConsole::IsOpen() const
{
	return m_isOpen;
}


//
//static event functions
//
bool DevConsole::Event_KeyPressed(EventArgs& args)
{
	if (g_theDevConsole != nullptr)
	{
		if (g_theDevConsole->m_caretStopwatch != nullptr)
		{
			g_theDevConsole->m_caretStopwatch->Restart();
			g_theDevConsole->m_caretVisible = true;
		}

		unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);

		//toggle open if tilde is pressed
		if (keyCode == KEYCODE_TILDE)
		{
			g_theDevConsole->ToggleOpen();

			return true;
		}
		else
		{
			if (g_theDevConsole->m_isOpen)
			{
				//handle all other key inputs only if it's open
				if (keyCode == KEYCODE_ENTER)
				{
					if (g_theDevConsole->m_inputText.length() > 0)
					{
						g_theDevConsole->Execute(g_theDevConsole->m_inputText);
					}
					else
					{
						g_theDevConsole->ToggleOpen();
					}
				}
				else if (keyCode == KEYCODE_ESC)
				{
					if (g_theDevConsole->m_inputText.length() > 0)
					{
						g_theDevConsole->m_inputText.clear();
						g_theDevConsole->m_caretPosition = 0;
					}
					else
					{
						g_theDevConsole->ToggleOpen();
					}
				}
				else if (keyCode == KEYCODE_LEFT)
				{
					if (g_theDevConsole->m_caretPosition > 0)
					{
						g_theDevConsole->m_caretPosition--;
					}
				}
				else if (keyCode == KEYCODE_RIGHT)
				{
					if (g_theDevConsole->m_caretPosition < g_theDevConsole->m_inputText.length())
					{
						g_theDevConsole->m_caretPosition++;
					}
				}
				else if (keyCode == KEYCODE_HOME)
				{
					g_theDevConsole->m_caretPosition = 0;
				}
				else if (keyCode == KEYCODE_END)
				{
					g_theDevConsole->m_caretPosition = (int)g_theDevConsole->m_inputText.length();
				}
				else if (keyCode == KEYCODE_BACKSPACE)
				{
					if (g_theDevConsole->m_caretPosition > 0)
					{
						g_theDevConsole->m_inputText.erase(--g_theDevConsole->m_caretPosition, 1);
					}
				}
				else if (keyCode == KEYCODE_DELETE)
				{
					g_theDevConsole->m_inputText.erase(g_theDevConsole->m_caretPosition, 1);
				}

				return true;
			}
		}
	}

	return false;
}


bool DevConsole::Event_CharInput(EventArgs& args)
{
	if (g_theDevConsole != nullptr)
	{
		if (g_theDevConsole->m_caretStopwatch != nullptr)
		{
			g_theDevConsole->m_caretStopwatch->Restart();
			g_theDevConsole->m_caretVisible = true;
		}

		unsigned char charCode = (unsigned char)args.GetValue("CharCode", -1);

		if (!g_theDevConsole->m_isOpen)
		{
			return false;
		}

		if (charCode == '`' || charCode == '~' || charCode < 32 || charCode > 126)
		{
			return false;
		}

		std::string charString;
		charString += charCode;

		g_theDevConsole->m_inputText.insert((size_t)g_theDevConsole->m_caretPosition, charString);

		g_theDevConsole->m_caretPosition++;

		return true;
	}

	return false;
}


bool DevConsole::Command_Clear(EventArgs& args)
{
	UNUSED(args);

	if (g_theDevConsole != nullptr)
	{
		g_theDevConsole->m_lines.clear();

		return true;
	}

	return false;
}


bool DevConsole::Command_Help(EventArgs& args)
{
	UNUSED(args);

	if (g_theDevConsole != nullptr)
	{
		g_theDevConsole->AddLine(DevConsole::COLOR_INFO_MAJOR, "Registered Commands: ");

		std::vector<std::string> allEventNames = g_theEventSystem->GetAllRegisteredEvents();

		for (int nameIndex = 0; nameIndex < allEventNames.size(); nameIndex++)
		{
			g_theDevConsole->AddLine(DevConsole::COLOR_INFO_MINOR, allEventNames[nameIndex]);
		}
	}

	return false;
}


bool DevConsole::Command_Echo(EventArgs& args)
{
	std::string echoedString = args.GetValue("Message", "");
	TrimString(echoedString, '\"');
	g_theDevConsole->AddLine(DevConsole::COLOR_NETWORK_ECHO, echoedString);
	
	return true;
}
