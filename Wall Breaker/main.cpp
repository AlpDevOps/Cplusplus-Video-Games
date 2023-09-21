#include <SFML/Graphics.hpp>
#include <time.h>

// Use the SFML namespace for easier access to its classes
using namespace sf;

int main()
{
    // Initialize random number generator with the current time as seed
    srand(time(0));

    // Create a window with specified dimensions and title
    RenderWindow app(VideoMode(520, 450), "Arkanoid!");
    // Limit the framerate to 60 FPS
    app.setFramerateLimit(60);

    // Load textures from image files
    Texture t1, t2, t3, t4;
    t1.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/block01.png");
    t2.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/background.jpg");
    t3.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/ball.png");
    t4.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/paddle.png");

    // Create sprites using the loaded textures
    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    // Set initial position for the paddle
    sPaddle.setPosition(300, 440);

    // Array to store block sprites
    Sprite block[1000];

    // Create block positions and assign texture
    int n = 0;
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            block[n].setTexture(t1);
            block[n].setPosition(i * 43, j * 20);
            n++;
        }

    // Define ball movement speed in the x and y directions
    float dx = 6, dy = 5;
    // Set initial ball position
    float x = 300, y = 300;

    // Main game loop
    while (app.isOpen())
    {
        Event e;
        // Event handling loop
        while (app.pollEvent(e))
        {
            // If the window is closed, close the application
            if (e.type == Event::Closed)
                app.close();
        }

        // Update ball position in x direction
        x += dx;
        // Check ball collision with blocks
        for (int i = 0; i < n; i++)
            if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
            {
                block[i].setPosition(-100, 0); // Move the block out of view after collision
                dx = -dx; // Change ball direction in x axis
            }

        // Update ball position in y direction
        y += dy;
        // Check ball collision with blocks
        for (int i = 0; i < n; i++)
            if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
            {
                block[i].setPosition(-100, 0); // Move the block out of view after collision
                dy = -dy; // Change ball direction in y axis
            }

        // If ball hits the left or right screen edge, change x direction
        if (x < 0 || x > 520) dx = -dx;
        // If ball hits the top or bottom screen edge, change y direction
        if (y < 0 || y > 450) dy = -dy;

        // Move paddle with arrow keys
        if (Keyboard::isKeyPressed(Keyboard::Right)) sPaddle.move(6, 0);
        if (Keyboard::isKeyPressed(Keyboard::Left)) sPaddle.move(-6, 0);

        // Check collision between ball and paddle
        if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds()))
            dy = -(rand() % 5 + 2); // Bounce the ball up after collision

        // Update ball sprite's position
        sBall.setPosition(x, y);

        // Draw game elements
        app.clear();
        app.draw(sBackground);
        app.draw(sBall);
        app.draw(sPaddle);

        // Draw each block
        for (int i = 0; i < n; i++)
            app.draw(block[i]);

        // Display everything drawn in the window
        app.display();
    }

    return 0;
}
