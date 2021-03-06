#include "bsplumpviewarea.h"

#include <QSplitter>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QtDebug>

#include "model/applicationmodel.h"
#include "widgets/lumpviewfactory.h"
#include "widgets/ilumpviewwidget.h"
#include "widgets/valuesearchwidget.h"
#include "structlumpviewwidget.h"

namespace
{
    enum LumpTableColumn
    {
        LumpNameColumn = 0,
        LumpTypeColumn,

        LumpTableColumnCount
    };
}

BSPLumpViewArea::BSPLumpViewArea(QWidget *parent)
    : QWidget(parent),
      m_pSideTabWidget(new QTabWidget()),
      m_pLumpTable(new QTableWidget()),
      m_pValueSearchWidget(new ValueSearchWidget()),
      m_pDataArea(new QTabWidget())
{
    initDataArea();
    initLumpTable();
    initSideTabWidget();

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setMargin(0);
    hLayout->setSpacing(0);

    QSplitter* splitter = new QSplitter();
    splitter->addWidget(m_pSideTabWidget);
    splitter->addWidget(m_pDataArea);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 7);

    hLayout->addWidget(splitter);
    setLayout(hLayout);
}

void BSPLumpViewArea::updateLumps()
{
    m_pLumpTable->clearContents();
    clearDataArea();

    BSPFileStructure* bspFileStructure = ApplicationModel::globalPointer()->bspFileStructure();

    m_pLumpTable->setRowCount(bspFileStructure->lumpDefCount());

    for ( int lumpIndex = 0; lumpIndex < bspFileStructure->lumpDefCount(); ++lumpIndex )
    {
        QSharedPointer<BSPLumpDef> lumpDef = bspFileStructure->lumpDef(lumpIndex);
        m_pLumpTable->setItem(lumpIndex, LumpNameColumn, new QTableWidgetItem(lumpDef->name()));
        m_pLumpTable->setItem(lumpIndex, LumpTypeColumn, new QTableWidgetItem(BSPLumpDef::lumpTypeNameMap().key(lumpDef->type())));
    }

    m_pValueSearchWidget->populate();
}

void BSPLumpViewArea::initDataArea()
{
    m_pDataArea->tabBar()->setUsesScrollButtons(true);
    m_pDataArea->tabBar()->setMovable(true);
    m_pDataArea->tabBar()->setElideMode(Qt::ElideNone);

    connect(m_pDataArea->tabBar(), &QTabBar::tabCloseRequested, this, &BSPLumpViewArea::handleTabCloseRequest);

    clearDataArea();
}

void BSPLumpViewArea::initSideTabWidget()
{
    m_pSideTabWidget->addTab(m_pLumpTable, tr("&Lumps"));
    m_pSideTabWidget->addTab(m_pValueSearchWidget, tr("&Search"));

    connect(m_pValueSearchWidget, &ValueSearchWidget::searchResultChosen, this, &BSPLumpViewArea::handleSearchResultChosen);
}

void BSPLumpViewArea::initLumpTable()
{
    m_pLumpTable->setColumnCount(LumpTableColumnCount);
    m_pLumpTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pLumpTable->setHorizontalHeaderLabels(QStringList() << tr("Lump") << tr("Type"));
    m_pLumpTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_pLumpTable->verticalHeader()->setHidden(true);
    m_pLumpTable->verticalHeader()->setDefaultSectionSize(20);

    QHeaderView* horizontalHeader = m_pLumpTable->horizontalHeader();
    for ( int section = 0; section < LumpTableColumnCount; ++section )
    {
        horizontalHeader->setSectionResizeMode(section, QHeaderView::Stretch);
    }

    connect(m_pLumpTable, &QTableWidget::cellDoubleClicked, this, &BSPLumpViewArea::handleLumpCellDoubleClicked);
}

void BSPLumpViewArea::clearDataArea()
{
    while ( m_pDataArea->count() > 0 )
    {
        m_pDataArea->removeTab(0);
    }

    updateDataAreaTabs();
}

void BSPLumpViewArea::handleTabCloseRequest(int index)
{
    if ( index < 0 || index >= m_pDataArea->count() )
    {
        return;
    }

    m_pDataArea->removeTab(index);
    updateDataAreaTabs();
}

void BSPLumpViewArea::handleLumpCellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    openTabForLump(row);
}

