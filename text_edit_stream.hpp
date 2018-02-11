#ifndef INCG_RN1_TEXT_EDIT_STREAM_HPP
#define INCG_RN1_TEXT_EDIT_STREAM_HPP
#include <QString> // QString
#include <QTextStream> // QTextStream
#include <ciso646> // not
#include <string> // std::string
#include <utility> // std::forward
#include <type_traits> // std::remove_reference_t, std::remove_cv_t, std::enable_if_t, std::is_same

class QTextEdit;

namespace rn1
{
class TextEditStream
    : public QObject
{
    Q_OBJECT

public:
    explicit TextEditStream(QTextEdit *textEdit, QObject *parent = nullptr);

    template <typename Ty>
    friend auto operator<<(TextEditStream &textEditStream, Ty &&ty)
        -> std::enable_if_t<
            not std::is_same<
                std::remove_cv_t<std::remove_reference_t<Ty>>, std::string
            >::value, TextEditStream &>
    {
        textEditStream.m_textStream << std::forward<Ty>(ty);
        textEditStream.addStringToTextEdit();
        return textEditStream;
    }

    friend TextEditStream &operator<<(
        TextEditStream &textEditStream,
        const std::string &str)
    {
        textEditStream.m_textStream << str.data();
        textEditStream.addStringToTextEdit();
        return textEditStream;
    }

private:
    void addStringToTextEdit();

    QTextEdit *m_textEdit;
    QString m_string;
    QTextStream m_textStream;
};
} // namespace rn1
#endif // INCG_RN1_TEXT_EDIT_STREAM_HPP
