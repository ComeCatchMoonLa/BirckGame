#pragma once

int ClampMachineSpeed(int speed);
int GetMachineSpeed();
void SetMachineSpeed(int speed);
int LogicIntervalMs();
void PumpFrame();
bool DueLogicTick();
void ResetTickState();
