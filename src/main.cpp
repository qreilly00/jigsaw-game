//include classes and headers
#include "../inc/common.hpp"
#include "TimeUtil.hpp"

int getRandomInt(int x, int y);

int main() {
    sf::RenderWindow window(sf::VideoMode(1500 , 1000), "SFML works!");
    window.setFramerateLimit(60);

    sf::Texture texture;
    if (!texture.loadFromFile("../scenery.jpg")) {
        std::cout << "Error with texture!" << std::endl;
    }

    sf::RectangleShape image;
    image.setTexture(&texture);
    image.setPosition(50, 50);
    image.setSize(sf::Vector2f(image.getTexture()->getSize().x, image.getTexture()->getSize().y));

    int pieceSize = 50;
    int pieceCountX = image.getTexture()->getSize().x / pieceSize;
    int pieceCountY = image.getTexture()->getSize().y / pieceSize;
    //std::cout << "x: " << pieceCountX << " y: " << pieceCountY << std::endl;

    sf::RectangleShape imageCover(sf::Vector2f(pieceCountX * pieceSize, pieceCountY * pieceSize));
    imageCover.setFillColor(sf::Color::Black);
    imageCover.setPosition(image.getPosition().x + (image.getSize().x - imageCover.getSize().x) / 2, image.getPosition().y + (image.getSize().y - imageCover.getSize().y) / 2);

    sf::RectangleShape tmp;

    std::vector<sf::RectangleShape> gridLines;
    tmp.setFillColor(sf::Color::White);
    tmp.setSize(sf::Vector2f(pieceCountX * pieceSize, 1));
    tmp.setPosition(imageCover.getPosition());

    for(int i = 0; i < pieceCountY; i++) {
        gridLines.push_back(tmp);
        tmp.move(0, pieceSize);
    }

    tmp.setSize(sf::Vector2f(1, pieceCountY * pieceSize));
    tmp.setPosition(imageCover.getPosition());

    for(int i = 0; i < pieceCountX; i++) {
        gridLines.push_back(tmp);
        tmp.move(pieceSize, 0);
    }

    std::vector<sf::RectangleShape> jigSawPieces;
    tmp.setSize(sf::Vector2f(pieceSize, pieceSize));
    tmp.setPosition(image.getPosition().x + image.getSize().x + 50, imageCover.getPosition().y);
    tmp.setTexture(&texture);
    tmp.setTextureRect(sf::IntRect(imageCover.getPosition().x - image.getPosition().x, imageCover.getPosition().y - image.getPosition().y, pieceSize, pieceSize));

    for(int i = 0; i < pieceCountY; i++) {
        for(int i = 0; i < pieceCountX; i++) {
            jigSawPieces.push_back(tmp);
            tmp.move(pieceSize, 0);
            tmp.setTextureRect(sf::IntRect(tmp.getTextureRect().left + pieceSize, tmp.getTextureRect().top, pieceSize, pieceSize));
        }
        tmp.move(-(pieceSize * pieceCountX), pieceSize);
        tmp.setTextureRect(sf::IntRect(imageCover.getPosition().x - image.getPosition().x, imageCover.getPosition().y - image.getPosition().y + (pieceSize * (i + 1)), pieceSize, pieceSize));
    }

    std::vector<int> randomNumbers;
    int tmpRng = getRandomInt(0, jigSawPieces.size() - 1);
    randomNumbers.push_back(tmpRng);

    for(auto x : jigSawPieces) {
        tmpRng = getRandomInt(0, jigSawPieces.size() - 1);
        bool noDupes = true;
        bool exitDupes = false;

        while(exitDupes == false) {
            for(int i = 0; i < randomNumbers.size(); i++) {
                if(tmpRng == randomNumbers[i]) {
                    //tmpRng = getRandomInt(0, jigSawPieces.size() - 1);
                    noDupes = false;
                }
            }

            if(noDupes == true) {
                randomNumbers.push_back(tmpRng);
                exitDupes = true;
            } else {
                tmpRng = getRandomInt(0, jigSawPieces.size() - 1);
                noDupes = true;
            }
        }
    }

    std::cout << randomNumbers.size() << ", " << jigSawPieces.size();

    std::vector<sf::RectangleShape> tmpPieces;
    tmpPieces = jigSawPieces;

    /*for(int i = 0; i < jigSawPieces.size(); i++) {
        tmpPieces[randomNumbers[i]] = jigSawPieces[i];
    }*/

    //jigSawPieces = tmpPieces;

    TimeUtil tu;
    tu.setTime();
    int totalTime = 0;

    while (window.isOpen()) {
        tu.setTime();
        totalTime += tu.getTime();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
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
