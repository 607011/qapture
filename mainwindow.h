/*
 Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
 All rights reserved.
*/

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QKeyEvent>
#include "VideoWidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent*);

private slots:
    void saveImage(void);
    void selectSnapshotDirectory(void);
    void setOriginalObjectWidth(double);
    void setOriginalObjectHeight(double);

private:
    void loadSettings(void);
    void saveSettings(void);
    void calculateAspectRatio(void);

private:
    Ui::MainWindow *ui;
    VideoWidget* mVideoWidget;
};

#endif // __MAINWINDOW_H_
