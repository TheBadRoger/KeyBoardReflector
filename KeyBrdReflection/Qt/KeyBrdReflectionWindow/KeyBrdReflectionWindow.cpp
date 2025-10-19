#include "KeyBrdReflectionWindow.h"
#include <algorithm>
#include <QWindow>

KeyBrdReflectionWindow::KeyBrdReflectionWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.LAB_ShowFileWordCount->setText("");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    Processor->moveToThread(ProcessorThread);

    connect(this, SIGNAL(NoticeRefreshFile()), Processor, SLOT(RefreshFile()));
    connect(this, SIGNAL(SendFile(QFile*)), Processor, SLOT(ReceiveFile(QFile*)));
	connect(this, SIGNAL(StartProcessingSignal()), Processor, SLOT(ProcessReflection()));
    connect(Processor, SIGNAL(ProcessFinishSignal(QString)), this, SLOT(CancelProcessingStat(QString)));
    connect(Processor, SIGNAL(SendNewStatusText(QString)), this, SLOT(UpdateStatusBarText(QString)));
}

KeyBrdReflectionWindow::~KeyBrdReflectionWindow(){}

void KeyBrdReflectionWindow::GetTargetFileName() {
    QString tPath = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"),
        LastDirPath, "All Files(*.*)", nullptr, QFileDialog::DontResolveSymlinks);

    if (!tPath.isEmpty()) {
        LastDirPath = CurrentFilePath = tPath;
        ShowTargetFileInfo();   
    }

    if (pTargetFile->isOpen())pTargetFile->close();
}

void KeyBrdReflectionWindow::StartButtonClicked() {
    if (CurrentFilePath.isEmpty())QMessageBox::critical(this, QString("错误"), "请先选择文件");
    else {
        if (ButtonProcessingStat) {
            ProcessorThread->quit();
            CancelProcessingStat("已取消");
            SwitchStartButtonStat(false);
        }
        else {
            ProcessingStat();
            ProcessorThread->start();
            emit SendFile(pTargetFile);
            while (isActiveWindow()) QCoreApplication::processEvents(QEventLoop::AllEvents);
            if (ProcessorThread->isRunning()) {
                ui.BTN_WriteConfirm->setEnabled(false);
                emit StartProcessingSignal();
                SwitchStartButtonStat(true);
            }
        }
    }
}

void KeyBrdReflectionWindow::UpdateStatusBarText(QString text) {
    ui.statusBar->showMessage(text);
}

void KeyBrdReflectionWindow::SwitchWindowOnTop() {
    if (ui.STC_KeepOnTop->isChecked())
        windowHandle()->setFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
        windowHandle()->setFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
}

void KeyBrdReflectionWindow::ShowTargetFileInfo() {
    pTargetFile->setFileName(CurrentFilePath);
    if (pTargetFile->open(QFile::ReadOnly | QFile::Text)) {
        ui.BOX_ShowPath->setText(CurrentFilePath);
        ui.BTN_RefreshContent->setEnabled(true);
        ShowText = QTextStream(pTargetFile).readAll();
        ui.BOX_ShowContent->setText(ShowText);
        ui.LAB_ShowFileWordCount->setText(QString("%1 个字符").arg(ShowText.size()));
    }
    else QMessageBox::critical(this, QString("错误"), "打开文件失败");
}

void KeyBrdReflectionWindow::ProcessingStat() {
	ui.BTN_OpenTargetFile->setEnabled(false);
    ui.BTN_RefreshContent->setEnabled(false);
	ui.statusBar->showMessage("就绪");
	SwitchStartButtonStat(true);
}

void KeyBrdReflectionWindow::CancelProcessingStat(QString text) {
    ui.BTN_OpenTargetFile->setEnabled(true);
    ui.BTN_RefreshContent->setEnabled(true);
    ui.BTN_WriteConfirm->setEnabled(true);
	ui.statusBar->showMessage(text);
    SwitchStartButtonStat(false);
}

void KeyBrdReflectionWindow::RefreshFileContent() {
    ShowTargetFileInfo();
    emit NoticeRefreshFile();
    ui.statusBar->showMessage("已刷新");
}

void KeyBrdReflectionWindow::SwitchStartButtonStat(bool flag) {
    if (flag)
        ui.BTN_WriteConfirm->setText("取消准备"), ButtonProcessingStat = true;
    else
        ui.BTN_WriteConfirm->setText("准备写入"), ButtonProcessingStat = false;
}