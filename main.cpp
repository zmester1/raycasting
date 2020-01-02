#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <math.h>

#define mapWidth 13
#define mapHeight 13

int worldMap[mapHeight][mapWidth]=
{
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
};

int floorMap[mapHeight][mapWidth]=
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {0, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 0},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {0, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 0},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {0, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 0},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {0, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 0},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {0, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 0},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int mapSquare(int x, int y, int gameMap[mapHeight][mapWidth])
{
    if (x > mapWidth || x < 0 || y > mapHeight || y < 0 )
        return 0;

    return gameMap[y][x];
}

sf::Color getColor(int x, int y, int gameMap[mapHeight][mapWidth])
{
        sf::Color color;
        switch(mapSquare(x, y, gameMap))
        {
            case 1:
                color = sf::Color::Red;
                break; //red
            case 2:
                color = sf::Color::Green;
                break; //green
            case 3:
                color = sf::Color::Blue;
                break; //blue
            case 4:
                color = sf::Color::White;
                break; //white
            default:
                color = sf::Color::Yellow;
                break; //yellow
        }

        return color;
}

int main()
{
    int h = 600, w = 800;
    float posX = 480.0, posY = 480.0, angle = 1.30899694;
    float deg30  = 0.523599;
    float deg60  = 2.0 * deg30;
    float deg90  = 3.0 * deg30;
    float deg180 = 2.0 * deg90;
    float deg270 = 3.0 * deg90;
    float deg360 = 2.0 * deg180;
    float degInterval = deg60 / (float) w;
    float wallHeight = 64.0;
    float playerHeight = wallHeight / 2.0;
    float wallWidth = 64.0;
    int posZ = h / 2.0;
    float distanceToProjectionPlane = w / 2 / tan(deg30);

    auto time = std::chrono::system_clock::now();
    auto oldTime = std::chrono::system_clock::now();

    int rot = 1;
    int mov = 0;
    int straf = 0;

    sf::RenderWindow window(sf::VideoMode(w, h), "Raycasting!", sf::Style::Titlebar | sf::Style::Close);

    while(window.isOpen())
    {
        window.clear();

        if (angle >= deg360)
            angle -= deg360;
        else if (angle < 0)
            angle += deg360;

        float deltaRayAngle = -1.0 * deg30;

        sf::Image ground;
        ground.create(w, h/2, sf::Color::Transparent);

        for (int x = 0; x < w; x++)
        {
            float rayAngle = angle + deltaRayAngle;

            if (rayAngle >= deg360)
                rayAngle -= deg360;
            else if (rayAngle < 0)
                rayAngle += deg360;

            int hit = 0;
            int side = 0;

            float rayLength = 0.0;

            int stepX = 0;
            int stepY = 0;

            float cosRayAngle = cos(rayAngle);
            float sinRayAngle = sin(rayAngle);
            float tanRayAngle = tan(rayAngle);

            int squareX = floor(posX / wallWidth);
            int squareY = floor(posY / wallWidth);

            if (rayAngle < deg90 || rayAngle > deg270)
                stepX = 1;
            else if (rayAngle > deg90 && rayAngle < deg270)
                stepX = -1;

            if (rayAngle < deg180)
                stepY = 1;
            else if (rayAngle > deg180)
                stepY = -1;

            float rayPosX = posX;
            float rayPosY = posY;

            while (hit == 0 && rayPosX < mapWidth * wallWidth && rayPosY < mapHeight * wallWidth && rayPosX > 0 && rayPosY > 0)
            {
                float nearestIntersectX = stepX == 1 ? (squareX + stepX) * wallWidth : squareX * wallWidth;
                float nearestIntersectY = stepY == 1 ? (squareY + stepY) * wallWidth : squareY * wallWidth;

                float tryDeltaRayLengthX = (nearestIntersectX - rayPosX) / cosRayAngle;
                float tryDeltaRayLengthY = (nearestIntersectY - rayPosY) / sinRayAngle;

                float deltaRayLengthX = !isnan(tryDeltaRayLengthX) ? tryDeltaRayLengthX : 9999999999999.0;
                float deltaRayLengthY = !isnan(tryDeltaRayLengthY) ? tryDeltaRayLengthY : 9999999999999.0;

                if (deltaRayLengthX < deltaRayLengthY)
                {
                    squareX += stepX;
                    rayPosY += (nearestIntersectX - rayPosX) * tanRayAngle;
                    rayPosX = nearestIntersectX;

                    rayLength += deltaRayLengthX;

                    if (mapSquare(squareX, squareY, worldMap) > 0)
                    {
                        hit = 1;
                        side = -1;
                    }
                }
                else
                {
                    squareY += stepY;
                    rayPosX += (nearestIntersectY - rayPosY) / tanRayAngle;
                    rayPosY = nearestIntersectY;

                    rayLength += deltaRayLengthY;

                    if (mapSquare(squareX, squareY, worldMap) > 0)
                    {
                        hit = 1;
                        side = 1;
                    }
                }
            }

            rayLength = rayLength * cos(deltaRayAngle);

            int projectedWallHeight = side != 0 ? wallHeight * distanceToProjectionPlane / rayLength : 0;

            int drawStart = -projectedWallHeight / 2 + h / 2;
            if(drawStart < 0)
                drawStart = 0;

            int drawEnd = drawStart + projectedWallHeight;

            sf::Color color = getColor(squareX, squareY, worldMap);

            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(1, projectedWallHeight));
            rectangle.setFillColor(color);
            rectangle.setPosition(x, drawStart);
            window.draw(rectangle);

            for (int z = drawEnd + 1; z < h; z++)
            {
                float distanceEyeToRowPj = z - posZ;

                float distanceFeetToP =  distanceToProjectionPlane * playerHeight / cos(deltaRayAngle) / distanceEyeToRowPj;

                float floorPosX = posX + distanceFeetToP * cosRayAngle;
                float floorPosY = posY + distanceFeetToP * sinRayAngle;

                int floorSquareX = floor(floorPosX / wallWidth);
                int floorSquareY = floor(floorPosY / wallWidth);

                color = getColor(floorSquareX, floorSquareY, floorMap);

                ground.setPixel(x, z - posZ, color);
            }

            deltaRayAngle += degInterval;
        }

        sf::Texture floorTexture;
        floorTexture.loadFromImage(ground);
        sf::Sprite sprite(floorTexture);
        sprite.setPosition(sf::Vector2f(0.f, posZ));

        window.draw(sprite);

        window.display();

        oldTime = time;
        time = std::chrono::system_clock::now();
        std::chrono::duration<float> frameTime = time - oldTime;
        float elapsedTime = frameTime.count();
        std::cout<<(1.0 / elapsedTime)<<std::endl;

        float movementSpeed  = 60.0f * elapsedTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                //case sf::Event::MouseMoveEvent::x:
                //    currentMousePosition = sf::Mouse::getPosition();
                //    std::cout<<currentMousePosition.x<<std::endl;
                //    deltaMouseMovement = currentMousePosition.x - initialMousePosition.x;
                //    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code)
                    {
                        case sf::Keyboard::Left:
                            straf = -1;
                            break;
                        case sf::Keyboard::Right:
                            straf = 1;
                            break;
                        case sf::Keyboard::Up:
                            mov = 1;
                            break;
                        case sf::Keyboard::Down:
                            mov = -1;
                            break;
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch(event.key.code)
                    {
                        case sf::Keyboard::Left:
                            straf = 0;
                            break;
                        case sf::Keyboard::Right:
                            straf = 0;
                            break;
                        case sf::Keyboard::Up:
                            mov = 0;
                            break;
                        case sf::Keyboard::Down:
                            mov = 0;
                            break;
                    }
                    break;
            }
        }

        posX += mov * movementSpeed * cos(angle);
        posY += mov * movementSpeed * sin(angle);

        //posX += straf * movementSpeed * cos(angle + deg90);
        //posY += straf * movementSpeed * sin(angle + deg90);

        //angle += rot * deltaMouseMovement * rotationSpeed;

    }

    return 0;
}
