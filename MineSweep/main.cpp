#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int main()
{
    // Initialize random seed
    srand(time(0));

    // Create the game window
    RenderWindow app(VideoMode(400, 400), "Minesweeper!");

    int w = 32; // Width and height of each tile
    int grid[12][12]; // The actual game grid
    int sgrid[12][12]; // Grid used for display purposes

    // Load tile texture
    Texture t;
    t.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/tiles.jpg");
    Sprite s(t);

    // Initialize the game grid and display grid
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            sgrid[i][j] = 10; // Set initial display value
            if (rand() % 5 == 0) grid[i][j] = 9; // 20% chance to place a mine
            else grid[i][j] = 0; // No mine
        }

    // Calculate numbers for the tiles based on neighboring mines
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            int n = 0; // Count of surrounding mines
            if (grid[i][j] == 9) continue;
            // Check each surrounding cell and increment n for each mine found
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++)
                    n += (grid[i + dx][j + dy] == 9);
            grid[i][j] = n;
        }

    // Game loop
    while (app.isOpen())
    {
        // Get the mouse position in the game window
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w; // Column
        int y = pos.y / w; // Row

        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            // Handle mouse button presses
            if (e.type == Event::MouseButtonPressed)
            {
                if (e.key.code == Mouse::Left) sgrid[x][y] = grid[x][y]; // Reveal the cell
                else if (e.key.code == Mouse::Right) sgrid[x][y] = 11; // Flag the cell as suspected mine
            }
        }

        // Drawing
        app.clear(Color::White);
        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                // If the player clicked on a mine, reveal the entire board
                if (sgrid[x][y] == 9) sgrid[i][j] = grid[i][j];
                // Set the texture rectangle based on the display grid value
                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                // Position and draw the sprite
                s.setPosition(i * w, j * w);
                app.draw(s);
            }

        app.display();
    }

    return 0;
}
