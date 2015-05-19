#pragma once

#include <QUndoView>

class QUndoCommand;
class QUndoStack;

namespace talga
{
  namespace editor
  {
    class HistoryViewer : public QUndoView
    {
      Q_OBJECT
    public:
      HistoryViewer(QWidget* parent = nullptr);

    public slots:
      void sl_addUndoCommand(QUndoCommand* c);

    protected:
      QUndoStack* mStack;
    };
  }
}
