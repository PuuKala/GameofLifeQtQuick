#include "gameoflife.h"

/**
 * @brief Game of life constructor, initializes state to 1x1 black image.
 */
GameOfLife::GameOfLife() : QQuickImageProvider(QQuickImageProvider::Image)
{
    _game_state_image.fill(0);
}

/**
 * @brief Game of life function, gets next state from image request
 * @details Parses the ID string and acts accordingly. First char 'r' is run, 's' is change size, 'p' is set point. Additional parameters such as size in numbers or point coordinates come immediately after the first command character.
 * @param id Request string to parse
 * @param size Handled by Qt, not used here
 * @param requestedSize Handled by Qt, not used here
 * @return Monochrome image of the game state
 */
QImage GameOfLife::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (!id.isEmpty()){

        // Run the game, id string format "r", no parameters needed or handled
        if (id.at(0) == 'r')
            _run_game();

        // Change size, id string format "snnn", with 'n' as a number
        else if(id.at(0) == 's')
        {
            _game_state_image = QImage(id.mid(1).toInt(), id.mid(1).toInt(), QImage::Format_Mono);
            _game_state_image.fill(0);
        }

        // Set a point, id string format "pnnnxmmm" with 'n' and 'm' as numbers
        else if(id.at(0) == 'p')
        {
            QStringList point = id.split('x');

            // GUI coordinates scaled to _game_state_image size coordinates
            int x = point.at(0).mid(1).toInt()*(_game_state_image.width()/_GUI_image_size);
            int y = point.at(1).toInt()*(_game_state_image.height()/_GUI_image_size);

            if (_game_state_image.pixel(x, y) == qRgb(0xff, 0xff, 0xff))
                _game_state_image.setPixel(x, y, 0);
            else
                _game_state_image.setPixel(x, y, 1);
        }
    }
    return _game_state_image;
}

///////////////////////////////////
//// PRIVATE FROM HERE ONWARDS ////
///////////////////////////////////

// Run game function separated from the "main" requestImage function for clarity
inline void GameOfLife::_run_game()
{
    // We'll need to raise/die all at the same time, thus just iterating through while making pixels alive doesn't work.
    // Instead, we need to store which pixel raises and which dies.
    std::vector<std::pair<unsigned, unsigned>> to_live;
    std::vector<std::pair<unsigned, unsigned>> to_die;

    for (unsigned long long pixel_x = 0; pixel_x < _game_state_image.width(); ++pixel_x)
    {
        for (unsigned long long pixel_y = 0; pixel_y < _game_state_image.height(); ++pixel_y)
        {
            unsigned char neighbours = 0;
            for (int neighbour_x = -1; neighbour_x <= 1; ++neighbour_x) {

                // REFACTOR IDEA: We could just remove out of bounds checks and ignore the warning prints. I think it would be more computationally efficient way.
                // It's not the cleanest solution though due to a flood of out of bounds warnings from the QImage class.

                // Skipping out of bounds neighbours
                if (pixel_x == 0 && neighbour_x == -1)
                    continue;
                else if (pixel_x == _game_state_image.width() - 1 && neighbour_x == 1)
                    continue;

                for (int neighbour_y = -1; neighbour_y <= 1; ++neighbour_y) {

                    // Skipping out of bounds neighbours
                    if (pixel_y == 0 && neighbour_y == -1)
                        continue;
                    if (pixel_y == _game_state_image.height()-1 && neighbour_y == 1)
                        continue;

                    // Ignore self
                    if (neighbour_x == 0 && neighbour_y == 0)
                        continue;

                    if (_game_state_image.pixel(pixel_x + neighbour_x, pixel_y + neighbour_y) == qRgb(0xff, 0xff, 0xff))
                        ++neighbours;
                }
            }

            // Check if alive or dead and list it to be killed/repopulated depending on neighbour count
            if (_game_state_image.pixel(pixel_x, pixel_y) == qRgb(0xff,0xff,0xff))
            {
                if (neighbours < 2 || neighbours > 3)
                    to_die.push_back(std::pair<unsigned, unsigned>(pixel_x, pixel_y));
            }
            else
            {
                if (neighbours == 3)
                    to_live.push_back(std::pair<unsigned, unsigned>(pixel_x, pixel_y));
            }
        }
    }

    // Do the killing/repopulating
    for(const std::pair<unsigned, unsigned> coord : to_live)
        _game_state_image.setPixel(coord.first, coord.second, 1);
    for(const std::pair<unsigned, unsigned> coord : to_die)
        _game_state_image.setPixel(coord.first, coord.second, 0);
}