void BSPLumpViewArea::openTabForLump(int lumpIndex)
{
    if ( lumpIndex < 0 || lumpIndex >= m_pLumpTable->rowCount() )
    {
        return;
    }

    BSPFileStructure* bspFileStructure = ApplicationModel::globalPointer()->bspFileStructure();

    int existingTabIndex = tabIndexForLump(lumpIndex);
    if ( existingTabIndex >= 0 )
    {
        m_pDataArea->setCurrentIndex(existingTabIndex);
        return;
    }

    QTableWidgetItem* item = m_pLumpTable->item(lumpIndex, LumpNameColumn);
    if ( !item )
    {
        return;
    }

    QSharedPointer<BSPLumpDef> lumpDef = bspFileStructure->lumpDef(lumpIndex);
    if ( !lumpDef )
    {
        return;
    }

    ILumpViewWidget* viewWidget = LumpViewFactory(lumpDef).createWidget();
    if ( !viewWidget )
    {
        return;
    }

    loadBSPDataIntoLumpView(lumpDef, viewWidget);

    m_pDataArea->addTab(viewWidget->asWidget(), lumpDef->name());
    m_pDataArea->setCurrentIndex(m_pDataArea->count() - 1);
    m_pDataArea->tabBar()->setTabData(m_pDataArea->count() - 1, QVariant(lumpIndex));
    updateDataAreaTabs();
}

void BSPLumpViewArea::handleSearchResultChosen(quint32 lumpIndex, quint32 itemIndex)
{
    openTabForLump(static_cast<int>(lumpIndex));

    BSPFileStructure* bspFileStructure = ApplicationModel::globalPointer()->bspFileStructure();
    QSharedPointer<BSPLumpDef> lumpDef = bspFileStructure->lumpDef(lumpIndex);

    if ( !lumpDef || lumpDef->type() != BSPLumpDef::LumpType::Struct )
    {
        return;
    }

    QWidget* viewWidget = m_pDataArea->currentWidget();
    if ( !viewWidget )
    {
        return;
    }

    StructLumpViewWidget* structWidget = qobject_cast<StructLumpViewWidget*>(viewWidget);
    if ( !structWidget )
    {
        return;
    }

    structWidget->setCurrentItem(static_cast<int>(itemIndex));
}

void BSPLumpViewArea::loadBSPDataIntoLumpView(const QSharedPointer<BSPLumpDef> &lumpDef, ILumpViewWidget *lumpView)
{
    if ( Q_UNLIKELY(!lumpDef || !lumpView) )
    {
        return;
    }

    BSPFileModel* bspFileModel = ApplicationModel::globalPointer()->bspFileModel();
    BSPDataFragment dataFragment = lumpDef->getDataFragment(*bspFileModel);
    lumpView->loadLumpData(lumpDef, dataFragment);
}

void BSPLumpViewArea::updateDataAreaTabs()
{
    switch ( getPlaceholderTabAction() )
    {
        case PlaceholderCreate:
        {
            m_pDataArea->addTab(createPlaceholderTabContents(), "...");
            m_pDataArea->setTabEnabled(0, false);
            m_pDataArea->setTabsClosable(false);
            break;
        }

        case PlaceholderRemove:
        {
            m_pDataArea->removeTab(0);
            m_pDataArea->setTabsClosable(true);
            break;
        }

        default:
        {
            break;
        }
    }
}

int BSPLumpViewArea::tabIndexForLump(int lumpIndex) const
{
    QTabBar* bar = m_pDataArea->tabBar();

    for ( int tabIndex = 0; tabIndex < bar->count(); ++tabIndex )
    {
        QVariant data = bar->tabData(tabIndex);

        if ( data.type() == QVariant::Int && data.toInt() == lumpIndex )
        {
            return tabIndex;
        }
    }

    return -1;
}

QLabel* BSPLumpViewArea::createPlaceholderTabContents()
{
    QLabel* label = new QLabel(tr("No lump selected."));
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    return label;
}

BSPLumpViewArea::PlaceholderAction BSPLumpViewArea::getPlaceholderTabAction() const
{
    // If there are no tabs at all, create the placeholder.
    if ( m_pDataArea->count() < 1 )
    {
        return PlaceholderCreate;
    }

    // If there's 1 tab, it's valid for it to be either the
    // placeholder or a genuine tab. Either way, do nothing.
    if ( m_pDataArea->count() == 1 )
    {
        return PlaceholderIgnore;
    }

    // If there's more than 1 tab, remove the placeholder if it exists.
    QVariant data = m_pDataArea->tabBar()->tabData(0);
    if ( data.type() == QVariant::Invalid )
    {
        return PlaceholderRemove;
    }

    return PlaceholderIgnore;
}
