#include "mainwindow.h"
#include "../database/db.h"
#include "../services/userservice.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QMessageBox>
#include <QString>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

static const char *BG_DEEP    = "#0d1117";
static const char *BG_SURFACE = "#161b22";
static const char *BG_CARD    = "#1c2128";
static const char *BORDER     = "#30363d";
static const char *ACCENT     = "#58a6ff";
static const char *ACCENT_DIM = "#1f3a5f";
static const char *TEXT_PRI   = "#e6edf3";
static const char *TEXT_SEC   = "#8b949e";
static const char *TEXT_MUT   = "#484f58";
static const char *GREEN      = "#3fb950";
static const char *RED        = "#f85149";

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("cpp-gui-postgres");
    setMinimumSize(680, 560);
    resize(780, 640);

    rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    applyGlobalStyles();
    buildHeader();
    buildStatusBar();
    buildUserTable();

    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
}

void MainWindow::buildHeader()
{
    QFrame *header = new QFrame();
    header->setObjectName("header");
    header->setStyleSheet(
        "QFrame#header {"
        "  background-color: " + QString(BG_SURFACE) + ";"
        "  border-bottom: 1px solid " + QString(BORDER) + ";"
        "}"
    );

    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(28, 20, 28, 20);
    hl->setSpacing(0);

    QHBoxLayout *left = new QHBoxLayout();
    left->setSpacing(14);

    QLabel *icon = new QLabel("⬡");
    icon->setStyleSheet(
        QString("font-size: 28px; color: %1;").arg(ACCENT)
    );

    QVBoxLayout *titles = new QVBoxLayout();
    titles->setSpacing(2);

    titleLabel = new QLabel("User Directory");
    titleLabel->setStyleSheet(
        QString("font-size: 18px; font-weight: 700; color: %1; font-family: 'SF Mono', 'Fira Code', monospace;").arg(TEXT_PRI)
    );

    subtitleLabel = new QLabel("PostgreSQL · libpqxx · Qt6");
    subtitleLabel->setStyleSheet(
        QString("font-size: 12px; color: %1; font-family: 'SF Mono', 'Fira Code', monospace;").arg(TEXT_SEC)
    );

    titles->addWidget(titleLabel);
    titles->addWidget(subtitleLabel);

    left->addWidget(icon);
    left->addLayout(titles);

    connectButton = new QPushButton("Connect");
    connectButton->setCursor(Qt::PointingHandCursor);
    connectButton->setFixedHeight(36);
    connectButton->setStyleSheet(
        QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: #ffffff;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 0 20px;"
        "  font-size: 13px;"
        "  font-weight: 600;"
        "  font-family: 'SF Mono', 'Fira Code', monospace;"
        "}"
        "QPushButton:hover { background-color: #4393e4; }"
        "QPushButton:pressed { background-color: #2d76c7; }"
        ).arg(ACCENT)
    );

    hl->addLayout(left);
    hl->addStretch();
    hl->addWidget(connectButton);

    rootLayout->addWidget(header);
}

void MainWindow::buildStatusBar()
{
    statusBar = new QFrame();
    statusBar->setObjectName("statusBar");
    statusBar->setFixedHeight(36);
    statusBar->setStyleSheet(
        "QFrame#statusBar {"
        "  background-color: " + QString(BG_DEEP) + ";"
        "  border-bottom: 1px solid " + QString(BORDER) + ";"
        "}"
    );

    QHBoxLayout *hl = new QHBoxLayout(statusBar);
    hl->setContentsMargins(28, 0, 28, 0);
    hl->setSpacing(8);

    statusDot = new QLabel("●");
    statusDot->setStyleSheet(QString("font-size: 9px; color: %1;").arg(TEXT_MUT));

    statusLabel = new QLabel("Not connected — press Connect to load users");
    statusLabel->setStyleSheet(
        QString("font-size: 12px; color: %1; font-family: 'SF Mono', 'Fira Code', monospace;").arg(TEXT_SEC)
    );

    userCountLabel = new QLabel("");
    userCountLabel->setStyleSheet(
        QString("font-size: 12px; color: %1; font-family: 'SF Mono', 'Fira Code', monospace;").arg(TEXT_MUT)
    );

    hl->addWidget(statusDot);
    hl->addWidget(statusLabel);
    hl->addStretch();
    hl->addWidget(userCountLabel);

    rootLayout->addWidget(statusBar);
}

