#include "quadtree.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height),"Quadtree");
    quadtree tree;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();

                }
                case sf::Event::MouseButtonPressed:
                {
                    tree.insert(event.mouseButton.x, event.mouseButton.y);
                }
            }
        }
        window.clear();
        if(!tree.is_empty()) tree.print_tree(window);
        window.display();
    }

    return 0;
}
