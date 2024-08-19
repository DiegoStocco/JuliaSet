#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Window.hpp>
#include <iostream>
#include <complex>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

void saveWindowToFile(const sf::Window &window, const std::string &path) {
		sf::Texture tex;
		tex.create(window.getSize().x, window.getSize().y);
		tex.update(window);
		tex.copyToImage().saveToFile("frattalo.png");
}

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
    // Create the texture that will be rendered by the shader
	  sf::RenderTexture render_tex;
		render_tex.create(renderWidth, renderHeight);

	  // Rectangle needed to draw on texture
    sf::RectangleShape tex_rec(sf::Vector2f(renderWidth, renderHeight));

		// sprite used to draw the texture on screen
		sf::Rect<float> screen(0, 0, width, height);
		sf::Sprite screen_sprite;
		screen_sprite.setTexture(render_tex.getTexture());

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
					saveWindowToFile(window, "frattalo.png");
				}
        if(a >= 2*M_PI)a = 0;
        else a += M_PI/200;
        
        std::complex o = std::exp(std::complex<float>(0, a));
        c.x = o.real();
        c.y = o.imag();

				render_tex.clear();

				// Set shaders variables
				render_julia.setUniform("u_screenSize", screen.getSize());
				render_julia.setUniform("u_constant", c);
				render_julia.setUniform("u_maxIterCount", maxIterCount);

				// Fill the texture with Julia using the shader
				render_tex.draw(tex_rec, &render_julia);
				render_tex.display();

        // Clear the window
        window.clear();
				

        // Draw the sprite, which contains the texture
		    screen_sprite.setScale(sf::Vector2((float)width/renderWidth , (float)height/renderHeight));
        window.draw(screen_sprite);
        // Display the contents of the window
        window.display();
    }
    
    return 0;
}

