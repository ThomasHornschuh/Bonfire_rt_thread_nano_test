
#include "rtthread.h"
#include "console.h"
#include "shell.h"
#include "bonfire.h"
#include "rthw.h"
#include "string.h"


static void reboot(int argc, char** argv)
{
   rt_hw_cpu_shutdown();
}

MSH_CMD_EXPORT(reboot, reboot sample: reboot);


rt_mailbox_t  mb;


static void sendMessage(char *msg) 
{
    char * buffer = malloc(strlen(msg)+1);
    strcpy(buffer,msg);
    rt_err_t err=rt_mb_send(mb,(rt_ubase_t)buffer);
    printk("thread %s: mb_send returns %d\n",rt_thread_self()->name,err);

}

static rt_timer_t timer = NULL;

static void  messageTimerCallBack()
{
char msg[80];
static int count=0;

    snprintf(msg,sizeof(msg),"Timer message %d",count++);
    sendMessage(msg);
}


static void test(int argc,char **argv)
{
 
  // printk("argc: %d\n",argc);
  // for(int i=0;i<argc;i++) {
  //   printk("Arg %d: %s\n",i,argv[i]);
  // }
  if (argc>=2) {
     switch (argv[1][0]) {
       case 'm':
          {
            void *buffer = malloc(256);
            printk("Buffer allocated at %lx\n",buffer);
            rt_thread_mdelay(100);
            free(buffer);
          }
          break;
       case 'c':
          {
            rt_thread_t t = rt_thread_self();
            printk("Current thread id %lx name: %s\n",t,t->name);
          }
          break;   
       case 's':          
          sendMessage(argc>=3?argv[2]:"default message");
          break;
       case 't':
          if (!timer) {
            timer=rt_timer_create("tim01",messageTimerCallBack,NULL,10000,RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
            RT_ASSERT(timer);
            printk("Timer %lx created\n",timer);
            rt_timer_start(timer);           
          } else {  
            rt_timer_delete(timer);
            timer=NULL;
          }            
                    
     }
  }
}

MSH_CMD_EXPORT(test,Usage: test m or test t)


int main() {

char * message;

  printk("Pulling finsh_system_init:  %lx\n",finsh_system_init);
  mb = rt_mb_create("mb01",1,RT_IPC_FLAG_FIFO);
  RT_ASSERT(mb);

  while (1) {
   
    rt_err_t err=rt_mb_recv(mb,(rt_ubase_t *)&message,1000);
    if (err==RT_EOK) {
      printk("thread %s received message: %s\n",rt_thread_self()->name,message);
      free(message);
    }
  }

}