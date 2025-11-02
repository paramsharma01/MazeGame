#include <SFML/Graphics.hpp>
#include "../include/Maze.hpp"
#include "../include/Player.hpp"
#include "../include/Timer.hpp"
#include <iostream>

int main()
{
    int rows = 25;
    int cols = 25;
    int cellSize = 25;
    int marginX = 50;
    int marginY = 50;
    float time = 30.0f;

    sf::RenderWindow window(
        sf::VideoMode(cols * cellSize + marginX * 2, rows * cellSize + marginY * 2),
        "Maze Game");
    window.setFramerateLimit(60);

    Maze maze(rows, cols, cellSize, marginX, marginY);
    Player player(0, 0, cellSize, marginX);
    Timer timer(time); // ⏰ Countdown timer

    // 🖋️ Load font
    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf"))
    {
        std::cerr << "Error loading font!\n";
    }

    // 🕓 Timer Text
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(20);
    timerText.setFillColor(sf::Color::Green);
    timerText.setStyle(sf::Text::Bold);

    // 🟩 Timer box setup (top-right corner)
    sf::RectangleShape timerBox;
    timerBox.setSize({100.f, 45.f});
    timerBox.setFillColor(sf::Color(40, 40, 40, 220)); // translucent dark box
    timerBox.setOutlineThickness(2);
    timerBox.setOutlineColor(sf::Color(100, 100, 100));
    timerBox.setPosition(window.getSize().x - timerBox.getSize().x - marginX, 0);

    // 🎮 Main Game Loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
                player.handleInput(event, maze.getGrid());
        }

        // 🕒 Update Timer
        float remaining = timer.getRemainingTime();
        timerText.setString("Time: " + std::to_string(static_cast<int>(remaining)) + "s");

        // 🟢🟠🔴 Change color depending on time left
        if (remaining > (time / 2))
            timerText.setFillColor(sf::Color::Green);
        else if (remaining > (time / 4))
            timerText.setFillColor(sf::Color(255, 165, 0)); // Orange
        else
            timerText.setFillColor(sf::Color::Red);

        // Center timer text inside box
        sf::FloatRect textBounds = timerText.getLocalBounds();
        timerText.setOrigin(textBounds.width / 2, textBounds.height / 2);
        timerText.setPosition(
            timerBox.getPosition().x + timerBox.getSize().x / 2,
            timerBox.getPosition().y + timerBox.getSize().y / 2 + 2);

        // 🏁 Check WIN condition
        sf::Vector2i playerPos = player.getPosition();
        if (playerPos.x == cols - 1 && playerPos.y == rows - 1)
        {
            sf::Text winText("🎉 YOU WIN! 🎉", font, 60);
            winText.setFillColor(sf::Color::Green);
            winText.setStyle(sf::Text::Bold);
            sf::FloatRect textRect = winText.getLocalBounds();
            winText.setOrigin(textRect.width / 2, textRect.height / 2);
            winText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

            window.clear(sf::Color(30, 30, 30));
            maze.draw(window);
            player.draw(window);
            window.draw(winText);
            window.display();

            sf::sleep(sf::seconds(2));
            window.close();
        }

        // 🕰️ TIME’S UP condition
        if (timer.isTimeUp())
        {
            sf::Text loseText("⏰ TIME'S UP! ⏰", font, 60);
            loseText.setFillColor(sf::Color::Red);
            loseText.setStyle(sf::Text::Bold);
            sf::FloatRect textRect = loseText.getLocalBounds();
            loseText.setOrigin(textRect.width / 2, textRect.height / 2);
            loseText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

            window.clear(sf::Color(30, 30, 30));
            maze.draw(window);
            player.draw(window);
            window.draw(loseText);
            window.display();

            sf::sleep(sf::seconds(2));
            window.close();

            // 🟦 Open new window showing the correct path
            sf::RenderWindow pathWindow(sf::VideoMode(cols * cellSize + marginX * 2, rows * cellSize + marginY * 2), "Correct Path");
            pathWindow.setFramerateLimit(60);

            // Create "Press any key to close" text
            sf::Text closeText("Press any key to close", font, 24);
            closeText.setFillColor(sf::Color(180, 180, 180));
            closeText.setStyle(sf::Text::Italic);
            sf::FloatRect closeRect = closeText.getLocalBounds();
            closeText.setOrigin(closeRect.width / 2, closeRect.height / 2);
            closeText.setPosition(pathWindow.getSize().x / 2, pathWindow.getSize().y - 40);

            // Get correct path
            auto path = maze.findPath();

            while (pathWindow.isOpen())
            {
                sf::Event e;
                while (pathWindow.pollEvent(e))
                {
                    if (e.type == sf::Event::Closed || e.type == sf::Event::KeyPressed)
                        pathWindow.close();
                }

                pathWindow.clear(sf::Color(20, 20, 20));
                maze.draw(pathWindow);

                // Draw correct path in dark blue
                for (auto &p : path)
                {
                    sf::RectangleShape cell(sf::Vector2f(cellSize - 4, cellSize - 4));
                    cell.setFillColor(sf::Color(0, 76, 153)); // Darker blue
                    cell.setPosition(p.x * cellSize + marginX + 2, p.y * cellSize + marginY + 2);
                    pathWindow.draw(cell);
                }

                pathWindow.draw(closeText);
                pathWindow.display();
            }
        }

        // 🎨 Draw everything
        window.clear(sf::Color(20, 20, 20)); // dark background
        maze.draw(window);
        player.draw(window);

        // 🟩 Draw timer box & text
        window.draw(timerBox);
        window.draw(timerText);

        window.display();
    }

    return 0;
}
