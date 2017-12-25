#ifndef BSPLUMPVIEWAREA_H
#define BSPLUMPVIEWAREA_H

#include <QWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QLabel>

#include "bsp/bspfilestructure.h"

class BSPLumpViewArea : public QWidget
{
    Q_OBJECT
public:
    explicit BSPLumpViewArea(QWidget *parent = nullptr);

    void updateLumps(const BSPFileStructure &bspFileStructure);

signals:

public slots:

private slots:
    void handleTabCloseRequest(int index);
    void handleLumpCellDoubleClicked(int row, int column);

private:
    static QLabel* createPlaceholderTabContents();

    void initDataArea();
    void initLumpTable();
    void clearLumpTable();
    void clearDataArea();
    void updateDataAreaTabs();
    bool lumpTabExists(int lumpIndex) const;

    QTableWidget* m_pLumpTable;
    QTabWidget* m_pDataArea;
    bool m_bPlaceholderActive;
};

#endif // BSPLUMPVIEWAREA_H