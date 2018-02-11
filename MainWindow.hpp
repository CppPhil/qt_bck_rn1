#ifndef INCG_RN1_MAINWINDOW_HPP
#define INCG_RN1_MAINWINDOW_HPP
#include "ui_MainWindow.h" // Ui::MainWindow
#include <cstddef> // std::size_t
#include <vector> // std::vector
#include <future> // std::future
#include <atomic> // std::atomic_bool
#include <mutex> // std::mutex
#include <condition_variable> // std::condition_variable

namespace rn1
{
class TextEditStream;

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::size_t threadCount, QWidget *parent = nullptr);

    ~MainWindow();

signals:
    void newStringSignal(QString str);

private slots:
    void sendButtonClickedSlot();
    void addStringSlot(QString str);
    void pauseButtonClickedSlot();

private:
    Ui::MainWindow m_ui;
    TextEditStream *m_textEditStream;
    std::vector<std::future<void>> m_vec;
    std::atomic_bool m_alive;
    std::mutex m_pauseMutex;
    std::condition_variable m_pauseCv;
    bool m_paused;
};
} // namespace rn1
#endif // INCG_RN1_MAINWINDOW_HPP
