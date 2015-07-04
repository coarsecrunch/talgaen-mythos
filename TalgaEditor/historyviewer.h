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


      virtual void keyPressEvent(QKeyEvent* e) override;
      virtual void keyReleaseEvent(QKeyEvent* e) override;

    public slots:
      void sl_addUndoCommand(QUndoCommand* c);
      void sl_updateHistoryMacro(bool startNewHistoryItem);
    protected:
      QUndoStack* mStack;
    };
  }
}
