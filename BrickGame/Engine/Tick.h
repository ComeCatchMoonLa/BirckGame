#pragma once

int ClampMachineSpeed(int speed);
int GetMachineSpeed();
void SetMachineSpeed(int speed);
int LogicIntervalMs();
void PumpFrame();
bool DueLogicTick();
int CountDueLogicSteps(int stepsPerMachineTick);
void ResetTickState();

template<typename Fn>
void TakeLogicSteps(int stepsPerMachineTick, Fn&& eachStep)
{
    const int n = CountDueLogicSteps(stepsPerMachineTick);
    for (int step = 0; step < n; ++step)
        eachStep();
}