void MainWindow::buildUserTable()
{
    QFrame *colHeader = new QFrame();
    colHeader->setFixedHeight(32);
    colHeader->setStyleSheet(
        QString(
        "QFrame {"
        "  background-color: %1;"
        "  border-bottom: 1px solid %2;"
        "}"
        ).arg(BG_SURFACE).arg(BORDER)
    );

    QHBoxLayout *chl = new QHBoxLayout(colHeader);
    chl->setContentsMargins(28, 0, 28, 0);
    chl->setSpacing(0);

    auto makeColLabel = [&](const QString &text, int stretch) {
        QLabel *l = new QLabel(text.toUpper());
        l->setStyleSheet(
            QString("font-size: 10px; font-weight: 700; color: %1; letter-spacing: 1px;"
                    "font-family: 'SF Mono', 'Fira Code', monospace;").arg(TEXT_MUT)
        );
        chl->addWidget(l, stretch);
    };

    makeColLabel("ID",    1);
    makeColLabel("Name",  3);
    makeColLabel("Email", 4);

    rootLayout->addWidget(colHeader);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(
        QString(
        "QScrollArea { background-color: %1; }"
        "QScrollBar:vertical {"
        "  background: %1; width: 8px; margin: 0;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: %2; border-radius: 4px; min-height: 24px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
        ).arg(BG_DEEP).arg(BORDER)
    );

    scrollContent = new QFrame();
    scrollContent->setStyleSheet(QString("background-color: %1;").arg(BG_DEEP));

    userListLayout = new QVBoxLayout(scrollContent);
    userListLayout->setContentsMargins(0, 0, 0, 0);
    userListLayout->setSpacing(0);
    userListLayout->addStretch();

    QLabel *placeholder = new QLabel("No data loaded yet");
    placeholder->setObjectName("placeholder");
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setStyleSheet(
        QString("font-size: 14px; color: %1; padding: 60px;"
                "font-family: 'SF Mono', 'Fira Code', monospace;").arg(TEXT_MUT)
    );
    userListLayout->insertWidget(0, placeholder);

    scrollArea->setWidget(scrollContent);
    rootLayout->addWidget(scrollArea, 1);
}

