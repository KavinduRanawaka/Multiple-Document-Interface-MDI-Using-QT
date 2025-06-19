#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QMap>
#include <QHBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void minimizeSubWindow(QMdiSubWindow *subWin);
    void restoreSubWindow();
    void cascadeWindows();
    void tileWindows();
    void listWindows();
    void minimizeAllSubWindows();
    void resetLayout();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QMdiArea *mdiArea;
    QWidget *taskBarWidget;
    QHBoxLayout *taskBarLayout;
    QMap<QPushButton*, QMdiSubWindow*> taskBarButtons;

    void setupMenuBar();
    void setupToolBar();
    void setupTaskBar();
    void createPositionedSubWindows();
    void addMinimizeContext(QMdiSubWindow *subWindow);
};

#endif // MAINWINDOW_H
