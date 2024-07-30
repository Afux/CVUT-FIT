#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <compare>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"

char __libc_single_threaded = 0;
using namespace std;
#endif /* __PROGTEST__ */
condition_variable g_condvarOUT;
condition_variable g_condvar_worker;
atomic_int g_firmleft;
mutex g_worker_mutex;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class CProblemPackWrapper {
public:
    explicit CProblemPackWrapper(AProblemPack pack, int id) {
        cnt_solved = 0;
        min_solved = 0;
        m_id = id;
        m_pack = pack;

    };
    AProblemPack m_pack;
    long unsigned int cnt_solved;
    long unsigned int min_solved;
    int m_id;

    bool solved();
};

bool CProblemPackWrapper::solved() {
    if (cnt_solved == m_pack->m_ProblemsCnt.size() && min_solved == m_pack->m_ProblemsMin.size()) {
        return true;
    }
    return false;
}

class CCompanyWrapper {
public:
    explicit CCompanyWrapper(ACompany company, int id) {
        m_id = id;
        m_company = company;
        m_is_last_sent = false;
        m_ii = 0;
        g_firmleft++;
    };

    void inFunc(int id);

    void outFunc(int id);

    thread m_in, m_out;
    mutex m_mutex;

private:
    int m_id;
    atomic_bool m_is_last_sent;
    int m_ii;
    queue<shared_ptr<CProblemPackWrapper>> m_local_queue;
    ACompany m_company;

};

queue<shared_ptr<CProblemPackWrapper>> g_queue;

void CCompanyWrapper::inFunc(int id) {
    //   printf("IN Thread N: %d started\n",id);
    while (true) {
        //    printf("T:%d - ADDING PACK: %d\n",id,m_ii);
        auto tmp = m_company->waitForPack();
        if (tmp == nullptr) {
            // printf("ADDING NULLPTR\n");
            m_is_last_sent = true;
            g_firmleft--;
            break;
        }
        //printf("PACK: %d has %zu cntProblems and %zu minProblems\n",m_ii,tmp->m_ProblemsCnt.size(),tmp->m_ProblemsMin.size());
        lock_guard<mutex> l_lock(m_mutex);
        m_local_queue.push(make_shared<CProblemPackWrapper>(tmp, m_ii++));
        g_worker_mutex.lock();
        g_queue.push(m_local_queue.back());
        g_worker_mutex.unlock();
        g_condvar_worker.notify_all();
    }
    g_condvar_worker.notify_all();
    //  printf("IN Thread N: %d stopped\n",id);

}

void CCompanyWrapper::outFunc(int id) {
    // printf("OUT Thread N: %d started\n",id);
    while (1) {
        unique_lock<mutex> l_lock(m_mutex);
        while ((m_local_queue.empty() && !m_is_last_sent) || !m_local_queue.front()->solved()) {
            g_condvarOUT.wait(l_lock);
        }
        l_lock.unlock();
        //  printf("T:%d Sending pack: %d\n", id,m_local_queue.front()->m_id);
        m_company->solvedPack(m_local_queue.front()->m_pack);
        m_local_queue.pop();
        if (m_local_queue.empty() && m_is_last_sent) {
            break;
        }
    }
    g_condvar_worker.notify_all();
    // printf("OUT Thread N: %d stopped\n",id);
}


class CSolverWrapper {
public:
    CSolverWrapper();

    void solveMin();

    void solveCnt();

    void endFunc();

    void createCntSolver();

    void createMinSolver();

    bool addPackageCnt(APolygon problem, shared_ptr<CProblemPackWrapper> from_pack);

    bool addPackageMin(APolygon problem, shared_ptr<CProblemPackWrapper> from_pack);

    bool has_Cnt_Free_Capacity();

    bool has_Min_Free_Capacity();

    int m_capacity_cnt;

    int m_capacity_min;


    AProgtestSolver m_min_solver;

    AProgtestSolver m_cnt_solver;

    map<shared_ptr<CProblemPackWrapper>, int> m_companies_in_min_solver;

    map<shared_ptr<CProblemPackWrapper>, int> m_companies_in_cnt_solver;



};

CSolverWrapper::CSolverWrapper() {
    m_capacity_cnt = 0;
    m_capacity_min = 0;
}

