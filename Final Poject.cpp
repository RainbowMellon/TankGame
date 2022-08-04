#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "TankGame.h"

using namespace sf;


int main()
{
    TankGame game;

    bool gameStarted = false;

    RenderWindow window(VideoMode(1000, 500), "Bunker Blaster");
    window.setFramerateLimit(60);

    Music explosionSound;
    explosionSound.openFromFile("explosion.ogg");
    explosionSound.setPitch(1);
    explosionSound.setVolume(100);
    explosionSound.setLoop(false);

    CircleShape trail[16];
    CircleShape abScTr = game.getProjectile(); // Above Screen Tracker;
    CircleShape fireBall, lastPos1, lastPos2;
    fireBall.setFillColor(Color::Red);
    for (int i = 0; i < 15; i++)
    {
        trail[i].setRadius(1);
        trail[i].setFillColor(Color::Black);
    }
    lastPos1.setFillColor(game.getPlayerBody(1).getFillColor());
    lastPos1.setRadius(1);
    lastPos1.setPosition(10, 10);
    lastPos2.setFillColor(game.getPlayerBody(2).getFillColor());
    lastPos2.setRadius(1);
    lastPos2.setPosition(10, 10);

    VertexArray explosionRadiusLine;
    explosionRadiusLine.setPrimitiveType(Lines);
    explosionRadiusLine.resize(2);
    explosionRadiusLine[0].position = Vector2f(100, 100);
    explosionRadiusLine[1].position = Vector2f(200, 100);

    std::string tempScore1 = "0", tempScore2 = "0", windSpeed = "0";

    Texture backTexture, windLeftTexture, windRightTexture;
    backTexture.loadFromFile("background.png");
    windLeftTexture.loadFromFile("Wind Left.png");
    windRightTexture.loadFromFile("Wind Right.png");

    Font font;
    font.loadFromFile("arial.ttf");

    Text text1("P1: 0", font), text2("P2: 0", font), windText("1 mph", font), startText("Type a level number(0-9)", font);
    text1.setCharacterSize(30);
    text1.setStyle(Text::Bold);
    text1.setFillColor(game.getPlayerBody(1).getFillColor());
    text1.setOutlineColor(Color::Black);
    text1.setOutlineThickness(1.f);
    text1.setPosition(20, 20);

    text2.setCharacterSize(30);
    text2.setStyle(Text::Bold);
    text2.setFillColor(game.getPlayerBody(2).getFillColor());
    text2.setOutlineColor(Color::Black);
    text2.setOutlineThickness(1.f);
    text2.setPosition(880, 20);
    //std::cout << text1.getString().getSize() << std::endl;

    windText.setCharacterSize(20);
    windText.setStyle(sf::Text::Bold);
    windText.setFillColor(Color(63, 166, 15));
    windText.setOutlineColor(Color::Black);
    windText.setOutlineThickness(1.f);
    windText.setPosition(495, 60);

    startText.setCharacterSize(20);
    startText.setFillColor(Color::Black);
    startText.setOutlineColor(Color::Black);
    startText.setOutlineThickness(1.f);
    startText.setPosition(390, 200);

    Sprite background, windArrow;
    background.setPosition(0, 0);
    background.setTexture(backTexture);
    windArrow.setPosition(500, 20);
    windArrow.setOrigin(19, 12);
    if (game.getWind().x < 0)
        windArrow.setTexture(windLeftTexture);
    else if (game.getWind().x > 0)
        windArrow.setTexture(windRightTexture);
    windSpeed = std::to_string(int(game.getWind().x * 10));
    windText.setString(windSpeed + " mph");

    Mouse mouse;

    Vector2i mSave1, mSave2; //mouse save 1 and 2
    mSave1.x = -1;
    mSave2.x = -1;

    int playerTurn = 1, frame = 1, explodeTimer = 0, player1Score = 0, player2Score = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            //used for pointing barrels
            if (event.type == Event::MouseMoved && gameStarted == true)
                game.setMousePosition(Vector2f(event.mouseMove.x, event.mouseMove.y));

            //change turn when mouse clicked
            if ((event.type == Event::MouseButtonPressed) && explodeTimer == 0 && game.getProjectileMomentum() == Vector2f(0, 0) && gameStarted == true)
            {
                std::cout << "Mouse pressed" << std::endl;
                game.fire(playerTurn);
                if (playerTurn == 1)
                {
                    mSave1 = mouse.getPosition();
                    if (mSave2.x > 0)
                        mouse.setPosition(mSave2);
                    playerTurn = 2;
                }
                else if (playerTurn == 2)
                {
                    mSave2 = mouse.getPosition();
                    if (mSave1.x > 0)
                        mouse.setPosition(mSave1);
                    playerTurn = 1;
                }
            }

            //set the world gen number 0-9
            if ((event.type == Event::KeyPressed) && gameStarted == false)
            {
                if (event.key.code == Keyboard::Num0)
                    game.setLevel(0);
                else if (event.key.code == Keyboard::Num1)
                    game.setLevel(1);
                else if (event.key.code == Keyboard::Num2)
                    game.setLevel(2);
                else if (event.key.code == Keyboard::Num2)
                    game.setLevel(3);
                else if (event.key.code == Keyboard::Num2)
                    game.setLevel(4);
                else if (event.key.code == Keyboard::Num2)
                    game.setLevel(5);
                else if (event.key.code == Keyboard::Num2)
                    game.setLevel(6);
                else if (event.key.code == Keyboard::Num2)
                    game.setLevel(7);
                else if (event.key.code == Keyboard::Num2)
                    game.setLevel(8);
                else if (event.key.code == Keyboard::Num2)
                    game.setLevel(9);             
                else
                    game.setLevel(14);

                gameStarted = true;
            }
        }

        lastPos1.setPosition(Vector2f(mSave1.x - 469, mSave1.y - 322));
        lastPos2.setPosition(Vector2f(mSave2.x - 469, mSave2.y - 322));
        //std::cout << mSave1.x << " " << mSave2.y << std::endl;

        //Add gravity and wind to the momentum vectors of apropriate game objects
        if (frame % 2 == 0)
            game.applyNature();

        //move the game objects
        if (frame % 2 == 1 && game.isLanded() == false)
        {
            game.applyMomentum();
            fireBall.setRadius(0);
        }
        
        //explode
        else if (game.isLanded() == true && explodeTimer == 0)
        { 
            explodeTimer = 60;
            fireBall.setPosition(game.getProjectile().getPosition() + Vector2f(game.getProjectileMomentum().x / 2, game.getProjectileMomentum().y / 2));
            explosionSound.play();
        }
        fireBall.setRadius(explodeTimer);
        fireBall.setOrigin(explodeTimer, explodeTimer);
        
        //I apparently made other parts of the code depend on the lines updating 
        //this is needed if 'window.draw(game.getDebugLine(playerTurn));' is commented out
        game.updateDebugLines(playerTurn);

        //move the barrels to the tanks
        game.updateBarrels(playerTurn);

        //direct hit on player?
        if (game.isPlayerHit(2) == 1 && explodeTimer == 0)
        {
            player1Score += 10;
            tempScore1 = std::to_string(player1Score);
            text1.setString("P1: " + tempScore1);
        }
        if (game.isPlayerHit(1) == 1 && explodeTimer == 0)
        {
            player2Score += 10;
            tempScore2 = std::to_string(player2Score);
            text2.setString("P2: " + tempScore2);
        }
        //explosion hit on player?
        if (playerTurn == 1 && explodeTimer == 40 && game.getPlayerBody(playerTurn).getGlobalBounds().contains
                (game.explosionHit(playerTurn, fireBall.getPosition(), game.getPlayerBody(2).getPosition())) == 1)
        {
            player1Score += 10;
            tempScore1 = std::to_string(player1Score);
            text1.setString("P1: " + tempScore1);
        }
        if (playerTurn == 2 && explodeTimer == 40 && game.getPlayerBody(playerTurn).getGlobalBounds().contains
                (game.explosionHit(playerTurn, fireBall.getPosition(), game.getPlayerBody(1).getPosition())) == 1)
        {
            player2Score += 10;
            tempScore2 = std::to_string(player2Score);
            text2.setString("P2: " + tempScore2);
        }

        window.clear();
        
        window.draw(background);

        //only draw when the game has started
        if (gameStarted == true)
        {
            //the projectile trail
            for (int i = 0; i < 15; i++)
                window.draw(trail[i]);
            if (frame % 4 == 0)
            {
                trail[frame / 4] = game.getProjectile();
            }
            //the above screen tracker
            if (game.getProjectile().getPosition().y < 0)
            {
                abScTr.setPosition(game.getProjectile().getPosition().x, 1);
                abScTr.setRadius(game.getProjectile().getRadius() - 1);
                window.draw(abScTr);
            }
            window.draw(game.getProjectile());
            window.draw(windArrow);
            window.draw(game.getPlayerBarrel(1));
            window.draw(game.getPlayerBarrel(2));
            window.draw(game.getPlayerBody(1));
            window.draw(game.getPlayerBody(2));
            window.draw(fireBall);
            window.draw(game.getTerrain());
            //window.draw(game.getDebugLine(playerTurn));
            //window.draw(explosionRadiusLine);
            window.draw(text1);
            window.draw(text2);
            window.draw(windText);
            window.draw(lastPos1);
            window.draw(lastPos2);
        }
        if (gameStarted == false)
        {
            window.draw(startText);
        }

        window.display();

        if (frame == 60)
            frame = 1;
        else
            frame++;
        //std::cout << game.getWind().x << std::endl;
        if (explodeTimer > 1)
            explodeTimer--;
        else if (explodeTimer == 1)
        {
            game.resetProjectile(Vector2f(-100, 50));
            explodeTimer--;
            for (int i = 0; i < 15; i++)
                trail[i].setPosition(game.getProjectile().getPosition());
        }
    }
    return 0;
}