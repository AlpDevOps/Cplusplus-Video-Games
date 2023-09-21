#include <SFML/Graphics.hpp>
using namespace sf;

const int num = 8; // Number of checkpoints
int points[num][2] = { /* Checkpoint coordinates initialized here */ };

// Car structure definition
struct Car
{
    float x, y, speed, angle;
    int n;  // current checkpoint target

    Car()
    {
        speed = 2;
        angle = 0;
        n = 0;
    }

    // Move the car based on its speed and angle
    void move()
    {
        x += sin(angle) * speed;
        y -= cos(angle) * speed;
    }

    // Adjust the car's angle to head towards the next target checkpoint
    void findTarget()
    {
        float tx = points[n][0];
        float ty = points[n][1];
        float beta = angle - atan2(tx - x, -ty + y);
        
        if (sin(beta) < 0) angle += 0.005 * speed;
        else angle -= 0.005 * speed;

        // If the car reaches the checkpoint, target the next one
        if ((x - tx)*(x - tx) + (y - ty)*(y - ty) < 25 * 25) n = (n + 1) % num;
    }
};

int main()
{
    RenderWindow app(VideoMode(640, 480), "Car Racing Game!");
    app.setFramerateLimit(60);

    // Load game assets
    Texture t1;
    t1.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/background.png");
    t1.setSmooth(true);
    Sprite sBackground(t1);

    Texture t2;
    t2.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/car.png");
    t2.setSmooth(true);
    Sprite sCar(t2);
    sCar.setOrigin(22, 22);
    float R = 22;

    const int N = 5;  // Number of cars
    Car car[N];
    for (int i = 0; i < N; i++)
    {
        // Initialize cars at different positions with different speeds
        car[i].x = 300 + i * 50;
        car[i].y = 1700 + i * 80;
        car[i].speed = 7 + i;
    }

    float speed = 0, angle = 0;
    float maxSpeed = 12.0;
    float acc = 0.2, dec = 0.3;
    float turnSpeed = 0.08;

    int offsetX = 0, offsetY = 0;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        // Capture keyboard input
        bool Up = 0, Right = 0, Down = 0, Left = 0;
        if (Keyboard::isKeyPressed(Keyboard::Up)) Up = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) Right = 1;
        if (Keyboard::isKeyPressed(Keyboard::Down)) Down = 1;
        if (Keyboard::isKeyPressed(Keyboard::Left)) Left = 1;

        // Control the first car with keyboard input
        if (Up && speed < maxSpeed) speed = (speed < 0) ? speed + dec : speed + acc;
        if (Down && speed > -maxSpeed) speed = (speed > 0) ? speed - dec : speed - acc;
        if (!Up && !Down) speed = (speed > dec) ? speed - dec : ((speed < -dec) ? speed + dec : 0);
        if (Right && speed != 0) angle += turnSpeed * speed / maxSpeed;
        if (Left && speed != 0) angle -= turnSpeed * speed / maxSpeed;

        car[0].speed = speed;
        car[0].angle = angle;

        // Move all cars
        for (int i = 0; i < N; i++) car[i].move();
        // All cars except the first one (controlled by the player) find their target checkpoint
        for (int i = 1; i < N; i++) car[i].findTarget();

        // Collision detection and resolution
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
            {
                int dx = 0, dy = 0;
                while (dx * dx + dy * dy < 4 * R * R)
                {
                    car[i].x += dx / 10.0;
                    car[i].x += dy / 10.0;
                    car[j].x -= dx / 10.0;
                    car[j].y -= dy / 10.0;
                    dx = car[i].x - car[j].x;
                    dy = car[i].y - car[j].y;
                    if (!dx && !dy) break;
                }
            }

        app.clear();

        // Adjust view based on the first car's position
        if (car[0].x > 320) offsetX = car[0].x - 320;
        if (car[0].y > 240) offsetY = car[0].y - 240;

        sBackground.setPosition(-offsetX, -offsetY);
        app.draw(sBackground);

        // Draw all cars
        Color colors[10] = {Color::Red, Color::Green, Color::Magenta, Color::Blue, Color::White};
        for (int i = 0; i < N; i++)
        {
            sCar.setPosition(car[i].x - offsetX, car[i].y - offsetY);
            sCar.setRotation(car[i].angle * 180 / 3.141593);
            sCar.setColor(colors[i]);
            app.draw(sCar);
        }

        app.display();
    }

    return 0;
}
