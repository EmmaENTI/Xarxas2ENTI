#pragma once
#include <list>
#include <mutex>

#include "Button.h"

class Window
{
public:
	/// <summary>
	/// Recordar que el constructor crea la ventana y qu no se pude tocar la ventana
	/// desde otro thread que no sea el que la ha creado
	/// Para mas cossas, addX(), el boton es un ejemplo
	/// </summary>
	Window(unsigned int width = 800, unsigned int height = 600, std::string title = "Windows");

	void AddButton(Button* bt);
	void RunWindowsLoop(); 

private:
	
	std::mutex _mutex;
	sf::RenderWindow _window;

	Button* _lastClickedDownBt;
	std::list<Button*> _buttons;
	std::list<sf::Drawable*> _objectsToDraw;
};

