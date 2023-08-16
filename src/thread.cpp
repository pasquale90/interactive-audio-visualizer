#include <pthread.h>
#include "visualizer.h"

class MyThread {
public:

   MyThread(int _argument): mArgument(_argument) { }

   void start() {
       pthread_create(&mThreadId, 0,&Visualizer::threadMethod, this);
   }

   void doThings(int x) {
       // something to be done in the thread.
   }

   static void *threadMethod(void *arg) {
       MyThread *_this=static_cast<MyThread *>(arg);
       _this->doThings(_this->getArgument());
   }

   int getArgument() const {
       return mArgument;
   }

private:
   pthread_t mThreadId;
   int mArgument;
};