//
// Created by jrx on 2020/5/11.
//
/*
 信号量有关的操作：
 1.定义
    struct semaphore sem;
 2.初始化
    static inline void sema_init(struct semaphore *sem,int val)
        //val是信号量初始值，表示同时只能有val个进程访问共享资源（执行临界区代码）
 3.获取信号量
    //如果没有获取到信号量，会导致当前进程休眠，中断仍然无法唤醒，因此不能在中断上下文使用
    extern void down(struct semaphore *sem)
    //与down类似，只是在休眠过程中可以被中断信号打断。成功获取信号量返回0.
    extern int _must_check down_interruptible(struct semphore *sem)
    //不管是否获取信号量，都会立即返回，成功返回0.不会休眠，可以用在中断上下文
    extern int _must_check down_trylock(struct semaphore *sem)
 4.释放信号量
 extern void up(struct semaphore *sem)

 使用：
 struct semaphore sem;
 sema_init(&sem,1);
 down(&sem);
 //临界区代码
 .......
 up(&sem);




 互斥体(mutex)
 1.定义
    struct mutex my_mutex;
 2.初始化
    mutex_innit(&my_mutex)
 3.获取信号量
    void mutex_lock(struct mutex *lock);
    //上下这两个区别仅仅是阻塞时是否可被中断打断。上面这个会被打断
    int mutex_lock_terruptible(struuct mutex *lock);
    int mutes_trylock(struct mutex *lock);//获取互斥体不会被阻塞
 4.释放信号量
 void mutex_unlock(struct mutex *lock);


//完成量completion
//完成量用于一个执行单元等待另一个执行单元完成某项工作。即在执行某段代码之前必须要执行另一段代码，就要用完成量
 1.定义
    struct completion my_completion;
 2.初始化
    init_completion(&my_completion)
 //步骤1、2可以合并：DECLARE_COMPLETION(my_completion)

 3.等待完成量
    //如果
    //completion.done ==0时;就会通过阻塞的方式等待另一个completion完成。每调用一次wait_for_completion，done的值会-1
    void wait_for_completion(struct completion *c)
    //上下两个的区别是，上面那个在等待时不能被中断打断
    int wait_for_completion_interruptible(struct completion *c)
 4.唤醒完成量
 void completion(struct completion *c)//用于唤醒一个等待的执行单元，done会+1
 void completion_all(struct completion *c)//用于唤醒所有等待统一完成量的执行单元，donw会变成int的最大值，即2147483647



 */
