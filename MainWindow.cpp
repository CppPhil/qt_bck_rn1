#include "MainWindow.hpp"
#include "text_edit_stream.hpp" // rn1::TextEditStream
#include "rng.hpp" // rn1::rng
#include "news.hpp" // rn1::News
#include <pl/annotations.hpp> // PL_PARENT
#include <boost/lexical_cast.hpp> // boost::lexical_cast
#include <ciso646> // not
#include <string> // std::string
#include <utility> // std::move
#include <type_traits> // std::underlying_type_t
#include <thread> // std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds

namespace rn1
{
MainWindow::MainWindow(std::size_t threadCount, QWidget *parent)
    : QMainWindow{ parent },
      m_textEditStream{ nullptr },
      m_vec{ },
      m_alive{ true },
      m_pauseMutex{ },
      m_pauseCv{ },
      m_paused{ false }
{
    static constexpr unsigned long long minSecondsToWait{ 1ULL };
    static constexpr unsigned long long maxSecondsToWait{ 5ULL };

    m_ui.setupUi(this);

    m_textEditStream = new TextEditStream{ m_ui.outputTextEdit, PL_PARENT(this) };

    connect(
        this, &MainWindow::newStringSignal,
        this, &MainWindow::addStringSlot,
        Qt::QueuedConnection);

    connect(
        m_ui.pausePushButton, &QPushButton::clicked,
        this, &MainWindow::pauseButtonClickedSlot);

    connect(
        m_ui.sendPushButton, &QPushButton::clicked,
        this, &MainWindow::sendButtonClickedSlot);

    for (std::size_t i{ 0U }; i < threadCount; ++i) {
        m_vec.push_back(std::async(std::launch::async,
        [this] {
            while (std::atomic_load_explicit(&m_alive, std::memory_order_seq_cst)) {
                {
                    std::unique_lock<std::mutex> l{ m_pauseMutex };
                    m_pauseCv.wait(l, [this]{ return not m_paused; });
                }

                std::string message{ boost::lexical_cast<std::string>(
                    rng().generate<int>())
                };
                const News news{ static_cast<News>(
                    rng().generate<std::underlying_type_t<News>>(
                        static_cast<std::underlying_type_t<News>>(News::info),
                        static_cast<std::underlying_type_t<News>>(News::corr)))
                };

                message = boost::lexical_cast<std::string>(news) + " " + message;

                const std::chrono::seconds secondsToWait{
                    rng().generate<unsigned long long>(
                        minSecondsToWait, maxSecondsToWait)
                };

                std::this_thread::sleep_for(secondsToWait);

                emit newStringSignal(QString::fromStdString(message));
            }
        }));
    }
}

MainWindow::~MainWindow()
{
    {
        std::unique_lock<std::mutex> l{ m_pauseMutex };
        m_paused = false;
    }
    m_pauseCv.notify_all();

    std::atomic_store_explicit(&m_alive, false, std::memory_order_seq_cst);
}

void MainWindow::sendButtonClickedSlot()
{
    QString str{ m_ui.newsComboBox->currentText() };
    str.push_back(QChar{ ' ' });
    str += m_ui.inputTextEdit->toPlainText();

    m_ui.inputTextEdit->setText(QStringLiteral(""));
    emit newStringSignal(std::move(str));
}

void MainWindow::pauseButtonClickedSlot()
{
    {
        std::unique_lock<std::mutex> l{ m_pauseMutex };
        m_paused = not m_paused;
    }
    m_pauseCv.notify_all();

    if (m_ui.pausePushButton->text() == QStringLiteral("pause")) {
        m_ui.pausePushButton->setText(QStringLiteral("resume"));
    } else {
        m_ui.pausePushButton->setText(QStringLiteral("pause"));
    }
}

void MainWindow::addStringSlot(QString str)
{
    (*m_textEditStream) << str;
}
} // namespace rn1
