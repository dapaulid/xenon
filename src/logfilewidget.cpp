#include <QScrollBar>
#include <QDebug>

#include "logfilewidget.h"
#include "ui_logfilewidget.h"

#include "logfilemodel.h"
#include "logentryitemdelegate.h"
#include "highlighter.h"


CLogFileWidget::CLogFileWidget(QWidget *parent, const QString& fileName):
    QWidget(parent),
    ui(new Ui::CLogFileWidget),
    m_LogFile(fileName),
    m_FilterModel()
{
    ui->setupUi(this);

    m_FilterModel.setSourceModel(new CLogFileModel(this, m_LogFile));
    m_FilterModel.setFilterKeyColumn(m_FilterModel.columnCount()-1);
    QObject::connect(
        &m_FilterModel, SIGNAL(rowsInserted(const QModelIndex&,int,int)),
        this, SLOT(model_rowsInserted(const QModelIndex&,int,int)) // NO on_ !!!!
    );

    // connect filter controls
    connect(ui->cbxFilter, SIGNAL(activated(int)), this, SLOT(applyFilter()));
    connect(ui->btnFilter, SIGNAL(clicked()), this, SLOT(applyFilter()));

    // init highlighters
    Highlighters* pHighlighters = new Highlighters;
    CHighlighter* pErrorHiLi = new CHighlighter("error");
    pErrorHiLi->SetPattern("error");
    pErrorHiLi->SetTextColor(Qt::red);
    pHighlighters->Add(pErrorHiLi);
    CHighlighter* pWarningHiLi = new CHighlighter("warning");
    pWarningHiLi->SetPattern("warning");
    pWarningHiLi->SetTextColor(QColor("orange"));
    pHighlighters->Add(pWarningHiLi);

    // set tableview proportions
    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 1);

    // init master tableview
    ui->tblMaster->setItemDelegate(new CLogEntryItemDelegate(ui->tblMaster, pHighlighters));
    ui->tblMaster->setModel(&m_FilterModel);

    // don't allow to change row heights
    ui->tblMaster->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // init filtered tableview
}

CLogFileWidget::~CLogFileWidget()
{
    delete ui;
}

void CLogFileWidget::applyFilter()
{
    m_FilterModel.setFilterRegularExpression(ui->cbxFilter->currentText());
ui->lblCount->setText(QString("%1 of %2 lines").arg(m_FilterModel.rowCount()).arg(m_LogFile.getEntryCount()));
}

void CLogFileWidget::model_rowsInserted(const QModelIndex & parent, int start, int end) {
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
    // Workaround for age-old bug Qt is too lazy to fix...
    // https://www.qtcentre.org/threads/335-QTreeView-problem-scrolling-to-end
    // But at least we can make use of it and sroll only to the bottom
    // if it was previously at the bottom as well
    QScrollBar* scrollBar = ui->tblMaster->verticalScrollBar();
    if (scrollBar->value() == scrollBar->maximum()) {
        QTimer::singleShot(0, this, SLOT(scrollToBottom()));
    }
};

void CLogFileWidget::scrollToBottom()
{
    ui->tblMaster->scrollToBottom();
}
