#include "text_edit_stream.hpp"
#include <QTextEdit> // QTextEdit

namespace rn1
{
TextEditStream::TextEditStream(QTextEdit *textEdit, QObject *parent)
    : QObject{ parent },
      m_textEdit{ textEdit },
      m_string{ QStringLiteral("") }, // start as an empty string
      m_textStream{ &m_string } // write to m_string
{
}

void TextEditStream::addStringToTextEdit()
{
    m_textEdit->append(m_string); // add the string to the GUI
    m_string = QStringLiteral(""); // clear the string.
}
} // namespace rn1
