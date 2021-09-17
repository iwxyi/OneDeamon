#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>
#include <QThread>
#include <conio.h>

bool isRunning(const QString& appName)
{
    QProcess process;
    process.start("tasklist", QStringList() << "-fi" << "imagename eq " + appName);
    process.waitForFinished();
    QString tasksList = QString::fromLocal8Bit(process.readAllStandardOutput());
    // qDebug() << tasksList << tasksList.contains(appName) << tasksList.split("\n").size();
    // if(tasksList.contains(appName)) // 过长的名称会省略，比如 app.e
    if (tasksList.split("\n").size() > 2)
    {
        return true;
    }
    return false;
}

bool openApplication(const QString& path)
{
    QProcess process;
    process.startDetached(path);
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString dir = QCoreApplication::applicationDirPath();
    QString baseName = QCoreApplication::applicationName(); // 不带exe
    baseName.replace(QRegularExpression("_?(Deamon|DEAMON|deamon)$"), "");
    baseName = baseName + ".exe";
    QString appPath = QDir(dir).absoluteFilePath(baseName);
    qInfo() << "target:" << appPath;

    while (true)
    {
        if (!isRunning(baseName))
        {
            qInfo() << "open:" << appPath;
            openApplication(appPath);
        }

        if(kbhit())
            break;

        QThread::sleep(3); // 不清楚几秒合适呀
    }

    return 0;
}
