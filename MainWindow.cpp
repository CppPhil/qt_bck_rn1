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
      m_textEditStream{ nullptr }, // delay initialization, as the GUI needs to
                                   // be set up first ...
      m_vec{ },
      m_alive{ true }, // by default the application runs.
      m_pauseMutex{ },
      m_pauseCv{ },
      m_paused{ false } // by default the threads are not paused.
{
    // constants for the wait times.
    static constexpr unsigned long long minSecondsToWait{ 1ULL };
    static constexpr unsigned long long maxSecondsToWait{ 5ULL };

    // setup the GUI.
    m_ui.setupUi(this);

    // create the TextEditStream, with this object as the parent.
    m_textEditStream = new TextEditStream{ m_ui.outputTextEdit, PL_PARENT(this) };

    // connect the newStringSignal to the addStringSlot.
    // QueuedConnection ensures addStringSlot is called in a thread safe manner.
    connect(
        this, &MainWindow::newStringSignal,
        this, &MainWindow::addStringSlot,
        Qt::QueuedConnection);

    // connect the pause button's OnClick event to the associated callback routine.
    connect(
        m_ui.pausePushButton, &QPushButton::clicked,
        this, &MainWindow::pauseButtonClickedSlot);

    // connect the send button's OnClick event to the associated callback routine.
    connect(
        m_ui.sendPushButton, &QPushButton::clicked,
        this, &MainWindow::sendButtonClickedSlot);

    // Create the threads.
    for (std::size_t i{ 0U }; i < threadCount; ++i) {
        // std::launch::async to force thread creation.
        // The std::future<void>s get 'joined' in their destructors
        // as they're created using std::async.
        m_vec.push_back(std::async(std::launch::async,
        [this] { // lambda as the thread function.
            // while the application runs -> run the threads.
            while (std::atomic_load_explicit(&m_alive, std::memory_order_seq_cst)) {
                {
                    // lock the pause mutex
                    std::unique_lock<std::mutex> l{ m_pauseMutex };

                    // wait until we're unpaused.
                    m_pauseCv.wait(l, [this]{ return not m_paused; });
                } // unlock the pause mutex

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

                // emit the signal, which is connected to the associated
                // slot using a QueuedConnection.
                emit newStringSignal(QString::fromStdString(message));
            }
        }));
    }
}

MainWindow::~MainWindow()
{
    {
        // lock the mutex.
        std::unique_lock<std::mutex> l{ m_pauseMutex };

        // unpause the threads.
        m_paused = false;
    } // unlock the mutex

    // wake all the sleeping threads.
    m_pauseCv.notify_all();

    // set the atomic boolean flag to false to indicate that the application
    // is shutting down, so that the threads return.
    std::atomic_store_explicit(&m_alive, false, std::memory_order_seq_cst);
}

void MainWindow::sendButtonClickedSlot()
{
    // Create the string to add to the GUI.
    QString str{ m_ui.newsComboBox->currentText() };
    str.push_back(QChar{ ' ' });
    str += m_ui.inputTextEdit->toPlainText();

    // Clear the input text edit
    m_ui.inputTextEdit->setText(QStringLiteral(""));

    // emit the signal, that is connected using QueuedConnection.
    emit newStringSignal(std::move(str));
}

void MainWindow::pauseButtonClickedSlot()
{
    {
        // lock the pause mutex
        std::unique_lock<std::mutex> l{ m_pauseMutex };

        m_paused = not m_paused; // toggle the paused flag.
    } // unlock the mutex

    // wake all the threads so that they call their lambda
    // that was put into the cv.wait call
    // so that they resume running if they were paused.
    m_pauseCv.notify_all();

    // switch the button's text.
    if (m_ui.pausePushButton->text() == QStringLiteral("pause")) {
        m_ui.pausePushButton->setText(QStringLiteral("resume"));
    } else {
        m_ui.pausePushButton->setText(QStringLiteral("pause"));
    }
}

void MainWindow::addStringSlot(QString str)
{
    (*m_textEditStream) << str; // write str to the GUI.
}
} // namespace rn1
