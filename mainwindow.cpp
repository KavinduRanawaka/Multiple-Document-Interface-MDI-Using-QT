#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create MDI area
    mdiArea = new QMdiArea(this);
    mdiArea->setBackground(QBrush(Qt::white));


    // Bottom task bar setup
    taskBarWidget = new QWidget(this);
    taskBarLayout = new QHBoxLayout(taskBarWidget);
    taskBarLayout->setContentsMargins(5, 5, 5, 5);
    taskBarLayout->setSpacing(5);

    // Central layout with MDI area + task bar
    QWidget *central = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(central);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(mdiArea);
    centralLayout->addWidget(taskBarWidget);
    setCentralWidget(central);

    setupMenuBar();
    setupToolBar();
    setupTaskBar();

    createPositionedSubWindows();
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenuBar() {
    QMenu *windowMenu = menuBar()->addMenu("Window");

    QAction *cascadeAct = new QAction("Cascade", this);
    QAction *tileAct = new QAction("Tile", this);
    QAction *listAct = new QAction("List Windows", this);

    connect(cascadeAct, &QAction::triggered, this, &MainWindow::cascadeWindows);
    connect(tileAct, &QAction::triggered, this, &MainWindow::tileWindows);
    connect(listAct, &QAction::triggered, this, &MainWindow::listWindows);

    windowMenu->addAction(cascadeAct);
    windowMenu->addAction(tileAct);
    windowMenu->addAction(listAct);
}

void MainWindow::setupToolBar() {
    QToolBar *toolBar = addToolBar("Main Toolbar");

    toolBar->addAction("Cascade", this, &MainWindow::cascadeWindows);
    toolBar->addAction("Tile", this, &MainWindow::tileWindows);

    QAction *minAllAct = toolBar->addAction("Minimize All");
    connect(minAllAct, &QAction::triggered, this, &MainWindow::minimizeAllSubWindows);

    QAction *resetLayoutAct = toolBar->addAction("Reset Layout");
    connect(resetLayoutAct, &QAction::triggered, this, &MainWindow::resetLayout);
}

void MainWindow::setupTaskBar() {
    taskBarWidget->setStyleSheet("background-color: #222; color: white;");
}

void MainWindow::createPositionedSubWindows() {
    QSize mdiSize = mdiArea->size();

    int w = mdiSize.width() / 2;
    int h = mdiSize.height() / 2;

    QStringList names = { "Top Left", "Top Right", "Bottom Left", "Bottom Right" };
    QPoint positions[] = {
        QPoint(0, 0),
        QPoint(w, 0),
        QPoint(0, h),
        QPoint(w, h)
    };

    for (int i = 0; i < 4; ++i) {
        QWidget *content = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(content);
        QLabel *label = new QLabel("This is " + names[i]);
        layout->addWidget(label);
        content->setLayout(layout);

        QMdiSubWindow *sub = mdiArea->addSubWindow(content);
        sub->setWindowTitle(names[i]);
        sub->resize(w - 20, h - 20);
        sub->move(positions[i]);
        sub->show();

        addMinimizeContext(sub);
    }

    mdiArea->tileSubWindows();
}

void MainWindow::addMinimizeContext(QMdiSubWindow *subWindow) {
    QAction *minimizeAct = new QAction("Minimize", subWindow);
    subWindow->addAction(minimizeAct);
    subWindow->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(minimizeAct, &QAction::triggered, this, [this, subWindow]() {
        minimizeSubWindow(subWindow);
    });
}

void MainWindow::minimizeSubWindow(QMdiSubWindow *subWin) {
    subWin->hide();
    QPushButton *btn = new QPushButton(subWin->windowTitle(), taskBarWidget);
    taskBarLayout->addWidget(btn);
    taskBarButtons[btn] = subWin;
    connect(btn, &QPushButton::clicked, this, &MainWindow::restoreSubWindow);
}

void MainWindow::restoreSubWindow() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn || !taskBarButtons.contains(btn)) return;

    QMdiSubWindow *subWin = taskBarButtons[btn];
    subWin->showNormal();
    subWin->show();

    taskBarLayout->removeWidget(btn);
    btn->deleteLater();
    taskBarButtons.remove(btn);
}

void MainWindow::cascadeWindows() {
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (QMdiSubWindow *win : windows) {
        if (win->isHidden()) {
            win->showNormal();
            win->show();

            QPushButton *btnToRemove = nullptr;
            for (auto it = taskBarButtons.begin(); it != taskBarButtons.end(); ++it) {
                if (it.value() == win) {
                    btnToRemove = it.key();
                    break;
                }
            }
            if (btnToRemove) {
                taskBarLayout->removeWidget(btnToRemove);
                btnToRemove->deleteLater();
                taskBarButtons.remove(btnToRemove);
            }
        }
    }
    mdiArea->cascadeSubWindows();
}

void MainWindow::tileWindows() {
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (QMdiSubWindow *win : windows) {
        if (win->isHidden()) {
            win->showNormal();
            win->show();

            QPushButton *btnToRemove = nullptr;
            for (auto it = taskBarButtons.begin(); it != taskBarButtons.end(); ++it) {
                if (it.value() == win) {
                    btnToRemove = it.key();
                    break;
                }
            }
            if (btnToRemove) {
                taskBarLayout->removeWidget(btnToRemove);
                btnToRemove->deleteLater();
                taskBarButtons.remove(btnToRemove);
            }
        }
    }
    mdiArea->tileSubWindows();
}

void MainWindow::listWindows() {
    QList<QMdiSubWindow*> wins = mdiArea->subWindowList();
    qDebug() << "Open Windows:";
    for (QMdiSubWindow *win : wins)
        qDebug() << win->windowTitle();
}

void MainWindow::minimizeAllSubWindows() {
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (QMdiSubWindow *win : windows) {
        if (!win->isHidden()) {
            minimizeSubWindow(win);
        }
    }
}

void MainWindow::resetLayout() {
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();

    QSize mdiSize = mdiArea->size();

    int w = mdiSize.width() / 2;
    int h = mdiSize.height() / 2;

    QPoint positions[] = {
        QPoint(0, 0),
        QPoint(w, 0),
        QPoint(0, h),
        QPoint(w, h)
    };

    int index = 0;
    for (QMdiSubWindow *win : windows) {
        if (win->isHidden()) {
            win->showNormal();
            win->show();

            QPushButton *btnToRemove = nullptr;
            for (auto it = taskBarButtons.begin(); it != taskBarButtons.end(); ++it) {
                if (it.value() == win) {
                    btnToRemove = it.key();
                    break;
                }
            }
            if (btnToRemove) {
                taskBarLayout->removeWidget(btnToRemove);
                btnToRemove->deleteLater();
                taskBarButtons.remove(btnToRemove);
            }
        }

        if (index < 4) {
            win->resize(w - 20, h - 20);
            win->move(positions[index]);
            ++index;
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    resetLayout();
}
