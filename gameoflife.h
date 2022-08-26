#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <QQuickImageProvider>
#include <QBitmap>

class GameOfLife : public QQuickImageProvider
{
public:
    GameOfLife();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
private:
    QBitmap _game_state = QBitmap(1,1);
};

#endif // GAMEOFLIFE_H