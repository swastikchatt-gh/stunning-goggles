#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QPlainTextEdit>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QLabel>
#include <QIcon>
#include <QCloseEvent>
#include <QTextCursor>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("MyGedit - Unsaved Document");
        setWindowIcon(QIcon(":/app-icon")); // use embedded icon

        // Central text editor
        textEdit = new QPlainTextEdit;
        setCentralWidget(textEdit);
        connect(textEdit, &QPlainTextEdit::textChanged, this, &MainWindow::documentModified);
        connect(textEdit, &QPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBar);

        // Create actions
        createActions();
        createMenus();
        createToolBars();
        createStatusBar();

        // Start with an empty document
        currentFile.clear();
        textEdit->setPlainText("");
        setWindowModified(false);
    }

protected:
    void closeEvent(QCloseEvent *event) override
    {
        if (maybeSave()) {
            event->accept();
        } else {
            event->ignore();
        }
    }

private slots:
    void newFile()
    {
        if (maybeSave()) {
            currentFile.clear();
            textEdit->clear();
            setWindowTitle("MyGedit - Unsaved Document");
            setWindowModified(false);
        }
    }

    void open()
    {
        if (maybeSave()) {
            QString fileName = QFileDialog::getOpenFileName(this, "Open File", QString(),
                                "Text Files (*.txt);;All Files (*)");
            if (!fileName.isEmpty())
                loadFile(fileName);
        }
    }

    bool save()   // now returns bool
    {
        if (currentFile.isEmpty()) {
            return saveAs();
        } else {
            return saveFile(currentFile);
        }
    }

    bool saveAs()   // now returns bool
    {
        QString fileName = QFileDialog::getSaveFileName(this, "Save File As", QString(),
                                "Text Files (*.txt);;All Files (*)");
        if (fileName.isEmpty())
            return false;   // cancelled
        return saveFile(fileName);
    }

    void cut() { textEdit->cut(); }
    void copy() { textEdit->copy(); }
    void paste() { textEdit->paste(); }

    void about()
    {
        QMessageBox::about(this, "About MyGedit",
            "MyGedit – A simple Qt6 text editor inspired by gedit.\n\n"
            "Built with Qt6, using the provided icons/image.png as window icon.");
    }

    void documentModified()
    {
        setWindowModified(textEdit->document()->isModified());
    }

    void updateStatusBar()
    {
        QTextCursor cursor = textEdit->textCursor();
        int line = cursor.blockNumber() + 1;
        int col = cursor.columnNumber() + 1;
        statusLabel->setText(tr("Ln %1, Col %2").arg(line).arg(col));
    }

private:
    void createActions()
    {
        newAct = new QAction(QIcon::fromTheme("document-new"), tr("&New"), this);
        newAct->setShortcut(QKeySequence::New);
        connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

        openAct = new QAction(QIcon::fromTheme("document-open"), tr("&Open..."), this);
        openAct->setShortcut(QKeySequence::Open);
        connect(openAct, &QAction::triggered, this, &MainWindow::open);

        saveAct = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), this);
        saveAct->setShortcut(QKeySequence::Save);
        connect(saveAct, &QAction::triggered, this, &MainWindow::save);

        saveAsAct = new QAction(tr("Save &As..."), this);
        saveAsAct->setShortcut(QKeySequence::SaveAs);
        connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

        exitAct = new QAction(tr("&Quit"), this);
        exitAct->setShortcut(QKeySequence::Quit);
        connect(exitAct, &QAction::triggered, this, &QWidget::close);

        cutAct = new QAction(QIcon::fromTheme("edit-cut"), tr("&Cut"), this);
        cutAct->setShortcut(QKeySequence::Cut);
        connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

        copyAct = new QAction(QIcon::fromTheme("edit-copy"), tr("C&opy"), this);
        copyAct->setShortcut(QKeySequence::Copy);
        connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

        pasteAct = new QAction(QIcon::fromTheme("edit-paste"), tr("&Paste"), this);
        pasteAct->setShortcut(QKeySequence::Paste);
        connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

        aboutAct = new QAction(tr("&About"), this);
        connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    }

    void createMenus()
    {
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(newAct);
        fileMenu->addAction(openAct);
        fileMenu->addAction(saveAct);
        fileMenu->addAction(saveAsAct);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAct);

        QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
        editMenu->addAction(cutAct);
        editMenu->addAction(copyAct);
        editMenu->addAction(pasteAct);

        QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(aboutAct);
    }

    void createToolBars()
    {
        QToolBar *toolBar = addToolBar(tr("File"));
        toolBar->addAction(newAct);
        toolBar->addAction(openAct);
        toolBar->addAction(saveAct);
        toolBar->addSeparator();
        toolBar->addAction(cutAct);
        toolBar->addAction(copyAct);
        toolBar->addAction(pasteAct);
    }

    void createStatusBar()
    {
        statusLabel = new QLabel("Ln 1, Col 1");
        statusBar()->addPermanentWidget(statusLabel);
        statusBar()->showMessage(tr("Ready"));
    }

    bool maybeSave()
    {
        if (!textEdit->document()->isModified())
            return true;

        QMessageBox::StandardButton ret = QMessageBox::warning(this, "MyGedit",
            tr("The document has been modified.\nDo you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();   // now save() returns bool
        else if (ret == QMessageBox::Cancel)
            return false;
        return true;
    }

    void loadFile(const QString &fileName)
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "MyGedit",
                tr("Cannot read file %1:\n%2.").arg(fileName, file.errorString()));
            return;
        }

        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        textEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();

        currentFile = fileName;
        setWindowTitle(QString("MyGedit - %1[*]").arg(QFileInfo(currentFile).fileName()));
        textEdit->document()->setModified(false);
        setWindowModified(false);
        statusBar()->showMessage(tr("File loaded"), 2000);
    }

    bool saveFile(const QString &fileName)
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "MyGedit",
                tr("Cannot write file %1:\n%2.").arg(fileName, file.errorString()));
            return false;
        }

        QTextStream out(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        out << textEdit->toPlainText();
        QApplication::restoreOverrideCursor();

        currentFile = fileName;
        setWindowTitle(QString("MyGedit - %1[*]").arg(QFileInfo(currentFile).fileName()));
        textEdit->document()->setModified(false);
        setWindowModified(false);
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    }

    QPlainTextEdit *textEdit;
    QLabel *statusLabel;
    QString currentFile;

    QAction *newAct, *openAct, *saveAct, *saveAsAct, *exitAct;
    QAction *cutAct, *copyAct, *pasteAct;
    QAction *aboutAct;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"
