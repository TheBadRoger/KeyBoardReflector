#include "ProcessingThread.h"
#include <QTextStream>
#include <QThread>
#include <QDebug>

ProcessingThread::ProcessingThread(){
    KeyBrdReflectionCore_Qt::initializer();
}

ProcessingThread::~ProcessingThread(){}

void ProcessingThread::ProcessReflection() {
    char keych;
    QThread::msleep(500);

    RefreshFile();
    pTargetFile->seek(FILE_BEGIN);
    QTextStream FileStream(pTargetFile);
    while (!FileStream.atEnd()) {
        FileStream >> keych;
        UpdateStreamPos(keych);
        emit SendNewStatusText(QString("正在输入字符：%1（第 %2 行，第 %3 列，第 %4 个字符）").arg(CharTranslator(keych)).arg(StreamLine).arg(StreamCol).arg(StreamPos));
        if (isalpha(keych)) {
            if (islower(keych)) {
                KeyBrdReflectionCore_Qt::switch_lower();
                KeyBrdReflectionCore_Qt::input_alphakey(keych);
            }
            else if (isupper(keych)) {
                KeyBrdReflectionCore_Qt::switch_upper();
                KeyBrdReflectionCore_Qt::input_alphakey(keych);
            }
        }
        else if (isdigit(keych)) KeyBrdReflectionCore_Qt::input_digit(keych);
        else if (keych == ' ') KeyBrdReflectionCore_Qt::input_space();
        else if (keych == '\n') KeyBrdReflectionCore_Qt::input_enter();
        else KeyBrdReflectionCore_Qt::input_special(keych);
        QThread::msleep(1);
    }
	emit ProcessFinishSignal("完成");
    QThread::currentThread()->quit();
}

void ProcessingThread::ReceiveFile(QFile* pfile) {
	pTargetFile = pfile;
}

void ProcessingThread::RefreshFile() {
    StreamPos = 0, StreamLine = 1, StreamCol = 0;
    pTargetFile->close();
	pTargetFile->open(QFile::ReadOnly | QFile::Text);
}

QString ProcessingThread::CharTranslator(char ch) {
	if (ch == ' ')return "Space";
    else if(ch=='\n')return "Enter";
	else if (ch == '\t')return "Tab";
    else return QString(ch);
}

void ProcessingThread::UpdateStreamPos(char ch) {
    StreamPos++;
    ch == '\n' ? StreamLine++, StreamCol = 1 : StreamCol++;
}