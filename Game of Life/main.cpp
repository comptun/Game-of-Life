#include <SFML/Graphics.hpp>

#define GRID_HEIGHT		100
#define GRID_WIDTH		100
#define WINDOW_HEIGHT	800
#define WINDOW_WIDTH	800
#define BLOCK_SIZE		(WINDOW_HEIGHT/GRID_HEIGHT)

sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of Life");

sf::RectangleShape aliveCell;

enum state
{
	DEAD,
	ALIVE
};

state grid[GRID_HEIGHT][GRID_WIDTH];
state grid2[GRID_HEIGHT][GRID_WIDTH];

void update() 
{
	window.clear();
	for (int i = 0; i < GRID_HEIGHT; ++i) {
		for (int j = 0; j < GRID_WIDTH; ++j) {
			if (grid[i][j] == ALIVE) {
				aliveCell.setPosition(BLOCK_SIZE * j, BLOCK_SIZE * i);
				window.draw(aliveCell);
			}
		}
	}
	window.display();
}

int main() 
{
	update();
	aliveCell.setFillColor(sf::Color::White);
	aliveCell.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
	sf::Clock tick;
	bool simulating = false;
	while (window.isOpen()) {

		sf::Event e;

		sf::Vector2i mouse = sf::Mouse::getPosition(window);

		if (mouse.y > 0 && mouse.y < WINDOW_HEIGHT && mouse.x > 0 && mouse.x < WINDOW_WIDTH) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !simulating) {
				grid[int(mouse.y / BLOCK_SIZE)][int(mouse.x / BLOCK_SIZE)] = ALIVE;
				update();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			simulating = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			simulating = false;
		}

		if (simulating) {
			if (tick.getElapsedTime().asSeconds() > 0.2f) {
				for (int i = 1; i < GRID_HEIGHT-1; ++i) {
					for (int j = 1; j < GRID_WIDTH-1; ++j) {
						int neighbours = 0;
						if (grid[i + 1][j + 1] == ALIVE) ++neighbours;
						if (grid[i + 1][j - 1] == ALIVE) ++neighbours;
						if (grid[i - 1][j + 1] == ALIVE) ++neighbours;
						if (grid[i - 1][j - 1] == ALIVE) ++neighbours;
						if (grid[i][j - 1] == ALIVE) ++neighbours;
						if (grid[i][j + 1] == ALIVE) ++neighbours;
						if (grid[i - 1][j] == ALIVE) ++neighbours;
						if (grid[i + 1][j] == ALIVE) ++neighbours;
						if (neighbours == 0 || neighbours == 1) {
							grid2[i][j] = DEAD;
						}
						if (neighbours >= 4) {
							grid2[i][j] = DEAD;
						}
						if (neighbours == 3) {
							grid2[i][j] = ALIVE;
						}
					}
				}
				for (int i = 1; i < GRID_HEIGHT - 1; ++i) {
					for (int j = 1; j < GRID_WIDTH - 1; ++j) {
						grid[i][j] = grid2[i][j];
					}
				}
				update();
				tick.restart();
			}
		}

		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}
	}
}