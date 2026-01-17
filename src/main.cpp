#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

int main() {
  unsigned int width = 800;
  unsigned int height = 800;
  unsigned int renderWidth = 800;
  unsigned int renderHeight = 800;

  sf::Vector2f c(0, 0);
  const int maxIterCount = 80;

  // Create the window
  sf::RenderWindow window(sf::VideoMode({width, height}), "Julia's Set'");
  window.setVerticalSyncEnabled(true);
  // Create the texture that will be rendered by the shader
  sf::RenderTexture render_tex({renderWidth, renderHeight});

  // Rectangle needed to draw on texture
  sf::RectangleShape tex_rec(sf::Vector2f(renderWidth, renderHeight));

  // sprite used to draw the texture on screen
  sf::Rect<float> screen({0, 0}, {(float)width, (float)height});
  sf::Sprite screen_sprite(render_tex.getTexture());

  // Load the shader
  sf::Shader render_julia;
  if (!render_julia.loadFromFile("shaders/render_julia.glsl",
                                 sf::Shader::Type::Fragment)) {
    std::cerr << "Unable to load shader" << std::endl;
  }

  // Main loop
  float a = 0;
  while (window.isOpen()) {
    
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto *size = event->getIf<sf::Event::Resized>()) {
        // Update the window size
        screen.size = (sf::Vector2f)size->size;
        window.setView(sf::View(screen));
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        std::ignore = render_tex.getTexture().copyToImage().saveToFile("julia.png");
    }

    a += M_PI / 200;
    a = std::fmod(a, 2 * M_PI);

    // c = e^(ia)
    c = {cosf(a), sinf(a)};

    render_tex.clear();

    // Set shaders variables
    render_julia.setUniform("u_screenSize", tex_rec.getSize());
    render_julia.setUniform("u_constant", c);
    render_julia.setUniform("u_maxIterCount", maxIterCount);

    // Fill the texture with Julia using the shader
    render_tex.draw(tex_rec, &render_julia);
    render_tex.display();

    // Clear the window
    window.clear();

    // Draw the sprite, which contains the texture
    screen_sprite.setScale(
        sf::Vector2((float)width / renderWidth, (float)height / renderHeight));
    window.draw(screen_sprite);
    // Display the contents of the window
    window.display();
  }

  return 0;
}
