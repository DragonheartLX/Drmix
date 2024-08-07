#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) 
    {
        const QString baseName = "Drmix_" + QLocale(locale).name();
        if (translator.load(":/i18n/"+ baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }
    
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/Drmix/App.qml"));
    QObject::connect
    (
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, 
        Qt::QueuedConnection
    );
    
    engine.load(url);

    return app.exec();
}