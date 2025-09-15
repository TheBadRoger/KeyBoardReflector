#pragma once

#define FILE_BEGIN 0x0

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QStringConverter>
#include <qmessagebox.h>
#include "ui_KeyBrdReflectionWindow.h"


class KeyBrdReflectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    KeyBrdReflectionWindow(QWidget *parent = nullptr);
    ~KeyBrdReflectionWindow();

    void ShowTimeOnBox();

public slots:
    void GetTargetFileName();
    void StartReflectKeys();
    void DisruptInput();
    void SetInputTime();

private:
    Ui::KeyBrdReflectionWindowClass ui;
    QString TargetFileName;
    QFile TargetFile;
    bool DisruptFlag = false;
    unsigned InputTime = 2;
    void GetTargetFileNameByPath();
    void GetTargetFileNameBySelector();
    void ShowTargetFileInfo();
    void ProcessKey(char tkey);
    void ProcessingStat();
    void noProcessingStat();
    void WellDelay(unsigned ms);
};

