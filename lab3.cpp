#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <deque>
#include <random>

#include "lab3/gui.h"

class Simulator {
  int taskCounter = 0;
  int quantSize;
  int quantTimeLeft = 0;
  std::deque<Task> queue;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> weight;
  std::uniform_int_distribution<> priority;
  std::uniform_int_distribution<> interval;
  int intervalTimeLeft = 0;
  int totalTicks = 0;
  int productiveTicks = 0;

 public:
  Simulator(int quantSize, std::pair<int, int> weightLimits,
            std::pair<int, int> priorityLimits,
            std::pair<int, int> intervalLimits)
      : quantSize(quantSize),
        quantTimeLeft(quantSize),
        gen(rd()),
        weight(weightLimits.first, weightLimits.second),
        priority(priorityLimits.first, priorityLimits.second),
        interval(intervalLimits.first, intervalLimits.second){};
  const std::deque<Task>& tick() {
    if (!intervalTimeLeft) {
      intervalTimeLeft = interval(gen);
      queue.push_back(Task(++taskCounter, weight(gen), priority(gen)));
    }
    if (queue.size()) {
      if (++(queue.front())) productiveTicks++;
      if (!quantTimeLeft) {
        if (!queue.front().isFinished()) queue.push_back(queue.front());
        queue.pop_front();
        quantTimeLeft = quantSize;
      };
      --quantTimeLeft;
    }
    --intervalTimeLeft;
    totalTicks++;
    return queue;
  }
};
int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  View view;
  view.show();
  Simulator* sim;
  QTimer* timer = new QTimer();
  QObject::connect(&view, &View::start, [&sim, &view, &timer]() {
    sim = new Simulator(view.getQuantSize(), view.getWeightLimits(),
                        view.getPriorityLimits(), view.getIntervalLimits());
    timer->setInterval(view.getSpeed());
    timer->start();
    QObject::connect(timer, &QTimer::timeout,
                     [&sim, &view]() { view.setTaskList(sim->tick()); });
  });
  QObject::connect(&view, &View::stop, [&sim, &timer]() {
    timer->stop();
    delete sim;
  });

  return a.exec();
}
