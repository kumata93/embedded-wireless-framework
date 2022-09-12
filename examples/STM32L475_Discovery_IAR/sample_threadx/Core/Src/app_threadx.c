/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEMO_STACK_SIZE         1024
#define DEMO_BLOCK_POOL_SIZE    100
#define DEMO_QUEUE_SIZE         100

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* Define the ThreadX object control blocks...  */

TX_THREAD               thread_0;
TX_THREAD               thread_1;
TX_THREAD               thread_2;
TX_THREAD               thread_3;
TX_THREAD               thread_4;
TX_THREAD               thread_5;
TX_THREAD               thread_6;
TX_THREAD               thread_7;
TX_QUEUE                queue_0;
TX_SEMAPHORE            semaphore_0;
TX_MUTEX                mutex_0;
TX_EVENT_FLAGS_GROUP    event_flags_0;
TX_BLOCK_POOL           block_pool_0;

/* Define the counters used in the demo application...  */

ULONG                   thread_0_counter;
ULONG                   thread_1_counter;
ULONG                   thread_1_messages_sent;
ULONG                   thread_2_counter;
ULONG                   thread_2_messages_received;
ULONG                   thread_3_counter;
ULONG                   thread_4_counter;
ULONG                   thread_5_counter;
ULONG                   thread_6_counter;
ULONG                   thread_7_counter;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* Define thread prototypes.  */

