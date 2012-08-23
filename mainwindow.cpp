/*
 Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
 All rights reserved.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QtDebug>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mVideoWidget(NULL)
{
    QCoreApplication::setOrganizationName("c't");
    QCoreApplication::setOrganizationDomain("ct.de");
    QCoreApplication::setApplicationName("Qapture");
    QSettings::setDefaultFormat(QSettings::NativeFormat);
    ui->setupUi(this);
    ui->verticalLayout->removeWidget(ui->dummyWidget);
    mVideoWidget = new VideoWidget;
    ui->verticalLayout->insertWidget(0, mVideoWidget);
    QObject::connect(ui->snapshotButton, SIGNAL(clicked()), SLOT(saveImage()));
    QObject::connect(ui->snapshotDirPushButton, SIGNAL(clicked()), SLOT(selectSnapshotDirectory()));
    QObject::connect(ui->widthSpinBox, SIGNAL(valueChanged(double)), SLOT(setOriginalObjectWidth(double)));
    QObject::connect(ui->heightSpinBox, SIGNAL(valueChanged(double)), SLOT(setOriginalObjectHeight(double)));
    loadSettings();
}


MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        mVideoWidget->resetTransform();
        e->accept();
        break;
    default:
        e->ignore();
        break;
    }
}


void MainWindow::saveImage(void)
{
    QString fileName = ui->snapshotDirLineEdit->text() + "\\" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") + ".png";
    QCursor currCursor = cursor();
    setCursor(Qt::WaitCursor);
    QSize maxSize = mVideoWidget->polygonSize();
    QImage image = mVideoWidget->currentImage().scaled(maxSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    image.save(fileName, "PNG");
    setCursor(currCursor);
    QMessageBox::information(this, tr("Image saved."), tr("Image saved as \"%1\".").arg(fileName));
}


void MainWindow::selectSnapshotDirectory(void)
{
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select snapshot directory"), ui->snapshotDirLineEdit->text());
    ui->snapshotDirLineEdit->setText(dir);
}


void MainWindow::calculateAspectRatio(void)
{
    ui->aspectRatioLabel->setText(QString("%1").arg(ui->widthSpinBox->value()/ui->heightSpinBox->value(), 7, 'f', 3));
}


void MainWindow::setOriginalObjectWidth(double width)
{
    ui->widthSpinBox->setValue(width);
    calculateAspectRatio();
}


void MainWindow::setOriginalObjectHeight(double height)
{
    ui->heightSpinBox->setValue(height);
    calculateAspectRatio();
}


void MainWindow::loadSettings(void)
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    ui->snapshotDirLineEdit->setText(settings.value("ImageDirectory").toString());
    settings.endGroup();
    settings.beginGroup("Original");
    ui->widthSpinBox->setValue(settings.value("width").toDouble());
    ui->heightSpinBox->setValue(settings.value("height").toDouble());
    settings.endGroup();
    calculateAspectRatio();
}


void MainWindow::saveSettings(void)
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("ImageDirectory", ui->snapshotDirLineEdit->text());
    settings.endGroup();
    settings.beginGroup("Original");
    settings.setValue("width", ui->widthSpinBox->value());
    settings.setValue("height", ui->heightSpinBox->value());
    settings.endGroup();
}
