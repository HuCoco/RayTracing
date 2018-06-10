#pragma once
#include <Platform/Platform.h>

enum ThreadPriority
{
    GE_THREAD_PRIORITY_LOWEST = 0,
    GE_THREAD_PRIORITY_BELOW_NORMAL = 1,
    GE_THREAD_PRIORITY_NORMAL = 2,
    GE_THREAD_PRIORITY_ABOVE_NORMAL = 3,
    GE_THREAD_PRIORITY_HEIGHTEST = 4,
};

typedef uint32_t ThreadAffinityMask;
enum ThreadAffinity
{
    GE_THREAD_CORE_0 = 1 << 0,
    GE_THREAD_CORE_1 = 1 << 1,
    GE_THREAD_CORE_2 = 1 << 2,
    GE_THREAD_CORE_3 = 1 << 3,
    GE_THREAD_CORE_4 = 1 << 4,
    GE_THREAD_CORE_5 = 1 << 5,
    GE_THREAD_CORE_6 = 1 << 6,
    GE_THREAD_CORE_7 = 1 << 7,
    GE_THREAD_CORE_8 = 1 << 8,
    GE_THREAD_CORE_9 = 1 << 9,
    GE_THREAD_CORE_10 = 1 << 10,
    GE_THREAD_CORE_11 = 1 << 11,
    //...
};

static const size_t GE_DEFUALT_THRED_STACK_SIZE = 0x4000; // 4K

typedef uint32_t(*ThreadEntryFunc)(void*);

struct ThreadInitializeDescription
{
    void* EntryFunc{ nullptr };
    void* Parameter{ nullptr };
    size_t StackSize{ GE_DEFUALT_THRED_STACK_SIZE };
    void* StackAddress{ nullptr };
    ThreadPriority Priority{ ThreadPriority::GE_THREAD_PRIORITY_NORMAL };
    ThreadAffinityMask Affinity{ ThreadAffinity::GE_THREAD_CORE_0 };
};

class ThreadWindows
{
public:
    ThreadWindows();
   ~ThreadWindows();
   void Initialize(const ThreadInitializeDescription& Description);
   void Finalize();
   void SetPriority(ThreadPriority Priority);
   void SetAffinity(ThreadAffinityMask Mask);
   void Join();
   void Detach();
   static DWORD ThreadEntryFuncWapper(void* self);
private:
    HANDLE m_Handle;
    ThreadInitializeDescription m_ThreadDescription;
    uint32_t m_ThreadID{ 0 };
};


class MutexWindows
{
public:
    MutexWindows();
    ~MutexWindows();
    void Initialize(bool InitalOwner);
    void Finalize();
    void Lock();
    void Unlock();
    bool TryLock();

private:
    HANDLE m_Handle;
};

class SemaphoreWindows
{
public:
    SemaphoreWindows();
    ~SemaphoreWindows();
    void Initialize(uint32_t initialCount, uint32_t maximumCount);
    void Finalize();
    void Acquire();
    void Release();
    void Release(uint32_t releaseCount);
private:
    HANDLE m_Handle;
};

class CriticalSectionWindows
{
public:
    CriticalSectionWindows();
    virtual ~CriticalSectionWindows();
    virtual void Initialize();
    virtual void Finalize();
    virtual void EnterSection();
    virtual void LeaveSection();
private:
    CRITICAL_SECTION m_Handle;
    bool m_IsIntial;
};