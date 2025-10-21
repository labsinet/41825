#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QInputDialog>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QLabel>

class AddDialog : public QDialog {
    Q_OBJECT

public:
    AddDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Add Reading");
        setModal(true);

        QFormLayout *layout = new QFormLayout(this);

        dateEdit = new QDateEdit(this);
        dateEdit->setDate(QDate::currentDate());
        layout->addRow("Date:", dateEdit);

        readingEdit = new QLineEdit(this);
        layout->addRow("Reading:", readingEdit);

        subscriberIdEdit = new QLineEdit(this);
        layout->addRow("Subscriber ID:", subscriberIdEdit);

        surnameEdit = new QLineEdit(this);
        layout->addRow("Surname:", surnameEdit);

        emailEdit = new QLineEdit(this);
        layout->addRow("Email:", emailEdit);

        QPushButton *okButton = new QPushButton("OK", this);
        QPushButton *cancelButton = new QPushButton("Cancel", this);
        layout->addRow(okButton, cancelButton);

        connect(okButton, &QPushButton::clicked, this, &AddDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &AddDialog::reject);
    }

    QString getDate() const { return dateEdit->date().toString("yyyy-MM-dd"); }
    double getReading() const { return readingEdit->text().toDouble(); }
    int getSubscriberId() const { return subscriberIdEdit->text().toInt(); }
    QString getSurname() const { return surnameEdit->text(); }
    QString getEmail() const { return emailEdit->text(); }

private:
    QDateEdit *dateEdit;
    QLineEdit *readingEdit;
    QLineEdit *subscriberIdEdit;
    QLineEdit *surnameEdit;
    QLineEdit *emailEdit;
};

class ReportDialog : public QDialog {
    Q_OBJECT

public:
    ReportDialog(int subscriberId = -1, QWidget *parent = nullptr) : QDialog(parent) {
        setModal(true);
        QVBoxLayout *layout = new QVBoxLayout(this);

        QTableView *tableView = new QTableView(this);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        QSqlQueryModel *model = new QSqlQueryModel(this);
        QSqlQuery query;

        if (subscriberId != -1) {
            setWindowTitle("Subscriber Report");
            query.prepare("SELECT date, reading, subscriber_id, surname, email FROM readings WHERE subscriber_id = ? ORDER BY date");
            query.addBindValue(subscriberId);
        } else {
            setWindowTitle("Full Report");
            query.prepare("SELECT date, reading, subscriber_id, surname, email FROM readings ORDER BY date");
        }

        if (!query.exec()) {
            QMessageBox::critical(this, "Error", "Failed to fetch data: " + query.lastError().text());
            return;
        }

        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, "Date");
        model->setHeaderData(1, Qt::Horizontal, "Reading");
        model->setHeaderData(2, Qt::Horizontal, "Subscriber ID");
        model->setHeaderData(3, Qt::Horizontal, "Fullname");
        model->setHeaderData(4, Qt::Horizontal, "Email");

        tableView->setModel(model);
        layout->addWidget(tableView);

        if (model->rowCount() == 0) {
            QLabel *noDataLabel = new QLabel("No data available.", this);
            layout->addWidget(noDataLabel);
        }

        QPushButton *closeButton = new QPushButton("Close", this);
        layout->addWidget(closeButton);
        connect(closeButton, &QPushButton::clicked, this, &ReportDialog::accept);

        resize(600, 400);
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Gas Meter Accounting");

        // Initialize database
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("gas.db");
        if (!db.open()) {
            QMessageBox::critical(this, "Error", "Failed to open database: " + db.lastError().text());
            return;
        }

        QSqlQuery query;
        if (!query.exec("CREATE TABLE IF NOT EXISTS readings ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "date TEXT, "
                        "reading REAL, "
                        "subscriber_id INTEGER, "
                        "surname TEXT, "
                        "email TEXT"
                        ")")) {
            QMessageBox::critical(this, "Error", "Failed to create table: " + query.lastError().text());
        }

        // Menu
        QMenu *menu = menuBar()->addMenu("Menu");

        QAction *addAction = new QAction("Add Reading", this);
        connect(addAction, &QAction::triggered, this, &MainWindow::addReading);
        menu->addAction(addAction);

        QAction *subscriberReportAction = new QAction("Subscriber Report", this);
        connect(subscriberReportAction, &QAction::triggered, this, &MainWindow::showSubscriberReport);
        menu->addAction(subscriberReportAction);

        QAction *fullReportAction = new QAction("Full Report", this);
        connect(fullReportAction, &QAction::triggered, this, &MainWindow::showFullReport);
        menu->addAction(fullReportAction);

        QAction *aboutAction = new QAction("About", this);
        connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
        menu->addAction(aboutAction);
    }

private slots:
    void addReading() {
        AddDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            QSqlQuery query;
            query.prepare("INSERT INTO readings (date, reading, subscriber_id, surname, email) VALUES (?, ?, ?, ?, ?)");
            query.addBindValue(dialog.getDate());
            query.addBindValue(dialog.getReading());
            query.addBindValue(dialog.getSubscriberId());
            query.addBindValue(dialog.getSurname());
            query.addBindValue(dialog.getEmail());

            if (!query.exec()) {
                QMessageBox::critical(this, "Error", "Failed to add reading: " + query.lastError().text());
            } else {
                QMessageBox::information(this, "Success", "Reading added successfully.");
            }
        }
    }

    void showSubscriberReport() {
        bool ok;
        int subscriberId = QInputDialog::getInt(this, "Subscriber Report", "Enter Subscriber ID:", 0, 0, 2147483647, 1, &ok);
        if (ok) {
            ReportDialog dialog(subscriberId, this);
            dialog.exec();
        }
    }

    void showFullReport() {
        ReportDialog dialog(-1, this);
        dialog.exec();
    }

    void showAbout() {
        QMessageBox::about(this, "About", "Gas Meter Accounting Program");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
