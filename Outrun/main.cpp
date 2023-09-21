// Including necessary SFML libraries for graphics functionality
#include <SFML/Graphics.hpp>

// Setting the SFML namespace for easier access to its classes and functions
using namespace sf;

// Global constants defining the screen dimensions, road width, segment length, and camera depth
int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200;
float camD = 0.84;

// Function to draw a quadrilateral on the screen given its vertices and color
void drawQuad(RenderWindow &w, Color c, int x1,int y1,int w1,int x2,int y2,int w2)
{
    ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, Vector2f(x1-w1,y1));
    shape.setPoint(1, Vector2f(x2-w2,y2));
    shape.setPoint(2, Vector2f(x2+w2,y2));
    shape.setPoint(3, Vector2f(x1+w1,y1));
    w.draw(shape);
}

// Struct defining properties of a road segment
struct Line
{
    float x,y,z;     // 3D center of line
    float X,Y,W;     // Screen coordinates
    float curve,spriteX,clip,scale;
    Sprite sprite;

    Line() {spriteX=curve=x=y=z=0;} // Constructor initializing values

    // Function to project the 3D coordinates of the road segment onto the 2D screen
    void project(int camX,int camY,int camZ)
    {
        scale = camD/(z-camZ);
        X = (1 + scale*(x - camX)) * width/2;
        Y = (1 - scale*(y - camY)) * height/2;
        W = scale * roadW  * width/2;
    }

    // Function to render a sprite (like a car or tree) on the road segment
    void drawSprite(RenderWindow &app)
    {
        Sprite s = sprite;
        int w = s.getTextureRect().width;
        int h = s.getTextureRect().height;
        float destX = X + scale * spriteX * width/2;
        float destY = Y + 4;
        float destW  = w * W / 266;
        float destH  = h * W / 266;

        destX += destW * spriteX;
        destY += destH * (-1);

        float clipH = destY+destH-clip;
        if (clipH<0) clipH=0;
        if (clipH>=destH) return;

        s.setTextureRect(IntRect(0,0,w,h-h*clipH/destH));
        s.setScale(destW/w,destH/h);
        s.setPosition(destX, destY);
        app.draw(s);
    }
};

int main()
{
    // Create a window with the specified dimensions and title
    RenderWindow app(VideoMode(width, height), "Outrun Racing!");
    app.setFramerateLimit(60);

    // Load textures for various game sprites
    Texture t[50];
    Sprite object[50];
    for(int i=1;i<=7;i++)
    {
        t[i].loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/"+std::to_string(i)+".png");
        t[i].setSmooth(true);
        object[i].setTexture(t[i]);
    }

    // Load background texture and set its properties
    Texture bg;
    bg.loadFromFile("/Users/yalcin/cmake-sfml-project/Build/images/bg.png");
    bg.setRepeated(true);
    Sprite sBackground(bg);
    sBackground.setTextureRect(IntRect(0,0,5000,411));
    sBackground.setPosition(-2000,0);

    // Create road lines (segments) with curves and sprites placed at intervals
    std::vector<Line> lines;
    for(int i=0;i<1600;i++)
    {
        Line line;
        line.z = i*segL;
        // ... (Various line property settings) ...

        lines.push_back(line);
    }

    int N = lines.size();
    float playerX = 0;
    int pos = 0;
    int H = 1500;

    // Game loop
    while (app.isOpen())
    {
        // Event processing loop
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        // Player control handling (keyboard inputs)
        // ... (Control handling code) ...

        app.clear(Color(105,205,4));
        app.draw(sBackground);

        // Rendering of the road and the sprites on it
        // ... (Road rendering code) ...

        app.display();  // Display all drawn items to the screen
    }

    return 0;
}
