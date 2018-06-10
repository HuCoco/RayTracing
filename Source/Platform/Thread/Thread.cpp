#include "Thread.h"

const int32_t ThreadWindowsPriorityMap[] = {
    THREAD_PRIORITY_LOWEST,
    THREAD_PRIORITY_BELOW_NORMAL,
    THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_HIGHEST,
    THREAD_PRIORITY_ABOVE_NORMAL
};

ThreadWindows::ThreadWindows()
    : m_Handle(nullptr)
{

}

ThreadWindows::~ThreadWindows()
{
    if (m_Handle)
    {
        Finalize();
    }
}

void ThreadWindows::Initialize(const ThreadInitializeDescription& description)
{
    m_ThreadDescription = description;
    m_Handle = ::CreateThread(nullptr, m_ThreadDescription.StackSize, ThreadWindows::ThreadEntryFuncWapper, this, 0, (LPDWORD)(&m_ThreadID));
    assert(m_Handle && "Failed to create thread.");
}

void ThreadWindows::Finalize()
{
    auto result = CloseHandle(m_Handle);
    m_Handle = nullptr;
}

void ThreadWindows::SetPriority(ThreadPriority Priority)
{
    auto result = ::SetThreadPriority(m_Handle, ThreadWindowsPriorityMap[Priority]);
}

void ThreadWindows::SetAffinity(ThreadAffinityMask Mask)
{
    auto result = ::SetThreadAffinityMask(m_Handle, Mask);
}

void ThreadWindows::Join()
{
    auto result = WaitForSingleObject(m_Handle, INFINITE);
}

void ThreadWindows::Detach()
{
    auto result = WaitForSingleObject(m_Handle, IGNORE);
}


DWORD ThreadWindows::ThreadEntryFuncWapper(void* parameter)
{
    ThreadWindows* self = reinterpret_cast<ThreadWindows*>(parameter);
    uint32_t result = ((ThreadEntryFunc)(self->m_ThreadDescription.EntryFunc))(self->m_ThreadDescription.Parameter);
    return DWORD(result);
}

MutexWindows::MutexWindows()
    : m_Handle(nullptr)
{

}

MutexWindows::~MutexWindows()
{
    if (m_Handle)
    {
        Finalize();
    }
}

void MutexWindows::Initialize(bool InitalOwner)
{
    m_Handle = (void*)CreateMutex(NULL, InitalOwner, NULL);
    assert(m_Handle && "Failed to create mutex.");
}

void MutexWindows::Finalize()
{
    auto result = CloseHandle(m_Handle);
    m_Handle = nullptr;
}

void MutexWindows::Lock()
{
    auto result = WaitForSingleObject(m_Handle, INFINITE);
}

void MutexWindows::Unlock()
{
    auto result = ReleaseMutex(m_Handle);
}

bool MutexWindows::TryLock()
{
    return WaitForSingleObject(m_Handle, 0) != WAIT_TIMEOUT;
}