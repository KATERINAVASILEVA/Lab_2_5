#pragma once
#include <SFML/Graphics.hpp>
#include "Circle.hpp"
#include <string>
#include <thread>

namespace mt
{

	const float pi = acos(-1);

	class Game
	{
		int m_width;
		int m_height;
		std::string m_capture;
		mt::Circle* m_c;
		bool* m_col;
		int m_n;
		sf::RenderWindow m_window;
		bool m_firstCollision = true;


	public:
		Game(int width, int height, const std::string& capture)
		{
			m_width = width;
			m_height = height;
			m_capture = capture;
		}

		void Setup(int n)
		{
			m_n = n;

			m_window.create(sf::VideoMode(m_width, m_height), m_capture);

			srand(time(0));

			m_c = new mt::Circle[m_n];
			m_col = new bool[m_n];

			std::fill(m_col, m_col + m_n, true); ///

			for (int i = 0; i < m_n; i++)
			{
				int r = rand() % 15 + 50;
				int x = rand() % (1000 - 2 * r)+r;
				int y = rand() % (800 - 2 * r)+r;
				int v = rand() % 50 + 40;
				float alfa = rand() % 7;
				if (x - r < 0) 
					x = r;
				if (x + r > m_width)
					x = m_width - r;
				if (y + r > m_height)
					y = m_height - r;
				if (y - r < 0)
					y = r;
				m_c[i].Setup(x, y, r, v, alfa);
				m_col[i] = true;
			}
		}

		void TouchBorder(Circle& obj)
		{
			float x = obj.X();
			float y = obj.Y();
			float r = obj.R();

			if (x + r >= m_width || x - r <= 0)
			{
				obj.Vx(-obj.Vx());
			}

			if (y + r >= m_height || y - r <= 0)
			{
				obj.Vy(-obj.Vy());
			}
		}

		void Collision(Circle& c1, Circle& c2, int i, int j)
		{
			float dist = pow(c1.X() - c2.X(), 2) + pow(c1.Y() - c2.Y(), 2);
			if (dist <= pow(c1.R() + c2.R(), 2))
			{
				if (m_col[i] && m_col[j])
				{
					m_col[i] = false;
					m_col[j] = false;

					float xm = c1.X() + (c2.X() - c1.X()) * (c1.R() / (c1.R() + c2.R()));
					float ym = c1.Y() + (c2.Y() - c1.Y()) * (c1.R() / (c1.R() + c2.R()));

					float px = c2.X() - c1.X();
					float py = c2.Y() - c1.Y();

					float len = sqrt(pow(px, 2) + pow(py, 2));
					float beta = acos((px) / len);

					if ((px > 0 && py > 0) || (px < 0 && py > 0))
						beta = -beta;

					std::cout << "Beta " << beta << std::endl;

					std::cout << c1.Vx() << " " << c1.Vy() << " " << c2.Vx() << " " << c2.Vy() << std::endl;

					float v1x = c1.Vx() * cos(beta) - c1.Vy() * sin(beta);
					float v1y = c1.Vx() * sin(beta) + c1.Vy() * cos(beta);
					float v2x = c2.Vx() * cos(beta) - c2.Vy() * sin(beta);
					float v2y = c2.Vx() * sin(beta) + c2.Vy() * cos(beta);

					//std::cout << v1x << " " << v1y << " " << v2x << " " << v2y << std::endl;

					float v1y_s = v1y;
					float v2y_s = v2y;
					float v1x_s = v2x;
					float v2x_s = v1x;

					//std::cout << v1x_s << " " << v1y_s << " " << v2x_s << " " << v2y_s << std::endl;

					c1.Vx(v1x_s * cos(beta) + v1y_s * sin(beta));
					c1.Vy(-v1x_s * sin(beta) + v1y_s * cos(beta));
					c2.Vx(v2x_s * cos(beta) + v2y_s * sin(beta));
					c2.Vy(-v2x_s * sin(beta) + v2y_s * cos(beta));
				}
			}
			else
			{
				m_col[i] = true;
				m_col[j] = true;
			}
		}

		void LifeCycle()
		{
			sf::Clock clock;

			while (m_window.isOpen())
			{
				sf::Event event;
				while (m_window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						m_window.close();
				}

				//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

				// Логика
				float dt = clock.getElapsedTime().asSeconds();
				clock.restart();

				// Перемещение шарика
				for (int i = 0; i < m_n; i++)
					m_c[i].Move(dt);

				// Столкновение шариков
				for (int i = 0; i < m_n - 1; i++)
					for (int j = i + 1; j < m_n; j++)
						Collision(m_c[i], m_c[j], i, j);

				for (int i = 0; i < m_n; i++)
					TouchBorder(m_c[i]);

				// Отображкние
				m_window.clear();
				for (int i = 0; i < m_n; i++)
					m_window.draw(m_c[i].Get());
				m_window.display();
			}
		}
	};

}