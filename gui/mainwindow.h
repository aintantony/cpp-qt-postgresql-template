#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QLabel;
class QPushButton;
class QVBoxLayout;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onConnectClicked();

private:
    QLabel *titleLabel;
    QLabel *statusLabel;
    QPushButton *connectButton;
    QVBoxLayout *layout;
};

#endif