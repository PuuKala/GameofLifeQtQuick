#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <QQuickImageProvider>
/**
 * @brief Game of life for QtQuick GUI
 * @details Acts as a image provider for QtQuick GUI and stores the game state. Takes commands through the given image path.
 */
class GameOfLife : public QQuickImageProvider
{
public:
    GameOfLife();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
private:
    QImage _game_state_image = QImage(1, 1, QImage::Format_Mono);
    const float _GUI_image_size = 435; // Float, as we want to use float division
};

#endif // GAMEOFLIFE_H
