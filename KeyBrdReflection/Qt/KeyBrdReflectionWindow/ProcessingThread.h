#pragma once

#include "KeyBrdReflectionCore_Qt.h"
#include <QObject>
#include <QFile>

class ProcessingThread : public QObject
{
	Q_OBJECT

public:
	ProcessingThread();
	~ProcessingThread();

public slots:
	void ReceiveFile(QFile* pfile);
	void RefreshFile();
	void ProcessReflection();

signals:
	void ProcessFinishSignal(QString text);
	void SendNewStatusText(QString text);

private:
	QFile* pTargetFile = nullptr;
	qint64 StreamPos = 0, StreamLine = 1, StreamCol = 0;
	QString CharTranslator(char ch);
	void UpdateStreamPos(char ch);
};

