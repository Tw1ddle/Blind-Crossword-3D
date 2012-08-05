#include "guessworddialog.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <QGridLayout>
#include <QVBoxLayout>

GuessWordDialog::GuessWordDialog(QWidget *parent) :
    QDialog(parent)
{
    wordLabel = new QLabel("Word:");

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    wordText = new QLineEdit;

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(wordLabel, 0, 0);
    gLayout->addWidget(wordText, 0, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 2, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked()),
                this, SLOT(accept()));

    connect(cancelButton, SIGNAL(clicked()),
                this, SLOT(reject()));

    setWindowTitle(tr("Enter word"));
}
