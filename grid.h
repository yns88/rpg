#ifndef GRID_H
#define GRID_H

#include <QLabel>

class Grid : public QLabel
{
    Q_OBJECT
private:
    qint32 gridLength;
    int offsetX;
    int offsetY;
    bool resized;
public:
    explicit Grid(int len, QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent * ev);
    virtual void resizeEvent(QResizeEvent *);
    virtual void showEvent(QShowEvent *);
    qint32 gridSize();
    void setGridSize(int n);
    
    void setOffset(QPoint p);
    QPoint offset();
signals:
    
public slots:
    
};

#endif // GRID_H
