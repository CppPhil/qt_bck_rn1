#include "MainWindow.hpp" // rn1::MainWindow
#include <QApplication> // QApplication
#include <boost/lexical_cast.hpp> // boost::lexical_cast
#include <cstdlib> // EXIT_FAILURE
#include <cstddef> // std::size_t
#include <iostream> // std::cerr

int main(int argc, char *argv[])
{
    constexpr int expectedArgc{ 2 };
    constexpr std::size_t threadCountIdx{ 1U };

    if (argc != expectedArgc) {
        std::cerr << "Invalid parameter count!\n";
        return EXIT_FAILURE;
    }

    std::size_t threadCount{ 0U };

    try {
        threadCount = boost::lexical_cast<std::size_t>(argv[threadCountIdx]);
    } catch (const boost::bad_lexical_cast &) {
        std::cerr << "Invalid thread count\n";
        return EXIT_FAILURE;
    }

    QApplication a{ argc, argv };
    rn1::MainWindow w{ 3 };
    w.show();

    return a.exec();
}