void CSolverWrapper::solveMin() {

    int tmp = m_min_solver->solve();
    g_worker_mutex.lock();
    m_capacity_min -= tmp;
    g_worker_mutex.unlock();


    for (auto & i : m_companies_in_min_solver) {
        i.first->min_solved += i.second;
    }
    g_condvarOUT.notify_all();
}

void CSolverWrapper::solveCnt() {
    int tmp = m_cnt_solver->solve();
    m_capacity_cnt -= tmp;

    for (auto & i : m_companies_in_cnt_solver) {
        i.first->cnt_solved += i.second;
    }
    g_condvarOUT.notify_all();
}

void CSolverWrapper::endFunc() {

    if (m_capacity_cnt != 0) {
        solveCnt();
    }
    if (m_capacity_min != 0) {
        solveMin();
    }
    g_condvarOUT.notify_all();
}

bool CSolverWrapper::addPackageCnt(APolygon problem, shared_ptr<CProblemPackWrapper> from_pack) {

    unique_lock l_lock(g_worker_mutex);
    if (m_cnt_solver->addPolygon(problem)) {
        m_capacity_cnt++;
        auto search = m_companies_in_cnt_solver.find(from_pack);
        if (search != m_companies_in_cnt_solver.end()) {
            search->second++;
        } else {
            m_companies_in_cnt_solver[from_pack] = 1;
        }
        return true;
    }
    return false;


}

bool CSolverWrapper::addPackageMin(APolygon problem, shared_ptr<CProblemPackWrapper> from_pack) {
    unique_lock l_lock(g_worker_mutex);
    if (m_min_solver->addPolygon(problem)) {
        m_capacity_min++;
        auto search = m_companies_in_min_solver.find(from_pack);
        if (search != m_companies_in_min_solver.end()) {
            search->second++;
        } else {
            m_companies_in_min_solver[from_pack] = 1;
        }
        return true;
    }
    return false;


}

void CSolverWrapper::createCntSolver() {
    m_companies_in_cnt_solver.clear();
    m_cnt_solver = createProgtestCntSolver();
    m_capacity_cnt = 0;
}

void CSolverWrapper::createMinSolver() {
    m_companies_in_min_solver.clear();
    m_min_solver = createProgtestMinSolver();
    m_capacity_min = 0;
}

bool CSolverWrapper::has_Cnt_Free_Capacity() {
    lock_guard l(g_worker_mutex);
    return m_cnt_solver->hasFreeCapacity();
}

bool CSolverWrapper::has_Min_Free_Capacity() {
    lock_guard l(g_worker_mutex);
    return m_min_solver->hasFreeCapacity();
}

shared_ptr<CSolverWrapper> g_solver;

class COptimizer {
public:
    COptimizer() {
        g_firmleft = 0;
        m_workers_cnt = 0;
        g_solver = make_shared<CSolverWrapper>(m_Wrap);
        g_solver->m_min_solver = createProgtestMinSolver();
        g_solver->m_cnt_solver = createProgtestCntSolver();

    }

    static bool usingProgtestSolver(void) {
        return true;
    }

    static void checkAlgorithmMin(APolygon p) {
        // dummy implementation if usingProgtestSolver() returns true
    }

    static void checkAlgorithmCnt(APolygon p) {
        // dummy implementation if usingProgtestSolver() returns true
    }

    void start(int threadCount);

    void stop(void);

    void addCompany(ACompany company);

    void threadFunc(int id);


private:
    vector<thread> m_workers;
    vector<shared_ptr<CCompanyWrapper>> m_companies;
    CSolverWrapper m_Wrap;
    int m_company_id = 0;
    atomic_int m_workers_cnt;


};

void COptimizer::addCompany(ACompany company) {
    m_companies.emplace_back(make_shared<CCompanyWrapper>(company, m_company_id++));
}

void COptimizer::stop(void) {
    g_condvar_worker.notify_all();
    g_condvarOUT.notify_all();
    for (auto & m_company : m_companies) {
        m_company->m_in.join();
    }
    for (auto & m_worker : m_workers) {
        m_worker.join();
    }
    for (auto & m_company : m_companies) {
        m_company->m_out.join();
    }

}

