#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../3rdparty/qcustomplot/qcustomplot.h"
#include "ui_mainwindow.h"
#include <QJSEngine>
#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
  int i = 30;
  QVector<double> n;
  QVector<double> time;
  QProcess process;
  QJSEngine engine;
  QString generateArgs() {
    QJSValueList args;
    args << i;
    return QString::number(
               engine.evaluate("n=>Math.floor(" + ui->sizeInput->text() + ")")
                   .call(args)
                   .toNumber()) +
           " " +
           QString::number(
               engine.evaluate("n=>Math.floor(" + ui->edgesInput->text() + ")")
                   .call(args)
                   .toNumber()) +
           " " + QString::number(ui->minInput->value()) + " " +
           QString::number(ui->maxInput->value()) + "\n";
  }

public:
  explicit MainWindow(QWidget *parent = nullptr)
      : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, [this]() {
      connect(&process, &QProcess::readyRead, [this]() {
        n.push_back(i);
        time.push_back(process.readLine().toInt());
        process.write(generateArgs().toUtf8());
        auto customPlot = new QCustomPlot(this);
        customPlot->addGraph();
        customPlot->graph(0)->setPen(QPen(Qt::blue));
        customPlot->graph(0)->setData(n, time);
        customPlot->graph(0)->rescaleAxes();
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                    QCP::iSelectPlottables);
        ui->tableWidget->setCellWidget(0, 0, customPlot);
        ++i;
      });
      process.start("lab4simulator.out");
      process.write(generateArgs().toUtf8());
    });
    connect(ui->stopButton, &QPushButton::clicked, &process, &QProcess::kill);
  };
  ~MainWindow() {
    process.kill();
    delete ui;
  };

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
