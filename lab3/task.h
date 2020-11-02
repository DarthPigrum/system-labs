#pragma once
#include <QString>
struct Task {
  int progress = 0;
  int total;
  int priority;
  QString name;
  explicit Task(QString name, int weight, int priority)
      : total(weight), priority(priority), name(name) {}
  explicit Task(int index, int weight, int priority)
      : total(weight),
        priority(priority),
        name(QString("#") + QString::number(index)){};
  bool isFinished() { return total == progress; };
  bool operator++() {
    bool res = progress < total;
    if (res) progress++;
    return res;
  }
};