QFrame *MainWindow::makeUserCard(const User &u, int index)
{
    QFrame *card = new QFrame();
    card->setFixedHeight(52);
    card->setStyleSheet(
        QString(
        "QFrame {"
        "  background-color: %1;"
        "  border-bottom: 1px solid %2;"
        "}"
        "QFrame:hover {"
        "  background-color: %3;"
        "}"
        ).arg(index % 2 == 0 ? BG_DEEP : BG_CARD)
         .arg(BORDER)
         .arg(BG_SURFACE)
    );

    QHBoxLayout *hl = new QHBoxLayout(card);
    hl->setContentsMargins(28, 0, 28, 0);
    hl->setSpacing(0);

    QLabel *idLabel = new QLabel(QString("#%1").arg(u.id));
    idLabel->setFixedWidth(56);
    idLabel->setStyleSheet(
        QString(
        "font-size: 12px; font-weight: 700; color: %1;"
        "background-color: %2; border-radius: 4px;"
        "padding: 2px 6px; font-family: 'SF Mono', 'Fira Code', monospace;"
        ).arg(ACCENT).arg(ACCENT_DIM)
    );

    QString initials = "?";
    if (!u.name.empty()) {
        initials = QString(QChar(std::toupper((unsigned char)u.name[0])));
        size_t sp = u.name.find(' ');
        if (sp != std::string::npos && sp + 1 < u.name.size())
            initials += QString(QChar(std::toupper((unsigned char)u.name[sp + 1])));
    }
    QLabel *avatar = new QLabel(initials);
    avatar->setFixedSize(30, 30);
    avatar->setAlignment(Qt::AlignCenter);

    static const char *avatarColors[] = {
        "#2ea043","#d29922","#db6d28","#58a6ff","#bc8cff","#f78166"
    };
    const char *avatarBg = avatarColors[u.id % 6];
    avatar->setStyleSheet(
        QString(
        "background-color: %1; border-radius: 15px;"
        "font-size: 11px; font-weight: 700; color: #ffffff;"
        "font-family: 'SF Mono', 'Fira Code', monospace;"
        ).arg(avatarBg)
    );

    QLabel *nameLabel = new QLabel(QString::fromStdString(u.name));
    nameLabel->setStyleSheet(
        QString("font-size: 13px; font-weight: 600; color: %1;").arg(TEXT_PRI)
    );

    QLabel *emailLabel = new QLabel(QString::fromStdString(u.email));
    emailLabel->setStyleSheet(
        QString("font-size: 13px; color: %1;").arg(TEXT_SEC)
    );

    hl->addWidget(idLabel, 1);
    hl->addSpacing(12);
    hl->addWidget(avatar);
    hl->addSpacing(10);
    hl->addWidget(nameLabel, 3);
    hl->addWidget(emailLabel, 4);

    return card;
}

void MainWindow::populateUsers(const std::vector<User> &users)
{
    QLayoutItem *item;
    while ((item = userListLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (users.empty()) {
        QLabel *empty = new QLabel("No users found in database");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet(
            QString("font-size: 14px; color: %1; padding: 60px;"
                    "font-family: 'SF Mono', 'Fira Code', monospace;").arg(TEXT_MUT)
        );
        userListLayout->addWidget(empty);
        userListLayout->addStretch();
        return;
    }

    for (int i = 0; i < (int)users.size(); ++i) {
        QFrame *card = makeUserCard(users[i], i);

        auto *effect = new QGraphicsOpacityEffect(card);
        effect->setOpacity(0.0);
        card->setGraphicsEffect(effect);

        userListLayout->addWidget(card);

        auto *anim = new QPropertyAnimation(effect, "opacity", card);
        anim->setDuration(200);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        QTimer::singleShot(i * 35, [anim]() { anim->start(); });
    }

    userListLayout->addStretch();

    userCountLabel->setText(
        QString("%1 user%2").arg(users.size()).arg(users.size() == 1 ? "" : "s")
    );
}

void MainWindow::setStatus(const QString &text, bool connected)
{
    statusLabel->setText(text);
    statusDot->setStyleSheet(
        QString("font-size: 9px; color: %1;").arg(connected ? GREEN : RED)
    );
}

void MainWindow::onConnectClicked()
{
    connectButton->setEnabled(false);
    connectButton->setText("Connecting…");
    setStatus("Connecting to database…", false);

    Database db;

    if (!db.connect()) {
        setStatus("Connection failed: " + QString::fromStdString(db.lastError()), false);
        connectButton->setEnabled(true);
        connectButton->setText("Retry");
        return;
    }

    UserService service;
    auto users = service.getUsers();

    setStatus(
        QString("Connected · %1").arg(QString::fromStdString(db.getVersion()).section('\n', 0, 0)),
        true
    );

    populateUsers(users);

    connectButton->setText("Refresh");
    connectButton->setEnabled(true);
}

void MainWindow::applyGlobalStyles()
{
    setStyleSheet(
        QString(
        "QWidget {"
        "  background-color: %1;"
        "  color: %2;"
        "  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;"
        "}"
        "QLabel { background: transparent; }"
        ).arg(BG_DEEP).arg(TEXT_PRI)
    );
}