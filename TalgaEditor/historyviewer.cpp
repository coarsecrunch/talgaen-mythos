#include "historyviewer.h"
#include <QUndoStack>
#include <QUndoCommand>

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

    void HistoryViewer::sl_addUndoCommand(QUndoCommand *c)
    {
      mStack->push(c);
    }

  }
}


