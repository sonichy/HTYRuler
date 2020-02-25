#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
#include <QSettings>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *);

private:
    bool isMLBD;
    QPoint m_point;
    QSettings settings;
    qreal angle;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void readSettings();

private slots:
    void about();
    void lengthen();
    void shorten();
    void reset();
    void set();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void rotateCW();
    void rotateCCW();

};

#endif // MAINWINDOW_H