#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <complex>
#include <iostream>

int main() {
    unsigned int width = 800;
    unsigned int height = 800;
    unsigned int renderWidth = 800;
    unsigned int renderHeight = 800;
    sf::Vector2f c(0,0);
    const int maxIterCount = 80;
    
    // Create the window
    sf::RenderWindow window(sf::VideoMode(width, height), "Julia's Set'");
    window.setVerticalSyncEnabled(true);
    // Create an image and a texture to render the pixels
    sf::Rect<float> screen(0, 0, width, height);

		// Load the shader
		sf::Shader render_julia;
		if(!render_julia.loadFromFile("shaders/render_julia.glsl", sf::Shader::Fragment)) {
			std::cerr << "Unable to load shader" << std::endl;
		}

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
								screen.width = width;
								screen.height = height;
                window.setView(sf::View(screen));
            }
        }
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
					sf::Texture tex;
					tex.create(width, height);
					tex.update(window);
					tex.copyToImage().saveToFile("frattalo.png");
				}
        if(a >= 2*M_PI)a = 0;
        else a += M_PI/200;
        
        std::complex o = std::exp(std::complex<float>(0, a));
        c.x = o.real();
        c.y = o.imag();

        // Clear the window
        window.clear();
				
				// Set shaders variables
				render_julia.setUniform("u_screenSize", screen.getSize());
				render_julia.setUniform("u_constant", c);
				render_julia.setUniform("u_maxIterCount", maxIterCount);

        // Draw the rectangle, which is filled by the shader
        window.draw(sf::RectangleShape(screen.getSize()), &render_julia);
        // Display the contents of the window
        window.display();
        //image.saveToFile("frat.png");
    }
    
    return 0;
}

