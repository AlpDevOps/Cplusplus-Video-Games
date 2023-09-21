#include <SFML/Graphics.hpp>
#include <time.h>

// Use the SFML namespace for easier access to its classes
using namespace sf;

// Structure to represent a point with x and y coordinates
struct point
{
    int x,y;
};

int main()
{
    // Initialize random number generator with current time as seed
    srand(time(0));

    // Create a window with specified dimensions and title
    RenderWindow app(VideoMode(400, 533), "Doodle Game!");
    // Limit the framerate to 60 FPS
    app.setFramerateLimit(60);

    // Load textures from image files
    Texture t1, t2, t3;
    t1.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/background.png");
    t2.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/platform.png");
    t3.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/doodle.png");

    // Create sprites using the loaded textures
    Sprite sBackground(t1), sPlat(t2), sPers(t3);

    // Create an array of platform points
    point plat[20];

    // Randomly initialize platform positions
    for (int i=0; i<10; i++)
    {
        plat[i].x = rand() % 400;
        plat[i].y = rand() % 533;
    }

    // Initialize character position and height
    int x = 100, y = 100, h = 200;
    // Initialize character's speed in x and y direction
    float dx = 0, dy = 0;

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

        // Control character movement with arrow keys
        if (Keyboard::isKeyPressed(Keyboard::Right)) x += 3;
        if (Keyboard::isKeyPressed(Keyboard::Left)) x -= 3;

        // Simulate gravity by increasing y-speed
        dy += 0.2;
        // Update character's y-position
        y += dy;
        // If character reaches the bottom, bounce back up
        if (y > 500) dy = -10;

        // If character's y-position is above a certain height, move platforms down
        if (y < h)
        {
            for (int i=0; i<10; i++)
            {
                y = h;
                plat[i].y = plat[i].y - dy;
                // If platform goes below the screen, reposition it at the top
                if (plat[i].y > 533)
                {
                    plat[i].y = 0;
                    plat[i].x = rand() % 400;
                }
            }
        }

        // Check for collision between character and platforms
        for (int i=0; i<10; i++)
            if ((x+50 > plat[i].x) && (x+20 < plat[i].x+68)
                && (y+70 > plat[i].y) && (y+70 < plat[i].y+14) && (dy > 0))
                dy = -10; // Bounce the character up after collision

        // Set character sprite's position
        sPers.setPosition(x, y);

        // Draw game elements
        app.draw(sBackground); // Background
        app.draw(sPers); // Character

        // Draw each platform
        for (int i=0; i<10; i++)
        {
            sPlat.setPosition(plat[i].x, plat[i].y);
            app.draw(sPlat);
        }

        // Display everything drawn in the window
        app.display();
    }

    return 0;
}
