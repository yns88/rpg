#ifndef BARITEM_H
#define BARITEM_H

#include <QLabel>

class BarItem : public QLabel
{
    Q_OBJECT
private:


    QPoint delta;

protected:
    static const int sideLength = 100;
    QPoint offset;
    QPixmap image;
    QString imgPath;
    QLabel * movingLabel;
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void dropAction(QPoint p){}

public:
    explicit BarItem(QString imageName, QWidget * parent=0);
    ~BarItem();
    
signals:
    
public slots:
    
};

#endif // BARITEM_H
