﻿/*! ***********************************************************************************************
 *
 * \file        mainwindow.h
 * \brief       MainWindow 类头文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QWidget>

namespace Ui {
class MainWindow;
}

class Aria2c;
class DownloadTableModel;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

protected:
    virtual void changeEvent(QEvent *e) override;

private:
    void addUri();
    void showOptions();

private:
    Ui::MainWindow *ui;
    Aria2c *aria2c_;
    DownloadTableModel *model_;
};
