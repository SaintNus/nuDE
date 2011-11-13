/*!
 * \file nuMachThread.mm
 * \brief MacOS X thread handler.
 * \author Nus
 * \date 2011/11/06 04:35
 */

#import "nuMachThread.h"
#import "nuThread.h"

template<>
void nude::Thread< nuMachThread >::sleep(ui32 int_seconds)
{
  ::sleep(int_seconds);
}

template<>
void nude::Thread< nuMachThread >::usleep(ui32 int_useconds)
{
  ::usleep(int_useconds);
}

@interface _MachThreadHandle : NSObject
{
  NSThread* thread;
  nuObject* object;
  nuFunction function;
  void* parameter;
}

@property (readonly, nonatomic) NSThread* thread;

- (void) dispatchWithTarget: (nuObject*) target function: (nuFunction) func parameter: (void*) param;
- (void) execute: (id) sender;

@end



@implementation _MachThreadHandle

@synthesize thread;

- (id) init
{
  self = [super init];
  if(self) {
    // NU_TRACE("Creating thread handle.\n");
    thread = [[NSThread alloc] initWithTarget: self selector: @selector(execute:) object: nil];
  }
  return self;
}

- (void) dealloc
{
  // NU_TRACE("Deleting thread handle.\n");
  [thread release];
  thread = nil;
}

- (void) dispatchWithTarget: (nuObject*) target function: (nuFunction) func parameter: (void*) param
{
  object = target;
  function = func;
  parameter = param;

  [thread start];
}

- (void) execute: (id) sender
{
  if(object && function) {
    ccstr name = [[thread name] cStringUsingEncoding: NSASCIIStringEncoding];
    NU_TRACE("Dispatching thread \"%s\"\n", name);
    (object->*(function))(parameter);
  }
}

@end



nuMachThread::nuMachThread()
{
  _MachThreadHandle* handle = [[_MachThreadHandle alloc] init];
  mpHandle = handle;
}

nuMachThread::~nuMachThread()
{
  if(mpHandle) {
    _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
    [handle release];
    mpHandle = nullptr;
  }
}

void nuMachThread::dispatch(nuObject* p_object, nuFunction func, void* param)
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  [handle dispatchWithTarget: p_object function: func parameter: param];
}

bool nuMachThread::isCancelled(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] isCancelled];
}

bool nuMachThread::isFinished(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] isFinished];
}

bool nuMachThread::isExecuting(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] isExecuting];
}

void nuMachThread::setName(ccstr name)
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  NSString* name_str = [NSString stringWithCString: name encoding: NSASCIIStringEncoding];
  [[handle thread] setName: name_str];
}

ccstr nuMachThread::getName(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  NSString* name_str = [[handle thread] name];
  return [name_str cStringUsingEncoding: NSASCIIStringEncoding];
}

void nuMachThread::setStackSize(size_t stack_sz)
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  [[handle thread] setStackSize: stack_sz];
}

size_t nuMachThread::getStackSize(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] stackSize];
}

void nuMachThread::setPriority(f64 priority)
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  [[handle thread] setThreadPriority: priority];
}

f64 nuMachThread::getPriority(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] threadPriority];
}