void COptimizer::threadFunc(int id) {
    // printf("Worker Thread N: %d started\n", id);

    shared_ptr<CProblemPackWrapper> f_pack;
    shared_ptr<CSolverWrapper> f_solver;

    long unsigned int ind_cnt;
    long unsigned int ind_min;
    while (true) {
        unique_lock l_lock(g_worker_mutex);
        while (g_queue.empty() && g_firmleft) {
            g_condvar_worker.wait(l_lock);
        }
        if (!g_firmleft && g_queue.empty()) {
            m_workers_cnt--;
            break;
        }
        f_pack = g_queue.front();
        g_queue.pop();
        ind_cnt = 0;
        ind_min = 0;
        l_lock.unlock();
        //  printf("Working on Pack N:%d\n",f_pack->m_id);

        while (ind_cnt != f_pack->m_pack->m_ProblemsCnt.size()) {

            if (!g_solver->has_Cnt_Free_Capacity()) {
                unique_lock l(g_worker_mutex);
                f_solver = std::make_shared<CSolverWrapper>(*g_solver);
                g_solver->createCntSolver();
                l.unlock();
                f_solver->solveCnt();
            }

            if (g_solver->addPackageCnt(f_pack->m_pack->m_ProblemsCnt[ind_cnt], f_pack)) {
                ind_cnt++;
            }
            if (!g_solver->has_Cnt_Free_Capacity()) {
                unique_lock l(g_worker_mutex);
                f_solver = std::make_shared<CSolverWrapper>(*g_solver);
                g_solver->createCntSolver();
                l.unlock();
                f_solver->solveCnt();
            }

        }

        while (ind_min != f_pack->m_pack->m_ProblemsMin.size()) {

            if (g_solver->addPackageMin(f_pack->m_pack->m_ProblemsMin[ind_min], f_pack)) {
                ind_min++;
            }
            if (!g_solver->has_Min_Free_Capacity()) {
                unique_lock l(g_worker_mutex);
                f_solver = std::make_shared<CSolverWrapper>(*g_solver);
                g_solver->createMinSolver();
                l.unlock();
                f_solver->solveMin();
            }
        }

    }
    if (!g_solver->has_Cnt_Free_Capacity()) {
        unique_lock l(g_worker_mutex);
        f_solver = std::make_shared<CSolverWrapper>(*g_solver);
        g_solver->createCntSolver();
        l.unlock();
        f_solver->solveCnt();
    }
    if (!g_solver->has_Min_Free_Capacity()) {
        unique_lock l(g_worker_mutex);
        f_solver = std::make_shared<CSolverWrapper>(*g_solver);
        g_solver->createMinSolver();
        l.unlock();
        f_solver->solveMin();
    }
    if (m_workers_cnt == 0) {
        g_solver->endFunc();
    }

    // printf("Worker Thred N: %d stopped and %d m_min_solver capacity and %d m_cnt_solver\n", id,g_solver->Has_Min_Free_Capacity(),g_solver->has_Min_Free_Capacity());
}

void COptimizer::start(int threadCount) {

    int thread_id = 0;
    m_workers_cnt = threadCount;
    for (auto & m_company : m_companies) {


        m_company->m_in = thread(&CCompanyWrapper::inFunc, m_company, thread_id++);
        m_company->m_out = thread(&CCompanyWrapper::outFunc, m_company, thread_id++);
    }
    for (int i = 0; i < threadCount; ++i) {
        m_workers.emplace_back(thread(&COptimizer::threadFunc, this, i));
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__


int main(void) {


    COptimizer optimizer;
    ACompanyTest company = std::make_shared<CCompanyTest>();
    ACompanyTest company1 = std::make_shared<CCompanyTest>();
    ACompanyTest company2 = std::make_shared<CCompanyTest>();
    ACompanyTest company3 = std::make_shared<CCompanyTest>();

    optimizer.addCompany(company);
    optimizer.addCompany(company1);
    optimizer.addCompany(company2);
    optimizer.addCompany(company3);

    optimizer.start(1);
    optimizer.stop();


    if (!company->allProcessed() || !company1->allProcessed() || !company2->allProcessed())
        throw std::logic_error("(some) problems were not correctly processsed");


    return 0;
}


#endif /* __PROGTEST__ */
