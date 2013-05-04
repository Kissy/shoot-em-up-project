// Copyright © 2008-2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

#pragma once

class TaskManager;

/**
 * Handles scheduling of all task execution.
 */
class Scheduler {
public:

    /**
     * Constructor.
     *
     * @param [in,out]  pTaskManager    A pointer to the task manager.
     */
    Scheduler(TaskManager* pTaskManager);

    /**
     * Destructor.
     */
    ~Scheduler(void);

    /**
     * Turns benchmarking on or off.
     *
     * @param   bEnable (Optional) the enable.
     */
    void EnableBenchmarking(bool bEnable = true) {
        m_bBenchmarkingEnabled = bEnable;
    }

    /**
     * Sets the UScene to schedule execution of.
     *
     * @param   pScene  A pointer to the universal scene.
     */
    void SetScene(const UScene* pScene);

    /**
     * Execute the set UScene.
     */
    void Execute(void);

protected:
    static const f32                sm_DefaultClockFrequency;
    TaskManager*                    m_pTaskManager;

    f32                             m_ClockFrequency;
    Handle                          m_hExecutionTimer;

    bool                            m_bBenchmarkingEnabled;

    typedef std::map<System::Type, ISystemScene*>   SceneExecs;
    typedef SceneExecs::iterator                    SceneExecsIt;

    SceneExecs                      m_SceneExecs;

};
