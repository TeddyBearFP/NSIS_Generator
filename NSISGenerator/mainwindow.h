#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGuiApplication>
#include <QMap>
#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

class MainWindow : public QQmlApplicationEngine
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    Q_INVOKABLE void close();

private:
    QObject *mainInst;

private slots:
    void objectCreated( QObject *object, const QUrl & );
};

#endif // MAINWINDOW_H
