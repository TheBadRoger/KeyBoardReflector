#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QThread>
#include "ui_KeyBrdReflectionWindow.h"
#include "ProcessingThread.h"

class KeyBrdReflectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    KeyBrdReflectionWindow(QWidget *parent = nullptr);
    ~KeyBrdReflectionWindow();

public slots:
    void GetTargetFileName();
    void StartButtonClicked();
    void CancelProcessingStat(QString text);
    void UpdateStatusBarText(QString text);
    void RefreshFileContent();
    void SwitchWindowOnTop();

signals:
    void SendFile(QFile* pfile);
	void StartProcessingSignal();
	void NoticeRefreshFile();

private:
    Ui::KeyBrdReflectionWindowClass ui;
    QString CurrentFilePath, LastDirPath, ShowText;
	QFile* pTargetFile = new QFile();

    ProcessingThread* Processor = new ProcessingThread();

    QThread* ProcessorThread = new QThread();

	bool ButtonProcessingStat = false;

    void ShowTargetFileInfo();
    void ProcessingStat();
    void SwitchStartButtonStat(bool flag);
};

