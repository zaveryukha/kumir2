#ifndef TEXTBLOCKDATA_H
#define TEXTBLOCKDATA_H

#include <QtCore>
#include <QtGui>

#include "interfaces/lexemtype.h"
#include "interfaces/analizerinterface.h"
#include "dataformats/kumfile.h"

namespace Editor {



struct TextLine
{
    inline explicit TextLine() {
        indentStart = indentEnd = 0;
        lineEndSelected = false;
        changed = false;
        inserted = false;
        protecteed = false;
        hidden = false;
    }
    int indentStart;
    int indentEnd;
    QList<Shared::LexemType> highlight;
    QList<bool> selected;
    bool lineEndSelected;
    bool protecteed;
    bool hidden;
    QString text;
    QStringList errors;
    QString marginText;
    bool changed;
    bool inserted;
};

class TextDocument
        : public QObject
{
    Q_OBJECT
    friend class InsertCommand;
    friend class RemoveCommand;
    friend class InsertBlockCommand;
    friend class RemoveBlockCommand;
    friend class ToggleCommentCommand;
    friend class TextCursor;
public:
    // Flag, set on restore session.
    // If present, do not actually undo/redo
    // while pushing actions into undo-stack.
    // Becomes false after session restore done.
    static bool noUndoRedo;

    explicit TextDocument(QObject * parent, QSettings * settings);
    int documentId;
    int indentAt(int lineNo) const;
    inline void setAnalizer(Shared::AnalizerInterface * a) { m_analizer = a; }
    inline bool isProtected(int lineNo) const { return data[lineNo].protecteed; }
    inline void setProtected(int lineNo, bool v) { data[lineNo].protecteed = v; }
    inline bool isHidden(int lineNo) const { return data[lineNo].hidden; }
    inline void setHidden(int lineNo, bool v) { data[lineNo].hidden = v; }
    int hiddenLineStart() const;
    inline int linesCount() const { return data.size(); }
    KumFile::Data toKumFile() const;
    QString toHtml(int fromLine = -1, int toLine = -1) const;
    QString lineToHtml(int lineNo) const;
    void setKumFile(const KumFile::Data & data, bool showHiddenLines);
    void setPlainText(const QString & data);
    inline QString textAt(int index) const { return index<data.size()? data[index].text : ""; }
    inline QString marginTextAt(int index) const { return index<data.size()? data[index].marginText : ""; }
    inline QList<bool> selectionMaskAt(int index) const { return index<data.size()? data[index].selected : QList<bool>(); }
    inline void setSelectionMaskAt(int index, const QList<bool> mask) { if (index<data.size()) data[index].selected = mask; }
    inline bool lineEndSelectedAt(int index) const { return index<data.size()? data[index].lineEndSelected : false; }
    inline QList<Shared::LexemType> highlightAt(int index) { return index<data.size()? data[index].highlight : QList<Shared::LexemType>(); }
    inline void setMarginTextAt(int index, const QString & text) { if (index<data.size()) data[index].marginText = text; }
    inline void setIndentRankAt(int index, const QPoint & rank) { if (index<data.size()) data[index].indentStart = rank.x(); data[index].indentEnd = rank.y(); }
    inline void setHighlightAt(int index, const QList<Shared::LexemType> & highlight) { if (index<data.size()) data[index].highlight = highlight; }
    inline void setErrorsAt(int index, const QStringList & errs) { if (index<data.size()) data[index].errors = errs; }
    inline QStringList errorsAt(int index) const { return index<data.size()? data[index].errors : QStringList(); }
    inline void clearErrorsAt(int index) { if (index<data.size()) data[index].errors.clear(); }

    inline void setSelected(int line, int pos, bool v) { if (line<data.size()) data[line].selected[pos] = v; }
    inline void setEndOfLineSelected(int line, bool v) { if (line<data.size()) data[line].lineEndSelected = v; }
    void evaluateCommand(const QUndoCommand & cmd);
    inline const QUndoStack * undoStack() const { return m_undoStack; }
    inline QUndoStack * undoStack() { return m_undoStack; }
    void removeSelection();
    void forceCompleteRecompilation();
    void flushChanges();
    void flushTransaction();
signals:
    void compilationRequest(const QStack<Shared::ChangeTextTransaction> & changes);
    void completeCompilationRequest(const QStringList & visibleText,
                                    const QStringList & hiddenText,
                                    int hiddenBaseLine);
protected:
    void insertText(const QString & text, const Shared::AnalizerInterface * analizer, int line, int pos, int &blankLines, int &blankChars);
    void removeText(QString &removedText, const Shared::AnalizerInterface * analizer, int line, int pos, int  blankLines, int  blankChars, int count);
    void checkForCompilationRequest(const QPoint & cursorPosition);
private:
    QSet<int> m_removedLines;
    QSet<int> m_removedHiddenLines;
    QStack<Shared::ChangeTextTransaction> changes;
    QPoint lastCursorPos;
    QUndoStack * m_undoStack;
    QList<TextLine> data;
    QString s_hiddenText;
    bool b_wasHiddenText;
    QSettings * m_settings;
    Shared::AnalizerInterface * m_analizer;
};

}

#endif // TEXTBLOCKDATA_H