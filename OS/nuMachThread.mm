/*!
 * \file nuMachThread.mm
 * \brief MacOS X thread handler.
 * \author Nus
 * \date 2011/11/06 04:35
 */

#import "nuMachThread.h"
#import "nuThread.h"

// @brief Template specialization for sleep function.
template<>
void nude::Thread< nuMachThread >::sleep(ui32 int_seconds)
{
  ::sleep(int_seconds);
}

// @brief Template specialization for sleep function.
template<>
void nude::Thread< nuMachThread >::usleep(ui32 int_useconds)
{
  ::usleep(int_useconds);
}



/*!
 * \class _MachThreadHandle
 * \brief Mach thread handle.
 */
@interface _MachThreadHandle : NSObject
{
  NSThread* thread;               //!< Thread handle.
  nuObject* object;               //!< Thread object.
  nuFunction function;            //!< Thread function.
  void* parameter;                //!< Thread parameter.
}

//! \brief Thread handle property.
@property (readonly, nonatomic) NSThread* thread;

//! \brief Dispatch thread.
- (void) dispatchWithTarget: (nuObject*) target function: (nuFunction) func parameter: (void*) param;
//! \brief Execute thread.
- (void) execute: (id) sender;

@end



@implementation _MachThreadHandle

@synthesize thread;

//! \brief Initialize object.
- (id) init
{
  self = [super init];
  if(self) {
    // NU_TRACE("Creating thread handle.\n");
    thread = [[NSThread alloc] initWithTarget: self selector: @selector(execute:) object: nil];
  }
  return self;
}

//! \brief Deallocate.
- (void) dealloc
{
  // NU_TRACE("Deleting thread handle.\n");
  [thread release];
  thread = nil;
}

// Dispatch thread.
- (void) dispatchWithTarget: (nuObject*) target function: (nuFunction) func parameter: (void*) param
{
  object = target;
  function = func;
  parameter = param;

  [thread start];
}

// Execute thread.
- (void) execute: (id) sender
{
  if(object && function) {
    NU_TRACE("Dispatching thread \"%s\"\n", [[thread name] cStringUsingEncoding: NSASCIIStringEncoding]);
    (object->*(function))(parameter);
  }
}

@end



// Default constructor.
nuMachThread::nuMachThread()
{
  _MachThreadHandle* handle = [[_MachThreadHandle alloc] init];
  mpHandle = handle;
}

// Default destructor.
nuMachThread::~nuMachThread()
{
  while(isExecuting())
    nude::Thread< nuMachThread >::usleep(16666);
  if(mpHandle) {
    _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
    [handle release];
    mpHandle = nullptr;
  }
}

// Dispatch thread.
void nuMachThread::dispatch(nuObject* p_object, nuFunction func, void* param)
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  [handle dispatchWithTarget: p_object function: func parameter: param];
}

// Is thread cancelled.
bool nuMachThread::isCancelled(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] isCancelled];
}

// Is thread finished.
bool nuMachThread::isFinished(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] isFinished];
}

// Is thread executing.
bool nuMachThread::isExecuting(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] isExecuting];
}

// Set thread name.
void nuMachThread::setName(ccstr name)
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  NSString* name_str = [NSString stringWithCString: name encoding: NSASCIIStringEncoding];
  [[handle thread] setName: name_str];
}

// Get thread name.
ccstr nuMachThread::getName(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  NSString* name_str = [[handle thread] name];
  return [name_str cStringUsingEncoding: NSASCIIStringEncoding];
}

// Set stack size.
void nuMachThread::setStackSize(size_t stack_sz)
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  [[handle thread] setStackSize: stack_sz];
}

// Get stack size.
size_t nuMachThread::getStackSize(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] stackSize];
}

// Set thread priority.
void nuMachThread::setPriority(f64 priority)
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  [[handle thread] setThreadPriority: priority];
}

// Get thread priority.
f64 nuMachThread::getPriority(void) const
{
  _MachThreadHandle* handle = static_cast< _MachThreadHandle* >(mpHandle);
  return [[handle thread] threadPriority];
}
