#include "Window.h"

Window::Window(unsigned int width, unsigned int height, std::string title)
{
	//
	_window.create(sf::VideoMode(width, height), title);
}

void Window::AddButton(Button* bt)
{
	_mutex.lock();

	_buttons.push_back(bt);
	_objectsToDraw.push_back(bt);

	_mutex.unlock();
}

void Window::RunWindowsLoop()
{
	//Hauria de ser thread safe

	while (_window.isOpen())
	{
		_window.clear(sf::Color::Black);
		for (sf::Drawable* drawable : _objectsToDraw)
		{
			//Esto no es necesario pero comotrabajamos con punteros nos aseguramos
			//Lo ideal es que este if no estubiese y siempre que entramos aqui estamos seguros
			//que los objetos que hay estan bien gestionados
			if (drawable != nullptr)
			{
				_window.draw(*drawable);
			}
		}
		_window.display();

		sf::Event event;
		while (_window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				{
				_window.close();
				break;
				}
				case sf::Event::MouseButtonPressed:
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i clickPixelPos = { event.mouseButton.x, event.mouseButton.y };
						sf::Vector2f worldPos = _window.mapPixelToCoords(clickPixelPos);

						for (auto it = _buttons.rbegin(); it != _buttons.rend(); it++)
						{
							Button* bt = *it;
							if (bt->CheckBound(worldPos.x, worldPos.y))
							{
								_lastClickedDownBt = bt;
								break;
							}
						}
					}
					break;
				}
				case sf::Event::MouseButtonReleased:
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i clickPixelPos = { event.mouseButton.x, event.mouseButton.y };
						sf::Vector2f worldPos = _window.mapPixelToCoords(clickPixelPos);

						if (_lastClickedDownBt != nullptr && _lastClickedDownBt->CheckBound(worldPos.x, worldPos.y))
						{
							_lastClickedDownBt->onClick();
							_lastClickedDownBt = nullptr;
						}
					}
					break;
				}
				case sf::Event::MouseWheelScrolled:
				{
					break;
				}
			}
		}

		//Execute the tasks
		_taskMutex.lock();

		size_t size = _tasks.size();
		for (int i = 0; i < size; i++)
		{
			_tasks[i]();
		}
		//Es neteja perque nomes la fas un cop, has de resetear per asegurarte que les tasques al tornar a utilitzarles son netes
		_tasks.clear();
		_taskMutex.unlock();
	}

}

void Window::AddTask(Task task)
{
	_taskMutex.lock();
	_tasks.push_back(task);
	_taskMutex.unlock();
}


//Ajedrez

//void Window::InitChessBoard() {
//	// Lógica para crear y posicionar los botones que representan el tablero de ajedrez
//	for (int i = 0; i < 8; ++i) {
//		std::vector<Button*> row;
//		for (int j = 0; j < 8; ++j) {
//			Button* button = new Button(j * 64, i * 64, "texture_path_for_empty_square");
//			// Asignar texturas de piezas según la posición en el tablero
//			// Lo hacemos en plan poniendo el initial state del ajedrez al principio
//			row.push_back(button);
//			AddButton(button); //boton para cada button
//		}
//		_chessBoard.push_back(row);
//	}
//}