#include <QCoreApplication>
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStringList>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // 外部模块导入路径（支持独立发布后的手动拷贝目录）
    QStringList importCandidates;
#ifdef MARS_QUICKTOOLS_MODULE_DIR
    importCandidates << QStringLiteral(MARS_QUICKTOOLS_MODULE_DIR);
#endif
    importCandidates << QDir(QCoreApplication::applicationDirPath()).filePath("modules");
    importCandidates << QDir(QCoreApplication::applicationDirPath()).filePath("../modules");
    importCandidates << QDir(QCoreApplication::applicationDirPath()).filePath("../../modules");

    for (const auto& path : std::as_const(importCandidates)) {
        if (QDir(path).exists()) {
            engine.addImportPath(path);
        }
    }

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("MarsQuickToolsExample", "Main");

    return app.exec();
}
