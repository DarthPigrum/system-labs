#include <QApplication>
#include <QDebug>
#include <QMap>
#include <QTimer>
#include <QVector>
#include <deque>
#include <random>

#include "lab3/graphs.h"
#include "lab3/gui.h"
#include "lab3/simulator.h"
int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  View view;
  Simulator* sim;
  QTimer* timer = new QTimer();
  QObject::connect(&view, &View::start, [&sim, &view, &timer]() {
    sim = new Simulator(view.getQuantSize(), view.getWeightLimits(),
                        view.getPriorityLimits(), view.getIntervalLimits());
    timer->setInterval(view.getSpeed());
    timer->start();
    QObject::connect(timer, &QTimer::timeout, [&sim, &view]() {
      sim->tick();
      view.setCurrentTask(sim->currentTask());
      view.setTaskList(sim->queue());
      view.setByPriorityGraph(generateByPriorityGraph(sim->finishedTasks()));
    });
  });
  QObject::connect(&view, &View::stop, [&sim, &timer]() {
    timer->stop();
    delete sim;
  });
  QObject::connect(&view, &View::draw, [&view]() {
    const auto [minInterval, maxInterval] = view.getIntervalLimits();
    std::map<int, std::vector<std::shared_ptr<Task>>> time;
    std::map<int, double> idle;
    for (int i = minInterval; i <= maxInterval; ++i) {
      auto simulation =
          new Simulator(view.getQuantSize(), view.getWeightLimits(),
                        view.getPriorityLimits(), std::make_pair(i, i));
      for (int n = 0; n < 2000; ++n) simulation->tick();
      time[i] = simulation->finishedTasks();
      idle[i] = simulation->loadFactor();
    }
    view.setTimeGraph(generateTimeGraph(time));
    view.setIdleGraph(generateIdleGraph(idle));
  });
  view.show();
  return a.exec();
}
