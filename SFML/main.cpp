#include <SFML/Graphics.hpp>
#include <time.h>
#include "Menuu.h"
#include <iostream>
#include <stdlib.h>
#include <sstream>
using namespace std;
using namespace sf;



int page_number;

const int R = 20;
const int C = 8;

int field[R][C] = { 0 };

struct Point
{
    int x, y;
} a[4], b[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

bool check()
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= C || a[i].y >= R) return 0;
        else if (field[a[i].y][a[i].x]) return 0;

    return 1;
};


int main()
{

    RenderWindow window(VideoMode(600, 600), "Tetris Menu", sf::Style::Close);
    Menuu menu(600, 600);


    while (true) {

        while (window.isOpen())
        {
            Event evnt;
            while (window.pollEvent(evnt))
            {
                if (evnt.type == Event::Closed)
                    window.close();

                if (evnt.type == Event::KeyReleased)
                {
                    if (evnt.key.code == Keyboard::Up)
                    {
                        menu.moveUp();
                        break;
                    }

                    if (evnt.key.code == Keyboard::Down)
                    {
                        menu.moveDown();
                        break;
                    }

                    // Choose Page/Choice
                    if (evnt.key.code == Keyboard::Enter)
                    {
                        
                        if (menu.mainMenuPressed() == 0)
                        {
                            window.close();
                            page_number = 0;
                        }
                        if (menu.mainMenuPressed() == 1)
                        {
                            window.close();
                            page_number = 1;
                        }
                        if (menu.mainMenuPressed() == 2)
                        {
                            window.close();
                            page_number = 2;
                        }
                    }
                }

           
            }

            Texture logo;
            logo.loadFromFile("images/tetris_logo.png");

            Sprite sp(logo);

            sp.setScale(.3f, .3f);

            window.clear(sf::Color::Black);
            menu.draw(window);
            window.draw(sp);
            window.display();

        }
        if (page_number == 0) // play
        {
            srand(time(0));

            RenderWindow window(VideoMode(320, 480), "Tetris Game");

            Font myFont;
            if (!myFont.loadFromFile("kimonoki.ttf")) {}


            Text score;
            score.setFont(myFont);
            score.setFillColor(Color::Green);
            score.setStyle(Text::Bold);
            score.setString("Score : ");
            score.setCharacterSize(10);
            score.setPosition(200, 60);

            Text scoreCurrent;
            scoreCurrent.setFont(myFont);
            scoreCurrent.setFillColor(Color::Green);
            scoreCurrent.setStyle(Text::Bold);
            scoreCurrent.setCharacterSize(10);
            scoreCurrent.setPosition(270, 60);

            RectangleShape rectangle(Vector2f(120.f, 50.f));
            rectangle.setFillColor(Color::Blue);
            rectangle.setOutlineColor(Color::Green);
            rectangle.setOutlineThickness(5);
            rectangle.setPosition(185, 45);

            Texture t;
            t.loadFromFile("images/tiles.png");

            Sprite s(t);

            int dx = 0; bool rotate = 0; int colorNum = 1; int currentScore = 0;
            float timer = 0, delay = 0.3;

            Clock clock;


            a[0].x = 0, a[0].y = 1;
            a[1].x = 1, a[1].y = 1;
            a[2].x = 1, a[2].y = 2;
            a[3].x = 1, a[3].y = 3;

   
            while (window.isOpen())
            {
                float time = clock.getElapsedTime().asSeconds();
                clock.restart();
                timer += time;
               
                sf::Event evnt;
                while (window.pollEvent(evnt))
                {
                   
                    if (evnt.type == sf::Event::Closed)
                        window.close();


                    if (evnt.type == Event::KeyPressed)
                        if (evnt.key.code == Keyboard::Up) rotate = true;
                        else if (evnt.key.code == Keyboard::Left) dx = -1;
                        else if (evnt.key.code == Keyboard::Right) dx = 1;

                    if (evnt.type == Event::KeyReleased)
                        if (evnt.key.code == Keyboard::Down) {
                            currentScore += 1;
                            cout << currentScore << endl;
                            std::stringstream s;
                            s << currentScore;
                            scoreCurrent.setString(s.str());
                        }
                }

                if (Keyboard::isKeyPressed(Keyboard::Down))
                {
                    delay = 0.05;
                }
  

                // MOVEMENT
                for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
                if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

                // ROTATION
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
                    if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
                }

                // TICK
                if (timer > delay)
                {
                    for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

                    if (!check())
                    {
                        for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

                        colorNum = 1 + rand() % 7;
                        int n = rand() % 7;
                        for (int i = 0; i < 4; i++)
                        {
                            a[i].x = figures[n][i] % 2;
                            a[i].y = figures[n][i] / 2;
                        }
                    }

                    timer = 0;
                }

                // CHECK LINES
                int k = R - 1;
                for (int i = R - 1; i > 0; i--)
                {
                    int count = 0;
                    for (int j = 0; j < C; j++)
                    {
                        if (field[i][j]) count++;
                        field[k][j] = field[i][j];
                    }
                    if (count < C) k--;
                }

                dx = 0; rotate = 0; delay = 0.3;

                // DRAW
                window.clear(Color::Black);
                window.draw(rectangle);
                window.draw(score);
                window.draw(scoreCurrent);
                
   
                
                for (int i = 0; i < R; i++)
                    for (int j = 0; j < C; j++)
                    {
                        if (field[i][j] == 0) continue;
                        s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                        s.setPosition(j * 18, i * 18);
                        s.move(28, 31); //offset
                        window.draw(s);
                    }

                for (int i = 0; i < 4; i++)
                {
                    s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
                    s.setPosition(a[i].x * 18, a[i].y * 18);
                    s.move(28, 31);
                    window.draw(s);
      
                }

                s.setTextureRect(IntRect((colorNum * 18), 0, 18, 18));
                s.setPosition(230, 150);
                window.draw(s);


                window.display();


            }

        }
        if (page_number == 1) // options
        {

        }
        if (page_number == 2) // exit
        {
            break;
        }



    }

    return 0;
}
