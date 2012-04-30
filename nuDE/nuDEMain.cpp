/*!
 * \file nuDEMain.cpp
 * \brief Main loop.
 * \author Nus
 * \date 2012/03/11 00:28
 */

#include "nuDEMain.h"

static const ui32 TEST_THREAD_POOL = 16;

class TestThreadPool : public nuObject
{
  DECLARE_TYPE_INFO;

  ui32 mSleepTime;
  ui32 mID;

public:
  TestThreadPool()
      : mSleepTime(0),
        mID(0)
  {
    // None...
  }

  ~TestThreadPool() {}

  void update(void* param) {
    // NU_TRACE("Starting test-%d: Sleep time = %d\n", mID, mSleepTime);
    nuThread::usleep(mSleepTime);
    mSleepTime = 0;
    // NU_TRACE("Test-%d is finished.\n", mID);
  }

  void setSleepTime(ui32 sleep_time) {
    mSleepTime = sleep_time;
  }

  void setID(ui32 id_val) {
    mID = id_val;
  }

  bool isIdling(void) const {
    return mSleepTime == 0;
  }

  void endUpdate(void* param) {
    // NU_TRACE("Finished.\n");
  }

};

static TestThreadPool test_thread[TEST_THREAD_POOL];

IMPLEMENT_TYPE_INFO(TestThreadPool, nuObject);
IMPLEMENT_TYPE_INFO(nuDEMain, nuAppMain);

nuDEMain::nuDEMain()
{
  for(ui32 ui = 0; ui < TEST_THREAD_POOL; ui++)
    test_thread[ui].setID(ui);
}

nuDEMain::~nuDEMain()
{

}

void nuDEMain::initialize(void)
{
  nuAppMain::initialize();

  // Test file!
  {
    ccstr path = "home://Temp/テスト.cpp";
    nuFile file(nude::FATTR_READ, path);
    NU_TRACE("File size: %lu\n", file.getSize());
  }
}

void nuDEMain::update(void)
{
  nuAppMain::update();
  const ui32 job_num = TEST_THREAD_POOL;

  nuTaskSet task_set(job_num);
  for(ui32 ui = 0; ui < job_num; ui++) {
    test_thread[ui].setSleepTime(7000);
    task_set.addTask(nuTask(&test_thread[ui],
                            static_cast< nuFunction >(&TestThreadPool::update),
                            nullptr));
  }
  task_set.addEndTask(nuTask(&test_thread[0],
                             static_cast< nuFunction >(&TestThreadPool::endUpdate),
                             nullptr));

  {
    nuThreadPool::JobTicket ticket = nuApplication::threadPool().entryJob(task_set);
    nuApplication::threadPool().waitUntilFinished(ticket);
  }
}

void nuDEMain::terminate(void)
{
  nuAppMain::terminate();
}
