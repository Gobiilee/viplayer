#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "viewmodels/PlayerViewModel.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
    QGuiApplication app(argc, argv);

    qmlRegisterType<PlayerViewModel>("VideoPlayer.Core", 1, 0, "PlayerViewModel");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/VideoPlayer/Main.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
