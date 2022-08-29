#include "gameoflife.h"
#include <QDebug>

/**
 * @brief Game of life constructor, initializes state to 1x1 black image.
 */
GameOfLife::GameOfLife() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    _game_state.fill(Qt::black);
}

/**
 * @brief Game of life function, gets next state from image request
 * @details Parses the ID string and acts accordingly. First char 'r' is run, 's' is change size, 'p' is set point. Additional parameters such as size in numbers or point coordinates come immediately after the first command character.
 * @param id
 * @param size
 * @param requestedSize
 * @return Pixel map of the game state
 */
QPixmap GameOfLife::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (!id.isEmpty()){
        // Run the game
        if (id.at(0) == 'r')
        {
            // To modify pixels in bitmap, we'll need to handle it as QImage.
            // REFACTOR TODO: Keep a QImage, not QBitmap in this class to avoid unnecessary conversions
            QImage game_image = _game_state.toImage();

            // We'll need to raise/die all at the same time, thus just iterating through while making pixels alive doesn't work.
            // Instead, we need to store which pixel raises and which dies.
            std::vector<std::pair<unsigned, unsigned>> to_live;
            std::vector<std::pair<unsigned, unsigned>> to_die;

            for (unsigned long long pixel_x = 0; pixel_x < _game_state.width(); ++pixel_x)
            {
                for (unsigned long long pixel_y = 0; pixel_y < _game_state.height(); ++pixel_y)
                {
                    unsigned char neighbours = 0;
                    for (int neighbour_x = -1; neighbour_x <= 1; ++neighbour_x) {

                        // REFACTOR IDEA: Check for null instead of out of bound coordinates. Out-of-bounds if-clauses should be more inefficient than null checking.
                        // Skipping out of bounds neighbours
                        if (pixel_x == 0 && neighbour_x == -1)
                            continue;
                        else if (pixel_x == _game_state.width() - 1 && neighbour_x == 1)
                            continue;

                        for (int neighbour_y = -1; neighbour_y <= 1; ++neighbour_y) {

                            // Skipping out of bounds neighbours
                            if (pixel_y == 0 && neighbour_y == -1)
                                continue;
                            if (pixel_y == _game_state.height()-1 && neighbour_y == 1)
                                continue;

                            // Ignore self
                            if (neighbour_x == 0 && neighbour_y == 0)
                                continue;

                            if (game_image.pixel(pixel_x + neighbour_x, pixel_y + neighbour_y) == qRgb(0xff, 0xff, 0xff))
                                ++neighbours;

                        }
                    }

                    // Check if alive or dead and list it to be killed/repopulated depending on neighbour count
                    if (game_image.pixel(pixel_x, pixel_y) == qRgb(0xff,0xff,0xff))
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
                game_image.setPixel(coord.first, coord.second, 0);
            for(const std::pair<unsigned, unsigned> coord : to_die)
                game_image.setPixel(coord.first, coord.second, 1);

            _game_state = QBitmap::fromImage(game_image);
        }
        // REFACTOR TODO: No need for the x portion, the sizes are always squares.
        // Change size
        else if(id.at(0) == 's')
        {
            QStringList sizes = id.split('x');
            qInfo() << "Setting size" << sizes;
            _game_state = QBitmap(sizes.at(0).mid(1).toInt(), sizes.at(1).toInt());
            _game_state.fill(Qt::black);
        }
        // Set a point
        else if(id.at(0) == 'p')
        {
            QStringList point = id.split('x');
            int x = point.at(0).mid(1).toInt()*(_game_state.width()/435.0);
            int y = point.at(1).toInt()*(_game_state.height()/435.0);

            qInfo() << "Point" << x << "x" << y;
            qInfo() << "From" << id;
            qInfo() << "Size" << _game_state.size();

            // To modify pixels in bitmap, we'll need to handle it as QImage
            QImage game_image = _game_state.toImage();
            if (game_image.pixel(x, y) == qRgb(0xff, 0xff, 0xff))
                game_image.setPixel(x, y, 1);
            else
                game_image.setPixel(x, y, 0);
            _game_state = QBitmap::fromImage(game_image);
        }
    }
    return _game_state;
}
