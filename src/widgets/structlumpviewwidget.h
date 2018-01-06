#ifndef STRUCTLUMPVIEWWIDGET_H
#define STRUCTLUMPVIEWWIDGET_H

#include <QWidget>

#include "ilumpviewwidget.h"

#include "bsp/structlumpdef.h"

namespace Ui {
class StructLumpViewWidget;
}

class StructLumpViewWidget : public QWidget,
                             public ILumpViewWidget
{
    Q_OBJECT

public:
    explicit StructLumpViewWidget(QWidget *parent = 0);
    ~StructLumpViewWidget();

    // ILumpViewWidget
    virtual QWidget* asWidget() override;
    virtual void loadLumpData(const QSharedPointer<BSPLumpDef> &lumpDef, const BSPDataFragment &fragment) override;

private slots:
    void lumpItemChanged(int item);

private:
    void initTable();
    void setTableMemberDefinitions();
    void updateLumpItemCount();
    void updateUI();
    QByteArray getStructData(quint32 item) const;
    void setNameItem(int row, const QString& memberName);

    Ui::StructLumpViewWidget *ui;
    QSharedPointer<StructLumpDef> m_pStructLumpDef;
    QByteArray m_LumpData;
    quint32 m_nItemCount;
};

#endif // STRUCTLUMPVIEWWIDGET_H