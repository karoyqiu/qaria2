﻿/*! ***********************************************************************************************
 *
 * \file        mainwindow.cpp
 * \brief       MainWindow 类源文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aria2c.h"
#include "optionsdialog.h"


MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , aria2c_(nullptr)
{
    ui->setupUi(this);
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::showOptions);

    aria2c_ = new Aria2c(this);
    aria2c_->start();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);

    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::showOptions()
{
    OptionsDialog dialog(this);
    dialog.exec();
}
