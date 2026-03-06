#include "mainwindow.h"
#include "../database/db.h"
#include "../services/userservice.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    UserService service;
    auto users = service.getUsers();

    setWindowTitle("Qt PostgreSQL App");
    resize(520, 320);

    titleLabel = new QLabel("Welcome to the Qt PostgreSQL App");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: 700; margin-bottom: 12px;");

    statusLabel = new QLabel("Click the button to test the database connection.");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("font-size: 14px; margin-bottom: 18px;");

    connectButton = new QPushButton("Connect to PostgreSQL");
    connectButton->setMinimumHeight(42);
    connectButton->setCursor(Qt::PointingHandCursor);

    layout = new QVBoxLayout();
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(connectButton);
    layout->addStretch();

    setLayout(layout);

    setStyleSheet(
        "QWidget {"
        "    background-color: #1f2430;"
        "    color: #f1f1f1;"
        "}"
        "QPushButton {"
        "    background-color: #3a86ff;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 10px 16px;"
        "    font-size: 15px;"
        "    font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2f6fd1;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #2458a8;"
        "}"
    );

    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
}

void MainWindow::onConnectClicked()
{
    Database db;

    if (!db.connect())
        throw std::runtime_error(db.lastError());

    UserService service;

    auto users = service.getUsers();

    QString text;

    for (auto &u : users)
    {
        text += QString::number(u.id) + " - ";
        text += QString::fromStdString(u.name) + " - ";
        text += QString::fromStdString(u.email) + "\n";
    }

    QMessageBox::information(this, "Users", text);
}

// void MainWindow::onConnectClicked()
// {
//     Database db;

//     if (!db.connect()) {
//         statusLabel->setText("Connection failed.");
//         QMessageBox::critical(this, "Database Error", QString::fromStdString(db.lastError()));
//         return;
//     }

//     const std::string version = db.getVersion();

//     if (version.empty()) {
//         statusLabel->setText("Connected, but query failed.");
//         QMessageBox::warning(this, "Connected", "Database connected, but failed to fetch version.");
//         return;
//     }

//     statusLabel->setText("Connected successfully.");
//     QMessageBox::information(
//         this,
//         "Success",
//         QString("Connected to PostgreSQL.\n\nVersion:\n%1").arg(QString::fromStdString(version))
//     );
// }