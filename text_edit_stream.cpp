#include "text_edit_stream.hpp"
#include <QTextEdit> // QTextEdit

namespace rn1
{
TextEditStream::TextEditStream(QTextEdit *textEdit, QObject *parent)
    : QObject{ parent },
      m_textEdit{ textEdit },
      m_string{ QStringLiteral("") },
      m_textStream{ &m_string }
{
}

void TextEditStream::addStringToTextEdit()
{
    m_textEdit->append(m_string);
    m_string = QStringLiteral("");
}
} // namespace rn1
