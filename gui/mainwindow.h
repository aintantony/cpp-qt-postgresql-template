#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <vector>
#include "../models/user.h"

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QScrollArea;
class QFrame;
class QGraphicsOpacityEffect;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onConnectClicked();

private:
    QLabel         *appIconLabel;
    QLabel         *titleLabel;
    QLabel         *subtitleLabel;
    QPushButton    *connectButton;

    QFrame         *statusBar;
    QLabel         *statusDot;
    QLabel         *statusLabel;

    QScrollArea    *scrollArea;
    QFrame         *scrollContent;
    QVBoxLayout    *userListLayout;

    QLabel         *userCountLabel;

    QVBoxLayout    *rootLayout;

    void buildHeader();
    void buildStatusBar();
    void buildUserTable();
    void applyGlobalStyles();
    void populateUsers(const std::vector<User> &users);
    QFrame *makeUserCard(const User &u, int index);
    void setStatus(const QString &text, bool connected);
};

#endif