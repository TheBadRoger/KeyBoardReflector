#include "KeyBrdReflectionWindow.h"
#include "../KeyBrdReflectionCore_Qt/KeyBrdReflectionCore_Qt.h"

KeyBrdReflectionWindow::KeyBrdReflectionWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

KeyBrdReflectionWindow::~KeyBrdReflectionWindow()
{}

void KeyBrdReflectionWindow::GetTargetFileName() {
    QString tPath = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"),
        LastDir.path(), "All Files(*.*)", nullptr, QFileDialog::DontResolveSymlinks);

    if (!tPath.isEmpty())
        LastDir = TargetFileName = tPath;

    ShowTargetFileInfo();
}

void KeyBrdReflectionWindow::ShowTargetFileInfo() {
    TargetFile.setFileName(TargetFileName);

    if (!TargetFile.fileName().isEmpty() && !TargetFile.open(QFile::ReadOnly | QFile::Text))
        QMessageBox::critical(this, QString("错误"), QString("打开文件 \"%1\" 失败").arg(TargetFileName));
    else {
        if (ui.BOX_ShowPath->text() != TargetFileName) ui.BOX_ShowPath->setText(TargetFileName);
        QTextStream ShowFileContent(&TargetFile);
        ui.BOX_ShowContent->setText(ShowFileContent.readAll());
    }
}

void KeyBrdReflectionWindow::StartReflectKeys() {
    QTextStream FileStream(&TargetFile);
    char keych;
    DisruptFlag = false;
   
    ProcessingStat();

    while (isActiveWindow()) QCoreApplication::processEvents(QEventLoop::AllEvents);
    WellDelay(500);

    FileStream.seek(FILE_BEGIN);
    while (!FileStream.atEnd() && !DisruptFlag) {
        FileStream >> keych;
        ProcessKey(keych);
        WellDelay(InputTime);

        if(isActiveWindow()) QCoreApplication::processEvents(QEventLoop::AllEvents);
    }

    noProcessingStat();
    FileStream.flush();
}

void KeyBrdReflectionWindow::DisruptInput() {
    DisruptFlag = true;
    noProcessingStat();
}

void KeyBrdReflectionWindow::ProcessKey(char tkey) {
    KeyBrdReflectionCore_Qt KeyCore;
    KeyCore.initializer();

    if (isalpha(tkey)) {
        if (islower(tkey)) {
            KeyCore.switch_lower();
            KeyCore.input_alphakey(tkey);
        }
        else if (isupper(tkey)) {
            KeyCore.switch_upper();
            KeyCore.input_alphakey(tkey);
        }
    }
    else if (isdigit(tkey)) KeyCore.input_digit(tkey);
    else if (tkey == ' ') KeyCore.input_space();
    else if (tkey == '\n') KeyCore.input_enter();
    else KeyCore.input_special(tkey);
}

void KeyBrdReflectionWindow::ProcessingStat() {
    ui.BTN_WriteConfirm->setEnabled(false);
    ui.BOX_InputTime->setEnabled(false);
    ui.BOX_ShowContent->setEnabled(false);
    ui.BOX_ShowPath->setEnabled(false);
	ui.BTN_OpenTargetFile->setEnabled(false);
}

void KeyBrdReflectionWindow::noProcessingStat() {
    ui.BTN_WriteConfirm->setEnabled(true);
    ui.BTN_OpenTargetFile->setEnabled(true);
    ui.BOX_InputTime->setEnabled(true);
    ui.BOX_ShowContent->setEnabled(true);
    ui.BOX_ShowPath->setEnabled(true);
}

void KeyBrdReflectionWindow::SetInputTime() {
    bool op_valid = true;
    double tmpVal = ui.BOX_InputTime->text().toDouble(&op_valid);
    if (op_valid) {
        tmpVal = max(1, tmpVal);
        InputTime = tmpVal;
        ShowTimeOnBox();
    }
    else if (ui.BOX_InputTime->text().isEmpty()) ShowTimeOnBox();
    else QMessageBox::critical(this, QString("错误"), QString("值 %1 不合法").arg(ui.BOX_InputTime->text()));
}

void KeyBrdReflectionWindow::ShowTimeOnBox() {
    ui.BOX_InputTime->setText(QString::number(InputTime));
}

void KeyBrdReflectionWindow::WellDelay(unsigned ms) {
    QTime _Timer = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}