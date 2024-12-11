#include "iZMotionActionState.h"

IZMotionActionState::~IZMotionActionState()
{
}
void IZMotionActionState::runing()
{
}
void IZMotionActionState::changeState(IZMotionActionState *p_state)
{
    p_state->run();
}

CWorkTypeData *workTypeData()
{
    static CWorkTypeData gAutoWorkStepData;
    return &gAutoWorkStepData;
}