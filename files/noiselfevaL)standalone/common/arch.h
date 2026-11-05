// ArbiterPlus 独立服务器工程 —— c-s/common/arch.h
#ifndef AP_ARCH_H
#define AP_ARCH_H

#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "log.h"

// 运行体约定: T 需提供 void run()
template <class T>
class Thread {
public:
    Thread(T* t, const std::string& name = "") : m_t(t), m_thread(0), m_info(name),
                                                  m_start(false)
    {
        sem_init(&m_sync, 0, 0);
    }
    ~Thread() { sem_destroy(&m_sync); }

    bool start();                 // 等线程就绪
    void startAway();             // detach
    void wait();                  // join
    bool started() const { return m_start; }
    pthread_t tid() const { return m_thread; }
    const char* info() const { return m_info.c_str(); }

    static void* threadProc(void* param);

private:
    T*          m_t;
    pthread_t   m_thread;
    std::string m_info;
    bool        m_start;
    sem_t       m_sync;
};

class HostEnv {
public:
    static bool expandPath(std::string& path);
    static std::string getLocalIP();
    static std::string getBcast();
};

template <class T>
struct _ThreadParam { T* t; Thread<T>* thread; };

template <class T>
bool Thread<T>::start()
{
    _ThreadParam<T>* param = new _ThreadParam<T>();
    param->t = m_t;
    param->thread = this;

    m_start = true;
    if (pthread_create(&m_thread, NULL, &Thread<T>::threadProc, param) == 0) {
        while (sem_wait(&m_sync) == -1 && errno == EINTR) {}
        return true;
    }
    m_start = false;
    delete param;
    APLog::printf(System, "failed to create thread \"%s\"", m_info.c_str());
    return false;
}

template <class T>
void Thread<T>::startAway()
{
    _ThreadParam<T>* param = new _ThreadParam<T>();
    param->t = m_t;
    param->thread = this;
    m_start = true;
    if (pthread_create(&m_thread, NULL, &Thread<T>::threadProc, param) == 0) {
        pthread_detach(m_thread);
    } else {
        m_start = false;
        delete param;
        APLog::printf(System, "failed to create thread \"%s\"", m_info.c_str());
    }
}

template <class T>
void Thread<T>::wait()
{
    if (m_start)
        pthread_join(m_thread, NULL);
}

template <class T>
void* Thread<T>::threadProc(void* p)
{
    _ThreadParam<T>* param = (_ThreadParam<T>*)p;
    T* t = param->t;
    Thread<T>* th = param->thread;
    delete param;

    const char* name = th->info()[0] ? th->info() : "no name";
    APLog::printf(Debug, "thread \"%s\" started", name);
    sem_post(&th->m_sync);

    t->run();

    th->m_start = false;
    APLog::printf(Debug, "thread \"%s\" exit", name);
    return NULL;
}


class GlobalConfig {
public:
    static unsigned m_chunk;
    static int      m_asyncMax;
};

#endif
