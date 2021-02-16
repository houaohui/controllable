#ifndef _JIANPAN_H
#define _JIANPAN_H
#include "stc12c5a60s2.h"

extern bit RunSnackFlag;


void passworddriver();

void Page1Control();

void ControlModeControl();
void OnOffControl();
void DisplayModeControl();
void ImagePlay();
void NumPlay();
void SendModeControl();


void TimeModeControl();
void Timeplay();
void SetTimeControl();


void GameModeControl();
void game1control();
void game2control();

void SetPasswordComtrol();

void AboutDeviceControl();
#endif