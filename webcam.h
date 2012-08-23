/*
 Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
 All rights reserved.
 $Id: webcam.h 9e28cb30f3ba 2012/04/27 14:19:35 Oliver Lau <oliver@von-und-fuer-lau.de> $
*/

#ifndef __WEBCAM_H_
#define __WEBCAM_H_

#include <QVector>
#include <QString>

class Webcam
{
public:
    Webcam(void);

    const QString& devicePath(void) const { return mDevicePath; }
    const QString& description(void) const { return mDescription; }
    const QString& name(void) const { return mName; }

    void setDevicePath(QString devicePath) { mDevicePath = devicePath; }
    void setDescription(QString description) { mDescription = description; }
    void setName(QString name) { mName = name; }

private:
    QString mDevicePath;
    QString mDescription;
    QString mName;

    static QVector<Webcam*> enumerate(void);

};

#endif // __WEBCAM_H_
