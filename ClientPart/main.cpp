#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMetaObject>
#include <memory>

#include "Router.h"
#include "TcpClient.h"
#include "Handler.h"
#include "Appcontroller.h"
#include "DialogManager.h"
#include "LoginViewModel.h"
#include "SearchViewModel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    auto state = std::make_unique<AppState>();
    auto router = std::make_unique<Router>();
    auto handler = std::make_unique<Handler>();
    auto client = std::make_unique<TCPClient>(6767, router.get());
    auto appController = std::make_unique<AppController>(router.get(), state.get(), handler.get(), client.get());
    auto dialogManager = std::make_unique<DialogManager>(handler.get(), state.get());

    client->onMessage = [h = handler.get()](const std::string& msg) {
        QMetaObject::invokeMethod(h, [h, msg]() {
            h->handleMessage(msg);
        }, Qt::QueuedConnection);
    };

    client->start();

    auto loginVM = std::make_unique<LoginViewModel>(router.get(), handler.get(), appController.get(), &app);
    auto searchVM = std::make_unique<SearchViewModel>(router.get());
    QObject::connect(handler.get(), &Handler::S_UserSearch, searchVM.get(), &SearchViewModel::onUserSearchResults);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("appController", appController.get());
    engine.rootContext()->setContextProperty("loginViewModel", loginVM.get());
    engine.rootContext()->setContextProperty("searchViewModel", searchVM.get());
    engine.rootContext()->setContextProperty("appState", state.get());

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "[MAIN] Failed to load QML";
        return -1;
    }

    dialogManager->start();

    int result = app.exec();

    return result;
}