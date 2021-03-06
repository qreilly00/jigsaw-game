//include classes and headers
#include "../inc/common.hpp"
#include "TimeUtil.hpp"

int getRandomInt(int x, int y);

int main() {
    // Init and first image.
    sf::RenderWindow window(sf::VideoMode(1600 , 900), "SFML works!"/*, sf::Style::Fullscreen*/);
    //window.setFramerateLimit(30);

    sf::Texture texture;
    if (!texture.loadFromFile("../scenery.png")) {
        std::cout << "Error with texture!" << std::endl;
    }

    sf::RectangleShape image;
    image.setTexture(&texture);
    image.setPosition(50, 50);
    image.setSize(sf::Vector2f(image.getTexture()->getSize().x, image.getTexture()->getSize().y));

    int pieceSize = 100;
    int pieceCountX = image.getTexture()->getSize().x / pieceSize;
    int pieceCountY = image.getTexture()->getSize().y / pieceSize;
    //std::cout << "x: " << pieceCountX << " y: " << pieceCountY << std::endl;

    // Image cover (Black box over the original image).
    sf::RectangleShape imageCover(sf::Vector2f(pieceCountX * pieceSize, pieceCountY * pieceSize));
    imageCover.setFillColor(sf::Color::Black);
    imageCover.setPosition(image.getPosition().x + (image.getSize().x - imageCover.getSize().x) / 2, image.getPosition().y + (image.getSize().y - imageCover.getSize().y) / 2);

    // Create grid lines.
    std::vector<sf::RectangleShape> gridLines;
    sf::RectangleShape tmp;
    tmp.setFillColor(sf::Color::White);
    tmp.setSize(sf::Vector2f(pieceCountX * pieceSize, 1));
    tmp.setPosition(imageCover.getPosition());

    // Horizontal.
    for(int i = 0; i < pieceCountY; i++) {
        gridLines.push_back(tmp);
        tmp.move(0, pieceSize);
    }

    tmp.setSize(sf::Vector2f(1, pieceCountY * pieceSize));
    tmp.setPosition(imageCover.getPosition());

    // Vertical
    for(int i = 0; i < pieceCountX; i++) {
        gridLines.push_back(tmp);
        tmp.move(pieceSize, 0);
    }

    // Set up all jig saw pieces.
    std::vector<sf::RectangleShape> jigSawPieces;
    tmp.setSize(sf::Vector2f(pieceSize, pieceSize));
    tmp.setPosition(image.getPosition().x + image.getSize().x + 50, imageCover.getPosition().y);
    tmp.setTexture(&texture);
    tmp.setTextureRect(sf::IntRect(imageCover.getPosition().x - image.getPosition().x, imageCover.getPosition().y - image.getPosition().y, pieceSize, pieceSize));

    for(int i = 0; i < pieceCountY; i++) {
        for(int i = 0; i < pieceCountX; i++) {
            jigSawPieces.push_back(tmp);
            tmp.move(pieceSize + 10, 0);
            tmp.setTextureRect(sf::IntRect(tmp.getTextureRect().left + pieceSize, tmp.getTextureRect().top, pieceSize, pieceSize));
        }
        tmp.move(-(pieceSize * pieceCountX) - pieceCountX * 10, pieceSize + 10);
        tmp.setTextureRect(sf::IntRect(imageCover.getPosition().x - image.getPosition().x, imageCover.getPosition().y - image.getPosition().y + (pieceSize * (i + 1)), pieceSize, pieceSize));
    }

    // Random numbers for jig saw position switching stage zero.
    std::vector<int> randomNumbers;
    int tmpRng = getRandomInt(0, jigSawPieces.size() - 1);
    randomNumbers.push_back(tmpRng);

    for(auto x : jigSawPieces) {
        tmpRng = getRandomInt(0, jigSawPieces.size() - 1);
        bool noDupes = true;
        bool exitDupes = false;

        while(exitDupes == false && randomNumbers.size() != jigSawPieces.size()) {
            for(int i = 0; i < randomNumbers.size(); i++) {
                if(tmpRng == randomNumbers[i]) {
                    noDupes = false;
                }
            }

            if(noDupes == true) {
                randomNumbers.push_back(tmpRng);
                exitDupes = true;
                break;
            } else {
                tmpRng = getRandomInt(0, jigSawPieces.size() - 1);
                noDupes = true;
            }
        }
    }

    // Switch stage one.
    std::vector<sf::RectangleShape> tmpPieces = jigSawPieces;

    for(int i = 0; i < jigSawPieces.size(); i++) {
        tmpPieces[randomNumbers[i]].setPosition(jigSawPieces[i].getPosition());
    }

    // Switch stage two.
    jigSawPieces = tmpPieces;

    // Init time class.
    TimeUtil tu;
    tu.setTime();
    int totalTime = 0;

    // For mouse movement.
    bool isMoving = 0;
    int pieceToMove = 0;
    while (window.isOpen()) {
        tu.setTime();
        totalTime += tu.getTime();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if(isMoving == 0) {
                for(int i = 0; i < jigSawPieces.size(); i++) {
                    if(jigSawPieces[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
                        isMoving = 1;
                        pieceToMove = i;
                        continue;
                    }
                }
            } else {
                jigSawPieces[pieceToMove].setPosition(sf::Mouse::getPosition(window).x - (jigSawPieces[pieceToMove].getSize().x - jigSawPieces[pieceToMove].getSize().y / 2), sf::Mouse::getPosition(window).y - (jigSawPieces[pieceToMove].getSize().y - jigSawPieces[pieceToMove].getSize().y / 2));
                //std::cout << jigSawPieces[pieceToMove].getPosition().x << " " << jigSawPieces[pieceToMove].getPosition().y << std::endl;

                for(int i = 0; i < imageCover.getSize().x / pieceSize; i++) {
                    for(int j = 0; j < imageCover.getSize().x / pieceSize; j++) {
                        if(sf::IntRect(imageCover.getPosition().x + (j * pieceSize), imageCover.getPosition().y + (i * pieceSize), pieceSize, pieceSize).contains(sf::Mouse::getPosition(window))) {
                            jigSawPieces[pieceToMove].setPosition(imageCover.getPosition().x + (j * pieceSize), imageCover.getPosition().y + (i * pieceSize));
                        }
                    }
                }
            }
        } else {
            isMoving = 0;
        }

        // Controls.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        }

        window.clear(sf::Color::Black);
        window.draw(image);
        window.draw(imageCover);
        //window.draw(tmp);

        for(auto x : gridLines) {
            window.draw(x);
        }

        for(auto x : jigSawPieces) {
            window.draw(x);
        }

        window.display();
    }

    // Exit program.
    return 0;
}

int getRandomInt(int x, int y) {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(x, y);

    return distribution(generator);
}
