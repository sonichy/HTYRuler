#include "mainwindow.h"
#include <QPainter>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QAction>
#include <QShortcut>
#include <QDesktopWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      settings(QCoreApplication::organizationName(), QCoreApplication::applicationName())
{
    setWindowIcon(QIcon(":/HTYRuler.png"));
    setWindowOpacity(0.5);
    isMLBD = false;
    int width = settings.value("width").toInt();
    if(width == 0) width = 500;
    int height = settings.value("height").toInt();
    if(height == 0) height = 100;
    setFixedSize(width, height);
    setWindowFlags(Qt::FramelessWindowHint);
    readSettings();

    QAction *action_set = new QAction(tr("&Set"), this);
    action_set->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    connect(action_set, SIGNAL(triggered(bool)), this, SLOT(set()));
    addAction(action_set);
    QAction *action_about = new QAction(tr("&About"), this);
    action_about->setShortcut(QKeySequence::HelpContents);
    connect(action_about, SIGNAL(triggered(bool)), this, SLOT(about()));
    addAction(action_about);
    QAction *action_quit = new QAction(tr("&Quit"), this);
    action_quit->setShortcut(QKeySequence::Quit);
    connect(action_quit, SIGNAL(triggered(bool)), this, SLOT(close()));
    addAction(action_quit);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right),this), SIGNAL(activated()), this, SLOT(lengthen()));
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left),this), SIGNAL(activated()), this, SLOT(shorten()));
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_0),this), SIGNAL(activated()), this, SLOT(reset()));
    connect(new QShortcut(QKeySequence(Qt::Key_Left),this), SIGNAL(activated()), this, SLOT(moveLeft()));
    connect(new QShortcut(QKeySequence(Qt::Key_Right),this), SIGNAL(activated()), this, SLOT(moveRight()));
    connect(new QShortcut(QKeySequence(Qt::Key_Up),this), SIGNAL(activated()), this, SLOT(moveUp()));
    connect(new QShortcut(QKeySequence(Qt::Key_Down),this), SIGNAL(activated()), this, SLOT(moveDown()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *)
{
    float start = settings.value("start").toFloat();
    float end = settings.value("end").toFloat();
    float mark = settings.value("mark").toFloat();
    int pen_width = settings.value("pen_width").toInt();
    int y_marktext = 35;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // face
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(QRect(0,0,width(),height()));
    // mark
    if(start < end){
        for(float i=start; i<=end; i+=1){
            float d = width()/(end-start);
            float x = d * (i-start);
            if(i == mark){
                painter.setPen(QPen(Qt::red, pen_width));
                painter.drawLine(QPoint(x,0), QPoint(x,20));
                painter.drawText(QPoint(x, y_marktext), QString::number(i));
            }else{
                painter.setPen(QPen(Qt::black, pen_width));
                painter.drawLine(QPoint(x,0), QPoint(x,10));
            }
        }
    }else{
        for(float i=start; i>=end; i-=1){
            float d = width()/(start-end);
            float x = d * (start-i);
            if(i == mark){
                painter.setPen(QPen(Qt::red, pen_width));
                painter.drawLine(QPoint(x,0), QPoint(x,20));
                painter.drawText(QPoint(x, y_marktext), QString::number(i));
            }else{
                painter.setPen(QPen(Qt::black, pen_width));
                painter.drawLine(QPoint(x,0), QPoint(x,10));
            }
        }
    }
    painter.setPen(QPen(Qt::black, pen_width));
    painter.drawText(QPoint(0, y_marktext), QString::number(start));
    painter.drawText(QPoint(width()-20, y_marktext), QString::number(end));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isMLBD = true;
        m_point = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isMLBD) {
        setCursor(Qt::ClosedHandCursor);
        move(event->pos() - m_point + pos());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMLBD = false;
    setCursor(Qt::ArrowCursor);
    settings.setValue("geometry", saveGeometry());
    //settings.setValue("windowState", saveState());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::lengthen()
{
    if(width() < QApplication::desktop()->width()){
        setFixedSize(width()+1, height());
        settings.setValue("width", width());
        settings.setValue("height", height());
        update();
    }
}

void MainWindow::shorten()
{
    if(width() > 200){
        setFixedSize(width()-1, height());
        settings.setValue("width", width());
        settings.setValue("height", height());
        update();
    }
}

void MainWindow::reset()
{
    setFixedSize(500, 100);
    settings.setValue("width", width());
    settings.setValue("height", height());
    update();
}

void MainWindow::set()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("设置");
    dialog->setFixedSize(200, 200);
    QVBoxLayout *vbox = new QVBoxLayout;

    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *label = new QLabel("起点");
    hbox->addWidget(label);
    int start = settings.value("start").toInt();
    QSpinBox *spin_start = new QSpinBox;
    spin_start->setValue(start);
    spin_start->setRange(0, 1000);
    hbox->addWidget(spin_start);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    label = new QLabel("终点");
    hbox->addWidget(label);
    int end = settings.value("end").toInt();
    QSpinBox *spin_end = new QSpinBox;
    spin_end->setValue(end);
    spin_end->setRange(0, 1000);
    hbox->addWidget(spin_end);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    label = new QLabel("标记点");
    hbox->addWidget(label);
    int mark = settings.value("mark").toInt();
    QSpinBox *spin_mark = new QSpinBox;
    spin_mark->setValue(mark);
    spin_mark->setRange(-1, 1000);
    hbox->addWidget(spin_mark);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    label = new QLabel("线宽");
    hbox->addWidget(label);
    int pen_width = settings.value("pen_width").toInt();
    QSpinBox *spin_pen_width = new QSpinBox;
    spin_pen_width->setValue(pen_width);
    spin_pen_width->setRange(1, 100);
    hbox->addWidget(spin_pen_width);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    QPushButton *pushButton_confirm = new QPushButton("确定");
    hbox->addWidget(pushButton_confirm);
    QPushButton *pushButton_cancel = new QPushButton("取消");
    hbox->addWidget(pushButton_cancel);
    vbox->addLayout(hbox);

    dialog->setLayout(vbox);

    connect(pushButton_confirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(pushButton_cancel, SIGNAL(clicked()), dialog, SLOT(reject()));
    if(dialog->exec() == QDialog::Accepted){
        settings.setValue("start", spin_start->value());
        settings.setValue("end", spin_end->value());
        settings.setValue("mark", spin_mark->value());
        settings.setValue("pen_width", spin_pen_width->value());
        update();
    }
    dialog->close();
}

void MainWindow::moveLeft()
{
    move(x() - 1, y());
}

void MainWindow::moveRight()
{
    move(x() + 1, y());
}

void MainWindow::moveUp()
{
    move(x(), y() - 1);
}

void MainWindow::moveDown()
{
    move(x(), y() + 1);
}

void MainWindow::about()
{
    QMessageBox MB(QMessageBox::NoIcon, "关于", "海天鹰尺子 1.0\n一款基于 Qt 的屏幕测量工具。\n作者：海天鹰\nE-mail: sonichy@163.com\n主页：https://github.com/sonichy\n\n快捷键：\nCtrl + ← 缩短尺子\nCtrl + → 加长尺子\n← 左移一个像素\n→ 右移一个像素\n↑ 上移一个像素\n↓ 下移一个像素");
    MB.setIconPixmap(QPixmap(":/HTYRuler.png"));
    MB.exec();
}