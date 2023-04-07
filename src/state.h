#pragma once

class State {
  public:
    State() = default;

    virtual void draw() = 0;
    virtual void update() = 0;

    bool isFinshed();

  private:
    bool m_isFinished = false;
};
