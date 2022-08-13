//
// Created by WJ on 2022/8/10.
//

#ifndef FFMPEGDEMO_SYNCCLOCK_H
#define FFMPEGDEMO_SYNCCLOCK_H

#include <LogUtil.h>

class SyncClock {

public:
    SyncClock() {
        lastPts = 0;
        frameTimer = 0;
        curPts = 0;
        lastUpdate = 0;
    }

    ~SyncClock() {}

    void setClock(double pts, double time) {
        this->curPts = pts;
        this->lastUpdate = time;
    }

    double getClock() {
        double time = GetSysCurrentTime();
        return curPts + time - lastUpdate;
    }


public:
    double lastPts;
    double frameTimer;
    double curPts;

private:
    double lastUpdate;

};


#endif //FFMPEGDEMO_SYNCCLOCK_H
