#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "growingfilemodel.h"
#include "logentryitemdelegate.h"
#include "highlighter.h"

#include "QDebug"
#include "QScrollBar"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView_2->hide();
    QAbstractTableModel* model = new GrowingFileModel(this,
        QCoreApplication::arguments().at(1));
    QObject::connect(
        model, SIGNAL(rowsInserted(const QModelIndex&,int,int)),
        this, SLOT(model_rowsInserted(const QModelIndex&,int,int)) // NO on_ !!!!
    );
    Highlighters* pHighlighters = new Highlighters;
    Highlighter* pErrorHiLi = new Highlighter("error");
    pErrorHiLi->SetPattern("error");
    pErrorHiLi->SetTextColor(Qt::red);
    pHighlighters->Add(pErrorHiLi);
    Highlighter* pWarningHiLi = new Highlighter("warning");
    pWarningHiLi->SetPattern("warning");
    pWarningHiLi->SetTextColor(QColor("orange"));
    pHighlighters->Add(pWarningHiLi);
    ui->tableView->setItemDelegate(new LogEntryItemDelegate(ui->tableView, pHighlighters));
    ui->tableView->setModel(model);

    // don't allow to change row heights
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::model_rowsInserted(const QModelIndex & parent, int start, int end) {
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

void MainWindow::scrollToBottom()
{
    ui->tableView->scrollToBottom();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    (void)index;
}
