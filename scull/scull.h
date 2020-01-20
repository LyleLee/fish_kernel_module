#ifndef _SCULL_H_
#define _SCULL_H_

#ifndef SCULL_MAJOR
#define SCULL_MAJOR (0)
#endif

#ifndef SCULL_MINOR
#define SCULL_MINOR (4)
#endif

#ifndef SCULL_MINOR_COUNT
#define SCULL_MINOR_COUNT (4)
#endif

#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

#ifndef SCULL_QSET
#define SCULL_QSET 1000
#endif

struct scull_dev{
    /*为了简单， 我们还是只创建一个数组，这个数组保存一个字符串指针，*/
    struct cdev cdev;
};

#endif
