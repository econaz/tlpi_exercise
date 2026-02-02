#ifndef EVENT_FLAG_H
#define EVENT_FLAG_H

int setEventFlag(int semid, int semNum);

int clearEventFlag(int semid, int semNum);

int waitForEventFlag(int semid, int semNum);

int getFlagState(int semid, int semNum);

#endif
