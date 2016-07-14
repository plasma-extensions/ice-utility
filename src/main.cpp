#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QtQuick/QQuickView>
#include <QtQml>
#include <QDebug>

#include "launchermodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("Ice Utility");
    QCoreApplication::setOrganizationDomain("com.plasma_light.ice_utility");

    QCommandLineParser parser;
    parser.setApplicationDescription("Ice Utility");
    parser.addHelpOption();
    QCommandLineOption targetUrlOption(QStringList() << "u" << "url",
                                       QCoreApplication::translate("main", "Url to be loaded."),
                                       QCoreApplication::translate("main", "url"));
    parser.addOption(targetUrlOption);
    parser.process(app);


    qmlRegisterType<LauncherModel>("com.plasma_light.ice_utility", 1, 0, "Launchers");

    QQuickView quickView;
    // Setup view

    QString url = parser.value(targetUrlOption);
    if (!url.isEmpty()) {
        qDebug() << url;
        quickView.rootContext()->setContextProperty("baseUrl", url);
        quickView.rootContext()->setContextProperty("rootWindow", &quickView);
        quickView.setSource(QUrl(QStringLiteral("qrc:/qml/SSB.qml")));
    } else {
        quickView.setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    }
    quickView.setResizeMode(QQuickView::SizeRootObjectToView);
    quickView.show();

    return app.exec();
}
