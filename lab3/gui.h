#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QProgressBar>
#include <deque>

#include "task.h"
#include "ui_gui.h"
namespace Ui {
class GUI;
}

class View : public QMainWindow {
  Q_OBJECT
 public:
  explicit View(QWidget *parent = nullptr)
      : QMainWindow(parent), ui(new Ui::GUI) {
    ui->setupUi(this);
    connect(ui->start, &QPushButton::clicked, this, &View::start);
    connect(ui->stop, &QPushButton::clicked, this, &View::stop);
  }
  ~View() { delete ui; }
  std::pair<int, int> getWeightLimits() {
    return std::make_pair(ui->minWeight->value(), ui->maxWeight->value());
  };
  std::pair<int, int> getPriorityLimits() {
    return std::make_pair(ui->minPriority->value(), ui->maxPriority->value());
  };
  std::pair<int, int> getIntervalLimits() {
    return std::make_pair(ui->minTicks->value(), ui->maxTicks->value());
  };
  int getSpeed() { return ui->msPerTick->value(); }
  int getQuantSize() { return ui->quantSize->value(); }
 signals:
  void start();
  void stop();
 public slots:
  void setTaskList(const std::deque<Task> &list) {
    int i = 0;
    ui->tasksTable->clearContents();
    ui->tasksTable->setRowCount(list.size());
    for (const auto &task : list) {
      ui->tasksTable->setItem(i, 0, new QTableWidgetItem(task.name));
      ui->tasksTable->setItem(
          i, 1, new QTableWidgetItem(QString::number(task.priority)));
      ui->tasksTable->setItem(
          i, 2, new QTableWidgetItem(QString::number(task.total)));
      QProgressBar *bar = new QProgressBar(ui->tasksTable);
      bar->setMaximum(task.total);
      bar->setValue(task.progress);
      ui->tasksTable->setCellWidget(i, 3, bar);
      ++i;
    }
  };

 private:
  Ui::GUI *ui;
};

#endif  // GUI_H
