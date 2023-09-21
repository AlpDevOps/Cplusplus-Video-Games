#include <SFML/Graphics.hpp>
#include <time.h>

// Use the SFML namespace for easier access to its classes
using namespace sf;

// Define grid size, cell size, and window dimensions
int N = 30, M = 20;
int size = 16;
int w = size * N;
int h = size * M;

// Variables to store direction and snake length
int dir, num = 4;

// Snake structure with x and y coordinates
struct Snake
{
    int x, y;
} s[100];

// Fruit structure with x and y coordinates
struct Fruit
{
    int x, y;
} f;

// Game logic update function
void Tick()
{
    // Move the snake body forward
    for (int i = num; i > 0; --i)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    // Update snake head position based on direction
    if (dir == 0) s[0].y += 1;
    if (dir == 1) s[0].x -= 1;
    if (dir == 2) s[0].x += 1;
    if (dir == 3) s[0].y -= 1;

    // Check for snake eating fruit
    if ((s[0].x == f.x) && (s[0].y == f.y))
    {
        num++; // Increase snake length
        f.x = rand() % N;
        f.y = rand() % M; // Place a new fruit randomly
    }

    // Wrap snake around if it goes out of the screen
    if (s[0].x > N) s[0].x = 0;
    if (s[0].x < 0) s[0].x = N;
    if (s[0].y > M) s[0].y = 0;
    if (s[0].y < 0) s[0].y = M;

    // Check for snake collision with itself
    for (int i = 1; i < num; i++)
        if (s[0].x == s[i].x && s[0].y == s[i].y) num = i; // Cut snake size at collision point
}

int main()
{
    // Initialize random number generator with the current time as seed
    srand(time(0));

    // Create a window with specified dimensions and title
    RenderWindow window(VideoMode(w, h), "Snake Game!");

    // Load textures from image files
    Texture t1, t2;
    t1.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/white.png"); // Background grid texture
    t2.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/red.png");   // Snake and fruit texture

    // Create sprites using the loaded textures
    Sprite sprite1(t1);
    Sprite sprite2(t2);

    Clock clock;
    float timer = 0, delay = 0.1;

    // Set initial fruit position
    f.x = 10;
    f.y = 10;

    // Game loop
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        // Player controls using arrow keys
        if (Keyboard::isKeyPressed(Keyboard::Left)) dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Up)) dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Down)) dir = 0;

        // Update game logic periodically based on delay
        if (timer > delay) { timer = 0; Tick(); }

        ////// Drawing code ///////
        window.clear();

        // Draw the background grid
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
            {
                sprite1.setPosition(i * size, j * size);
                window.draw(sprite1);
            }

        // Draw the snake body
        for (int i = 0; i < num; i++)
        {
            sprite2.setPosition(s[i].x * size, s[i].y * size);
            window.draw(sprite2);
        }
    
        // Draw the fruit
        sprite2.setPosition(f.x * size, f.y * size);
        window.draw(sprite2);

        window.display();
    }

    return 0;
}
