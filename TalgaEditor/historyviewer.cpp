#include "historyviewer.h"
#include <QUndoStack>
#include <QUndoCommand>
#include <QKeyEvent>
#include <QApplication>

namespace talga
{
  namespace editor
  {
    HistoryViewer::HistoryViewer(QWidget* parent)
      : QUndoView(parent)
      , mStack(new QUndoStack())
    {
      setStack(mStack);
    }

    void HistoryViewer::keyPressEvent(QKeyEvent *e)
    {
      if ((e->key() == Qt::Key_Z)  && QApplication::keyboardModifiers() && Qt::ControlModifier)
      {
        mStack->undo();
      }
    }

    void HistoryViewer::keyReleaseEvent(QKeyEvent *e)
    {

    }

    void HistoryViewer::sl_addUndoCommand(QUndoCommand *c)
    {
      mStack->push(c);
    }

  }
}


