#include <SFML/Graphics.hpp>
#include <time.h>

// Use the SFML namespace for convenience
using namespace sf;

// Constants for grid dimensions
const int M = 20;
const int N = 10;

// Initialize game field with zeros
int field[M][N] = {0};

// Structure to hold point data for Tetris blocks
struct Point
{
    int x,y;
} a[4], b[4];

// Definitions for different Tetris shapes
int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5  // O
};

// Function to check if the current piece can be placed at the given coordinates
bool check()
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
        else if (field[a[i].y][a[i].x]) return 0;

    return 1;
};

int main()
{
    // Initialize random number generator
    srand(time(0));

    // Create a window with specified dimensions
    RenderWindow window(VideoMode(320, 480), "Tetris Game");

    // Load textures from files
    Texture t1, t2, t3;
    t1.loadFromFile("tiles.png");
    t2.loadFromFile("background.png");
    t3.loadFromFile("frame.png");

    // Create sprites from textures
    Sprite s(t1), background(t2), frame(t3);

    // Variables to store state
    int dx = 0; // horizontal move
    bool rotate = 0; // rotation flag
    int colorNum = 1; // color index
    float timer = 0; // timer for block falling
    float delay = 0.3; // initial delay for block falling

    // Initialize clock for timing
    Clock clock;

    // Main game loop
    while (window.isOpen())
    {
        // Update timer
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        // Event handling
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close(); // Close window

            // Key press events
            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Up) rotate = true;
                else if (e.key.code == Keyboard::Left) dx = -1;
                else if (e.key.code == Keyboard::Right) dx = 1;
            }
        }

        // Speed up block falling when down arrow key is pressed
        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

        // Move the piece horizontally
        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i]; // Undo if invalid move

        // Rotate the piece
          if (rotate)
          {
              Point p = a[1]; // center of rotation
              for (int i = 0; i < 4; i++)
              {
                  int x = a[i].y - p.y;
                  int y = a[i].x - p.x;
                  a[i].x = p.x - x;
                  a[i].y = p.y + y;
              }
              if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i]; // Undo if invalid rotation
          }

          // Tick: move the piece down
          if (timer > delay)
          {
              for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
              if (!check())
              {
                  for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum; // Freeze the piece

                  // Randomly select a new piece and color
                  colorNum = 1 + rand() % 7;
                  int n = rand() % 7;
                  for (int i = 0; i < 4; i++)
                  {
                      a[i].x = figures[n][i] % 2;
                      a[i].y = figures[n][i] / 2;
                  }
              }

              timer = 0; // Reset timer
          }

          // Check for completed lines
          int k = M - 1;
          for (int i = M - 1; i > 0; i--)
          {
              int count = 0;
              for (int j = 0; j < N; j++)
              {
                  if (field[i][j]) count++;
                  field[k][j] = field[i][j];
              }
              if (count < N) k--;
          }

          dx = 0; rotate = 0; delay = 0.3;

          // Drawing the game state
          window.clear(Color::White);
          window.draw(background);

          // Draw game field
          for (int i = 0; i < M; i++)
              for (int j = 0; j < N; j++)
              {
                  if (field[i][j] == 0) continue;
                  s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                  s.setPosition(j * 18, i * 18);
                  s.move(28, 31); // offset
                  window.draw(s);
              }

          // Draw current piece
          for (int i = 0; i < 4; i++)
          {
              s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
              s.setPosition(a[i].x * 18, a[i].y * 18);
              s.move(28, 31); // offset
              window.draw(s);
          }

          window.draw(frame); // Draw game frame
          window.display(); // Update the display
      }

      return 0;
  }

