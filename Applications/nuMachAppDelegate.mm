/*!
 * \file nuMachAppDelegate.mm
 * \brief Application delegate for MacOSX
 * \author Nus
 * \date 2011/09/14 23:44
 */

#import "nuMachAppDelegate.h"
#import "nuApplication.h"

static nuMachAppDelegate* gpAppDelegate = nil;

@interface nuMachAppDelegate(Private)

- (void) appThreadProcedure: (id) sender;

@end

@implementation nuMachAppDelegate

@synthesize window;
@synthesize view;

+ (id) instance
{
  return gpAppDelegate;
}

- (NSString*) windowTitle
{
  return @"nuDE";
}

- (const nuTypeInfo&) appMain
{
  return nuAppMain::TypeInfo();
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
  NU_TRACE("Application launching!\n");
  gpAppDelegate = self;
  window = [[nuMachWindow alloc] initWithName: [self windowTitle]];
  view = [[nuMachView alloc] initWithWindow: window];
  appThread = [[NSThread alloc] initWithTarget: self
                                selector: @selector(appThreadProcedure:)
                                object: nil];
  if(window && view && appThread) {
    [window center];
    [window makeKeyAndOrderFront: self];

    nuSingleton< nuApplication >::createInstance();
    INST(nuApplication)->initialize([self appMain]);

    [appThread start];
    [view startDraw];

    // Test file!
    {
      ccstr path = "home://Temp/テスト.cpp";
      nuFile file(nude::FATTR_READ, path);
      NU_TRACE("File size: %lu\n", file.getSize());
    }

    // Test thread!
    {
      class NusTest : public nuObject
      {
      public:
        NusTest() {
          mThread.setName("NusTest");
          mThread.dispatchThread(this,
                                 static_cast< nuFunction >(&NusTest::proc),
                                 nullptr);
        }
        ~NusTest() {}
        void proc(void* param) {
          nuMutex mutex;
          nuRecursiveMutex r_mutex;
          nuSpinLock spin_lock;
          for(ui32 ui = 0; ui < 10; ui++) {
            mutex.lock();
            r_mutex.lock();
            spin_lock.lock();
            NU_TRACE("Count = %d\n", ui);
            spin_lock.unlock();
            r_mutex.unlock();
            mutex.unlock();
            nuThread::usleep(33333);
          }
        }
      private:
        nuThread mThread;
      } nus;
    }
  }
}

- (void) applicationWillTerminate: (NSNotification*) notification
{
  NU_TRACE("Application terminating!\n");
  [view stopDraw];
  [view release];
  view = nil;
  [window release];
  window = nil;
  [appThread release];
  nuSingleton< nuApplication >::deleteInstance();
  NU_TRACE("--- Bye ---\n");
}

- (void) appThreadProcedure: (id) sender
{
  NU_TRACE("Running application thread.\n");
  INST(nuApplication)->run();
}

@end
