#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "growingfilemodel.h"
#include "logfilemodel.h"
#include "logentryitemdelegate.h"
#include "highlighter.h"

#include "QDebug"
#include "QScrollBar"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    ui->tableView_2->hide();
    QAbstractTableModel* model = new CLogFileModel/*CGrowingFileModel*/(this,
        QCoreApplication::arguments().at(1));
    QObject::connect(
        model, SIGNAL(rowsInserted(const QModelIndex&,int,int)),
        this, SLOT(model_rowsInserted(const QModelIndex&,int,int)) // NO on_ !!!!
    );
    Highlighters* pHighlighters = new Highlighters;
    CHighlighter* pErrorHiLi = new CHighlighter("error");
    pErrorHiLi->SetPattern("error");
    pErrorHiLi->SetTextColor(Qt::red);
    pHighlighters->Add(pErrorHiLi);
    CHighlighter* pWarningHiLi = new CHighlighter("warning");
    pWarningHiLi->SetPattern("warning");
    pWarningHiLi->SetTextColor(QColor("orange"));
    pHighlighters->Add(pWarningHiLi);
    ui->tableView->setItemDelegate(new CLogEntryItemDelegate(ui->tableView, pHighlighters));
    ui->tableView->setModel(model);

    // don't allow to change row heights
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::model_rowsInserted(const QModelIndex & parent, int start, int end) {
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
    // Workaround for age-old bug Qt is too lazy to fix...
    // https://www.qtcentre.org/threads/335-QTreeView-problem-scrolling-to-end
    // But at least we can make use of it and sroll only to the bottom
    // if it was previously at the bottom as well
    QScrollBar* scrollBar = ui->tableView->verticalScrollBar();
    if (scrollBar->value() == scrollBar->maximum()) {
        QTimer::singleShot(0, this, SLOT(scrollToBottom()));
    }
};

void CMainWindow::scrollToBottom()
{
    ui->tableView->scrollToBottom();
}

void CMainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    (void)index;
}
