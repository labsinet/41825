#include "adddialog.h"

AddDialog::AddDialog(QWidget *parent) : QDialog(parent) {
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

QString AddDialog::getDate() const { return dateEdit->date().toString("yyyy-MM-dd"); }
double AddDialog::getReading() const { return readingEdit->text().toDouble(); }
int AddDialog::getSubscriberId() const { return subscriberIdEdit->text().toInt(); }
QString AddDialog::getSurname() const { return surnameEdit->text(); }
QString AddDialog::getEmail() const { return emailEdit->text(); }