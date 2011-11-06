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
    nuApplication::createApplication([self appMain]);
    [appThread start];

    // Test file!
    {
      nuFile file(nude::FATTR_READ, "home://Temp/test.cpp");
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
          for(ui32 ui = 0; ui < 10; ui++) {
            NU_TRACE("Count = %d\n", ui);
            sleep(1);
          }
        }
      private:
        nuThread mThread;
      } nus;
    }
  }
}

- (void) applicationWillTerminate:(NSNotification *)notification
{
  NU_TRACE("Application terminating!\n");
  [view release];
  view = nil;
  [window release];
  window = nil;
  [appThread release];
  nuApplication::deleteApplication();
}

- (void) appThreadProcedure: (id) sender
{
  NU_TRACE("Running application thread.\n");
  nuApplication::getCurrent()->run();
}

@end
