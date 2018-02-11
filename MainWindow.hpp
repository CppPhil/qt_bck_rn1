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
class TextEditStream; // forward declare the TextEditStream type

/*!
 * \brief The type for the main window of the application.
**/
class MainWindow
    : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Creates a MainWindow object.
     * \param threadCount The amount of threads that shall be run.
     * \param parent The Qt parent of the instance to be created.
     *
     * Creates a MainWindow object.
     * Launches the threads that are to be launched.
    **/
    explicit MainWindow(std::size_t threadCount, QWidget *parent = nullptr);

    /*!
     * \brief Destroys a MainWindow object.
     *
     * Destroys a MainWindow object by unpausing all the its paused threads
     * and causing all its threads to return.
    **/
    ~MainWindow();

signals:
    /*!
     * \brief Signal that is to be emitted when a new string shall be added
     *        into the GUI.
     * \param str The QString to add to the GUI.
     * \warning Shall only go through QueuedConnections.
    **/
    void newStringSignal(QString str);

private slots:
    /*!
     * \brief The callback routine for the send button's OnClick event.
     *
     * The callback routine for the send button's OnClick event.
     * Adds the string entered by the user with the associated News-type
     * to the GUI.
    **/
    void sendButtonClickedSlot();

    /*!
     * \brief The slot to add a new QString into the GUI.
     * \param str The QString to add to the GUI.
     * \warning Shall only be connected to through a QueuedConnection.
    **/
    void addStringSlot(QString str);

    /*!
     * \brief Callback routine for the pause button's OnClick event.
     *
     * Callback routine for the pause button's OnClick event, pauses or resumes
     * the threads of this MainWindow object.
    **/
    void pauseButtonClickedSlot();

private:
    Ui::MainWindow m_ui; /*!< The GUI */
    TextEditStream *m_textEditStream; /*!< The stream to use to write to the
                                       *   output QTextEdit.
                                      **/
    std::vector<std::future<void>> m_vec; /*!< Contains the threads */
    std::atomic_bool m_alive; /*!< Atomic flag that indicates whether the
                               *   threads shall consider themselves alive.
                               *   If true the threads shall continue running.
                               *   If false the threads shall return as the
                               *   application is shutting down.
                              **/
    std::mutex m_pauseMutex; /*!< Mutex used for pausing the threads */
    std::condition_variable m_pauseCv; /*!< Condition variable used for pausing
                                        *   the threads.
                                       **/
    bool m_paused; /*!< The paused flag. May only be accessed when holding
                    *   m_pauseMutex. If true indicates, that the threads shall
                    *   pause. If false indicates that the threads shall keep
                    *   running.
                   **/
};
} // namespace rn1
#endif // INCG_RN1_MAINWINDOW_HPP
