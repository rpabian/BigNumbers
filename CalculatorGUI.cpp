#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <C:\Users\itopa\source\repos\BigNumbers\BigNumbers\BigNumbersLib.h>
#include <QDebug>

double firstNum;

//std strings to insert into the BigNumbers classes
std::string firstStringNum;
std::string secondStringNum;

bool userIsTypingSecondNumber = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_0,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_1,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_2,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_3,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_4,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_5,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_6,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_7,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_8,SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->pushButton_9,SIGNAL(released()), this, SLOT(digitPressed()));

    connect(ui->AddButton,SIGNAL(released()), this, SLOT(binaryOperationPressed()));
    connect(ui->SubButton,SIGNAL(released()), this, SLOT(binaryOperationPressed()));
    connect(ui->DivideButton,SIGNAL(released()), this, SLOT(binaryOperationPressed()));
    connect(ui->MultButton,SIGNAL(released()), this, SLOT(binaryOperationPressed()));
    //connect(ui->ModButton,SIGNAL(released()), this, SLOT(binaryOperationPressed()));

    ui->AddButton->setCheckable(true);
    ui->SubButton->setCheckable(true);
    ui->DivideButton->setCheckable(true);
    ui->MultButton->setCheckable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digitPressed()
{
    QPushButton* button = (QPushButton*)sender();

    double labelNumber;
    QString newLabel;

    bool buttonIsChecked = false;

    if(ui->AddButton->isChecked() || ui->SubButton->isChecked()
            || ui->DivideButton->isChecked() || ui->MultButton->isChecked())
        buttonIsChecked = true;

    if(buttonIsChecked && (!userIsTypingSecondNumber))
    {
        labelNumber = button->text().toDouble();
        userIsTypingSecondNumber = true;
    }
    else
    {
        labelNumber = (ui->label->text() + button->text()).toDouble();
    }

    newLabel = QString::number(labelNumber, 'g', 15);

    //if operator has been checked fill in second string, otherwise fill in first string
    if(!buttonIsChecked)
        firstStringNum = newLabel.toStdString();
    else {
        secondStringNum = newLabel.toStdString();
    }

    ui->label->setText(newLabel);
}

void MainWindow::on_Clear_released()
{
    ui->AddButton->setChecked(false);
    ui->SubButton->setChecked(false);
    ui->DivideButton->setChecked(false);
    ui->MultButton->setChecked(false);

    userIsTypingSecondNumber = false;

    firstStringNum = "";
    secondStringNum = "";

    ui->label->setText("0");
}

void MainWindow::on_EqualButton_released()
{
    double secondNum;
    QString newLabel;

    secondNum = ui->label->text().toDouble();

    if(ui->AddButton->isChecked())
    {
        //using my library here
        BigNumbers::BigNumbers firstBigNum(firstStringNum);
        BigNumbers::BigNumbers secondBigNum(secondStringNum);

        BigNumbers::BigNumbers resultBigNum = firstBigNum + secondBigNum;

        //turn result BigNumbers vector to an std string then to a QString
        std::string resultString = resultBigNum.bigNumberToString();

        //convert std string from result to QString and print
        newLabel = QString::fromStdString(resultString);
        ui->label->setText(newLabel);

        ui->AddButton->setChecked(false);
    }
    if(ui->SubButton->isChecked())
    {
        //using my library here
        BigNumbers::BigNumbers firstBigNum(firstStringNum);
        BigNumbers::BigNumbers secondBigNum(secondStringNum);

        BigNumbers::BigNumbers resultBigNum = firstBigNum - secondBigNum;

        //turn result BigNumbers vector to an std string then to a QString
        std::string resultString = resultBigNum.bigNumberToString();

        //convert std string from result to QString and print
        newLabel = QString::fromStdString(resultString);
        ui->label->setText(newLabel);

        ui->SubButton->setChecked(false);
    }
    if(ui->DivideButton->isChecked())
    {
        //using my library here
        BigNumbers::BigNumbers firstBigNum(firstStringNum);
        BigNumbers::BigNumbers secondBigNum(secondStringNum);

        BigNumbers::BigNumbers resultBigNum = firstBigNum / secondBigNum;

        //turn result BigNumbers vector to an std string then to a QString
        std::string resultString = resultBigNum.bigNumberToString();

        //convert std string from result to QString and print
        newLabel = QString::fromStdString(resultString);
        ui->label->setText(newLabel);

        ui->DivideButton->setChecked(false);
    }
    if(ui->MultButton->isChecked())
    {
        //using my library here
        BigNumbers::BigNumbers firstBigNum(firstStringNum);
        BigNumbers::BigNumbers secondBigNum(secondStringNum);

        BigNumbers::BigNumbers resultBigNum = firstBigNum * secondBigNum;

        //turn result BigNumbers vector to an std string then to a QString
        std::string resultString = resultBigNum.bigNumberToString();

        //convert std string from result to QString and print
        newLabel = QString::fromStdString(resultString);
        ui->label->setText(newLabel);

        ui->MultButton->setChecked(false);
    }

    userIsTypingSecondNumber = false;
}

void MainWindow::binaryOperationPressed()
{
    QPushButton* button = (QPushButton*)sender();

    firstNum = ui->label->text().toDouble();

    button->setChecked(true);

}
