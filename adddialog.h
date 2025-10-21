#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>

class AddDialog : public QDialog {
    Q_OBJECT

public:
    AddDialog(QWidget *parent = nullptr);
    QString getDate() const;
    double getReading() const;
    int getSubscriberId() const;
    QString getSurname() const;
    QString getEmail() const;

private:
    QDateEdit *dateEdit;
    QLineEdit *readingEdit;
    QLineEdit *subscriberIdEdit;
    QLineEdit *surnameEdit;
    QLineEdit *emailEdit;
};

#endif // ADDDIALOG_H