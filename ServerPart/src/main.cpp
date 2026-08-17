#include "../include/TcpServer.h"
#include "../include/SignalHandler.h"
#include "../include/Logger.h"

int main() {
    if (const char* logLevel = std::getenv("LOG_LEVEL")) {
        if (const std::string level(logLevel); level == "DEBUG") Logger::instance().setMinLevel(LogLevel::DEBUG);
        else if (level == "INFO") Logger::instance().setMinLevel(LogLevel::INFO);
        else if (level == "WARNING") Logger::instance().setMinLevel(LogLevel::WARNING);
        else if (level == "CRITICAL") Logger::instance().setMinLevel(LogLevel::CRITICAL);
    }

    Logger::instance().enableAllCategories();

    LOG_INFO(MAIN, "Запуск сервера");
    SignalHandler::s_Setup();

    if(const auto error = Config::load("config.json")){
        std::cerr << "Failed to load config: " << *error << '\n';
        return 1;
    }

    TcpServer server(Config::getServer().port);

    std::thread serverThread([&server](){
        if (!server.start())
            std::cerr << "Server failed to start\n";
    });

    while(!SignalHandler::s_isShutdownRequested())
        pause();

    server.stop();

    if(serverThread.joinable())
        serverThread.join();

    return 0;
}