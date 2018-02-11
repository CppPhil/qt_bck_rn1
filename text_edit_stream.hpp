#ifndef INCG_RN1_TEXT_EDIT_STREAM_HPP
#define INCG_RN1_TEXT_EDIT_STREAM_HPP
#include <QString> // QString
#include <QTextStream> // QTextStream
#include <ciso646> // not
#include <string> // std::string
#include <utility> // std::forward
#include <type_traits> // std::remove_reference_t, std::remove_cv_t, std::enable_if_t, std::is_same

class QTextEdit; // forward declare QTextEdit

namespace rn1
{
/*!
 * \brief A TextEditStream to 'print' to a QTextEdit.
**/
class TextEditStream
    : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Creates a TextEditStream object.
     * \param textEdit Pointer to the QTextEdit to print to.
     * \param parent The parent of this object.
    **/
    explicit TextEditStream(QTextEdit *textEdit, QObject *parent = nullptr);

    /*!
     * \brief Prints an object to the underlying QTextEdit.
     * \param textEditStream The TextEditStream to print to.
     * \param ty The object to print.
     * \return A reference to 'textEditStream'
     * \note SFINAE'd away if Ty is a std::string type.
    **/
    template <typename Ty>
    friend auto operator<<(TextEditStream &textEditStream, Ty &&ty)
        -> std::enable_if_t<
            not std::is_same<
                std::remove_cv_t<std::remove_reference_t<Ty>>, std::string
            >::value, TextEditStream &>
    {
        textEditStream.m_textStream << std::forward<Ty>(ty); // print it
        textEditStream.addStringToTextEdit(); // show it
        return textEditStream;
    }

    /*!
     * \brief Prints a std::string to the underlying QTextEdit.
     * \param textEditStream The TextEditStream to print to.
     * \param str The std::string to print.
     * \return A reference to 'textEditStream'.
    **/
    friend TextEditStream &operator<<(
        TextEditStream &textEditStream,
        const std::string &str)
    {
        textEditStream.m_textStream << str.data(); // print it
        textEditStream.addStringToTextEdit(); // show it
        return textEditStream;
    }

private:
    /*!
     * \brief Implementation function to add the current string to the QTextEdit.
    **/
    void addStringToTextEdit();

    QTextEdit *m_textEdit; /*!< Pointer to the QTextEdit in the GUI */
    QString m_string; /*!< The current string */
    QTextStream m_textStream; /*!< The underlying QTextStream */
};
} // namespace rn1
#endif // INCG_RN1_TEXT_EDIT_STREAM_HPP