void    thread_0_entry(ULONG thread_input);
void    thread_1_entry(ULONG thread_input);
void    thread_2_entry(ULONG thread_input);
void    thread_3_and_4_entry(ULONG thread_input);
void    thread_5_entry(ULONG thread_input);
void    thread_6_and_7_entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR    *pointer;

  /* Allocate the stack for thread 0.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create the main thread.  */
  ret =  tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,
          pointer, DEMO_STACK_SIZE,
          1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }


  /* Allocate the stack for thread 1.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create threads 1 and 2. These threads pass information through a ThreadX
     message queue.  It is also interesting to note that these threads have a time
     slice.  */
  ret = tx_thread_create(&thread_1, "thread 1", thread_1_entry, 1,
          pointer, DEMO_STACK_SIZE,
          16, 16, 4, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate the stack for thread 2.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  ret = tx_thread_create(&thread_2, "thread 2", thread_2_entry, 2,
          pointer, DEMO_STACK_SIZE,
          16, 16, 4, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate the stack for thread 3.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create threads 3 and 4.  These threads compete for a ThreadX counting semaphore.
     An interesting thing here is that both threads share the same instruction area.  */
  ret = tx_thread_create(&thread_3, "thread 3", thread_3_and_4_entry, 3,
          pointer, DEMO_STACK_SIZE,
          8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate the stack for thread 4.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

  ret = tx_thread_create(&thread_4, "thread 4", thread_3_and_4_entry, 4,
          pointer, DEMO_STACK_SIZE,
          8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate the stack for thread 5.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create thread 5.  This thread simply pends on an event flag which will be set
     by thread_0.  */
  ret = tx_thread_create(&thread_5, "thread 5", thread_5_entry, 5,
          pointer, DEMO_STACK_SIZE,
          4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate the stack for thread 6.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create threads 6 and 7.  These threads compete for a ThreadX mutex.  */
  ret = tx_thread_create(&thread_6, "thread 6", thread_6_and_7_entry, 6,
          pointer, DEMO_STACK_SIZE,
          8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate the stack for thread 7.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  ret = tx_thread_create(&thread_7, "thread 7", thread_6_and_7_entry, 7,
          pointer, DEMO_STACK_SIZE,
          8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate the message queue.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_QUEUE_SIZE*sizeof(ULONG), TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create the message queue shared by threads 1 and 2.  */
  ret = tx_queue_create(&queue_0, "queue 0", TX_1_ULONG, pointer, DEMO_QUEUE_SIZE*sizeof(ULONG));
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create the semaphore used by threads 3 and 4.  */
  ret = tx_semaphore_create(&semaphore_0, "semaphore 0", 1);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create the event flags group used by threads 1 and 5.  */
  ret = tx_event_flags_create(&event_flags_0, "event flags 0");
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create the mutex used by thread 6 and 7 without priority inheritance.  */
  ret = tx_mutex_create(&mutex_0, "mutex 0", TX_NO_INHERIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate the memory for a small block pool.  */
  ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, DEMO_BLOCK_POOL_SIZE, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Create a block memory pool to allocate a message buffer from.  */
  ret = tx_block_pool_create(&block_pool_0, "block pool 0", sizeof(ULONG), pointer, DEMO_BLOCK_POOL_SIZE);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* Allocate a block and release the block memory.  */
  ret = tx_block_allocate(&block_pool_0, (VOID **) &pointer, TX_NO_WAIT);
  if (ret != TX_SUCCESS)
  {
    while(1);
  }

  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/* Define the test threads.  */

void    thread_0_entry(ULONG thread_input)
{
  UINT    status;

  /* This thread simply sits in while-forever-sleep loop.  */
  while(1)
  {
    /* Increment the thread counter.  */
    thread_0_counter++;

    /* Sleep for 10 ticks.  */
    tx_thread_sleep(10);

    /* Set event flag 0 to wakeup thread 5.  */
    status =  tx_event_flags_set(&event_flags_0, 0x1, TX_OR);

    /* Check status.  */
    if (status != TX_SUCCESS)
        break;
  }
}


void    thread_1_entry(ULONG thread_input)
{
  UINT    status;

  /* This thread simply sends messages to a queue shared by thread 2.  */
  while(1)
  {
    /* Increment the thread counter.  */
    thread_1_counter++;

    /* Send message to queue 0.  */
    status =  tx_queue_send(&queue_0, &thread_1_messages_sent, TX_WAIT_FOREVER);

    /* Check completion status.  */
    if (status != TX_SUCCESS)
        break;

    /* Increment the message sent.  */
    thread_1_messages_sent++;
  }
}


void    thread_2_entry(ULONG thread_input)
{
  ULONG   received_message;
  UINT    status;

  /* This thread retrieves messages placed on the queue by thread 1.  */
  while(1)
  {
    /* Increment the thread counter.  */
    thread_2_counter++;

    /* Retrieve a message from the queue.  */
    status = tx_queue_receive(&queue_0, &received_message, TX_WAIT_FOREVER);

    /* Check completion status and make sure the message is what we
       expected.  */
    if ((status != TX_SUCCESS) || (received_message != thread_2_messages_received))
        break;

    /* Otherwise, all is okay.  Increment the received message count.  */
    thread_2_messages_received++;
  }
}


void    thread_3_and_4_entry(ULONG thread_input)
{
  UINT    status;

  /* This function is executed from thread 3 and thread 4.  As the loop
     below shows, these function compete for ownership of semaphore_0.  */
  while(1)
  {
    /* Increment the thread counter.  */
    if (thread_input == 3)
        thread_3_counter++;
    else
        thread_4_counter++;

    /* Get the semaphore with suspension.  */
    status =  tx_semaphore_get(&semaphore_0, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status != TX_SUCCESS)
        break;

    /* Sleep for 2 ticks to hold the semaphore.  */
    tx_thread_sleep(2);

    /* Release the semaphore.  */
    status =  tx_semaphore_put(&semaphore_0);

    /* Check status.  */
    if (status != TX_SUCCESS)
        break;
  }
}


void    thread_5_entry(ULONG thread_input)
{

UINT    status;
ULONG   actual_flags;

  /* This thread simply waits for an event in a forever loop.  */
  while(1)
  {
    /* Increment the thread counter.  */
    thread_5_counter++;

    /* Wait for event flag 0.  */
    status =  tx_event_flags_get(&event_flags_0, 0x1, TX_OR_CLEAR,
                                 &actual_flags, TX_WAIT_FOREVER);

    /* Check status.  */
    if ((status != TX_SUCCESS) || (actual_flags != 0x1))
        break;
  }
}


void    thread_6_and_7_entry(ULONG thread_input)
{

UINT    status;

  /* This function is executed from thread 6 and thread 7.  As the loop
     below shows, these function compete for ownership of mutex_0.  */
  while(1)
  {
    /* Increment the thread counter.  */
    if (thread_input == 6)
        thread_6_counter++;
    else
        thread_7_counter++;

    /* Get the mutex with suspension.  */
    status =  tx_mutex_get(&mutex_0, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status != TX_SUCCESS)
        break;

    /* Get the mutex again with suspension.  This shows
       that an owning thread may retrieve the mutex it
       owns multiple times.  */
    status =  tx_mutex_get(&mutex_0, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status != TX_SUCCESS)
        break;

    /* Sleep for 2 ticks to hold the mutex.  */
    tx_thread_sleep(2);

    /* Release the mutex.  */
    status =  tx_mutex_put(&mutex_0);

    /* Check status.  */
    if (status != TX_SUCCESS)
        break;

    /* Release the mutex again.  This will actually
       release ownership since it was obtained twice.  */
    status =  tx_mutex_put(&mutex_0);

    /* Check status.  */
    if (status != TX_SUCCESS)
        break;
  }
}
/* USER CODE END 1 */
