#include <SFML/Graphics.hpp>
#include <random>
#include <complex>
#include <cmath>

//Funzione iterazione sigola
sf::Vector2f iter(sf::Vector2f in, sf::Vector2f constant){
    float re = in.x, im = in.y;
    sf::Vector2f out(0,0);
    out.x = re*re - im*im + constant.x;
    out.y = re*im * 2 + constant.y;
    return out;
}

int squredMod(sf::Vector2f v){
    return v.x * v.x + v.y * v.y;
}

//Funzione 
int iterCount(sf::Vector2f start, int maxIterCount, sf::Vector2f constant){
    int iterCount = 0;
    sf::Vector2f curr = start;
    while(iterCount < maxIterCount && squredMod(curr) < 4){
        curr = iter(curr, constant);
        iterCount++;
    }
    return iterCount;
}

float lerp(float a, float b, float t){
    return a + t * (b - a);
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main() {
    // Initial resolution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(-0.8, 0.8);

    unsigned int width = 1000;
    unsigned int height = 1000;
    unsigned int renderWidth = 1000;
    unsigned int renderHeight = 1000;
    sf::Vector2f c(0,0);
    const int maxIterCount = 100;
    
    // Create the window
    sf::RenderWindow window(sf::VideoMode(width, height), "Pixel-by-Pixel Rendering");
    window.setFramerateLimit(144);
    // Create an image and a texture to render the pixels
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    auto updateImageAndTexture = [&]() {
        // Resize the image and texture
        image.create(renderWidth, renderHeight, sf::Color::Black);
        texture.create(renderWidth, renderHeight);
        texture.update(image);
        sprite.setTexture(texture);
    };

    // Initial setup
    updateImageAndTexture();

    // Function to set a pixel color
    auto setPixel = [&](unsigned int x, unsigned int y, sf::Color color) {
        if (x < renderWidth && y < renderHeight) {
            image.setPixel(x, y, color);
        }
    };

    
    // Main loop
    float a = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                // Update the window size
                width = event.size.width;
                height = event.size.height;
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                // Update the image, texture, and sprite
                updateImageAndTexture();
            }
        }
        if(a >= 2*M_PI)a = 0;
        else a += M_PI/100;
        
        std::complex o = std::exp(std::complex<float>(0, a));
        c.x = o.real();
        c.y = o.imag();
        for (unsigned int y = 0; y < renderHeight; ++y) {
            for (unsigned int x = 0; x < renderWidth; ++x) {
                float re = map(x,0,renderWidth, -1,1);
                float im = map(y,0,renderHeight, -1,1);
                int itCount = iterCount(sf::Vector2f(re,im),maxIterCount,c);
                int col = map(itCount, 0, maxIterCount,0,255);
                setPixel(x, y, sf::Color(col,col,col));
            }
        }

        // Update the texture with the new pixel data
        texture.update(image);

        // Clear the window
        window.clear();
        // Draw the sprite (which uses the updated texture)
        sprite.setScale((float)width/renderWidth,(float)height/renderHeight);
        window.draw(sprite);
        // Display the contents of the window
        window.display();
        //image.saveToFile("frat.png");
    }
    
    return 0;
}

