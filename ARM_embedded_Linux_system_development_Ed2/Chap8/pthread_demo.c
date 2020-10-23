// pthread_demo.c                                                                          
#include <pthread.h>                                                                       
#include <unistd.h>                                                                        
#include <stdio.h>                                                                         
#include <stdlib.h>                                                                        
                                                                                           
void* mid_thread(void *arg);			// mid�߳�����                                           
void* term_thread(void *arg);		// term�߳�����                                            
                                                                                           
int main()                                                                                 
{                                                                                          
  pthread_t mid_tid, term_tid;			// ����߳�id                                          
                                                                                           
  if (pthread_create(&mid_tid, NULL, mid_thread, NULL)) {		// ����mid�߳�                 
    perror("Create mid thread error!");                                                    
    return 0;                                                                              
  }                                                                                        
                                                                                           
  if (pthread_create(&term_tid, NULL, term_thread, (void*)&mid_thread)) {		// ����term�߳�
    perror("Create term thread fail!\n");                                                  
    return 0;                                                                              
  }                                                                                        
                                                                                           
  if (pthread_join(mid_tid, NULL)) {		// �ȴ�mid�߳̽���                                 
    perror("wait mid thread error!");                                                      
    return 0;                                                                              
  }                                                                                        
                                                                                           
  if (pthread_join(term_tid, NULL)) {		// �ȴ�term�߳̽���                                
    perror("wait term thread error!");                                                     
    return 0;                                                                              
  }                                                                                        
                                                                                           
  return 0;                                                                                
}                                                                                          
                                                                                           
void* mid_thread(void *arg)				// mid�̶߳���                                           
{                                                                                          
  int times = 0;                                                                           
  printf("mid thread created!\n");                                                         
  while(1) {							// ���ϴ�ӡ�ȴ��Ĵ��������2��                                   
    printf("waitting term thread %d times!\n", times);                                     
    sleep(2);                                                                              
    times++;                                                                               
  }                                                                                        
}                                                                                          
                                                                                           
void* term_thread(void *arg)			// term�̶߳���                                          
{                                                                                          
  pthread_t *tid;                                                                          
  printf("term thread created!\n");                                                        
  sleep(2);                                                                                
  if (NULL!=arg) {                                                                         
    tid = (pthread_t*)arg;                                                                 
    pthread_cancel((*tid));				// ����߳�id�Ϸ��������߳�                              
  }                                                                                        
}                                                                                          
