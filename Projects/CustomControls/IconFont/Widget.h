#pragma once

#include <QtWidgets/QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    
private:
    void radioClicked();

private:
    QButtonGroup* radioGroup = nullptr;
    QGridLayout* gridLayout = nullptr;

    QLabel* lblRed = nullptr;
    QLabel* lblOrange = nullptr;
    QLabel* lblYellow = nullptr;
    QLabel* lblGreen = nullptr;
    QLabel* lblCycan = nullptr;
    QLabel* lblBlue = nullptr;
    QLabel* lblPurple = nullptr;

    QList<QLabel*> lblList;
};
