#include "stdafx.h"
#include "libmmgr.h"
#include <dbghelp.h>

void stack_trace(u64 (*raddr)[12], void* outer_thread)
{
    u64 (*raddra)[12]; // [rsp+30h] [rbp+8h] BYREF

    raddra = raddr;
    stack_trace((const stack_tracer*)&raddra, outer_thread);
}

//template <class T>
void stack_trace(const stack_tracer* t, void* outer_thread)
{
    HANDLE CurrentThread; // rbp
    unsigned __int64 v6; // rdi
    unsigned __int64 v7; // rbx
    unsigned __int64 v8; // rcx
    unsigned int v9; // ebx
    __int64 v10; // rcx
    _tagSTACKFRAME64 frame; // [rsp+50h] [rbp-618h] BYREF
    _CONTEXT context; // [rsp+160h] [rbp-508h] BYREF

    if (outer_thread)
        CurrentThread = outer_thread;
    else
        CurrentThread = GetCurrentThread();
    if (exception)
    {
        safe_copy(&context, exception->ContextRecord, 0x4D0u);
    }
    else if (outer_thread)
    {
        memset(&context, 0, sizeof(context));
        context.ContextFlags = CONTEXT_FULL;
        if (!GetThreadContext(outer_thread, &context))
            return;
    }
    else
    {
        v6 = get_rbp();
        v7 = get_rip();
        v8 = get_rsp();
        context.Rbp = v6;
        context.Rip = v7;
        context.Rsp = v8;
    }
    memset(&frame, 0, sizeof(frame));
    frame.AddrFrame.Offset = context.Rbp;
    frame.AddrPC.Offset = context.Rip;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;
    v9 = 0;
    frame.AddrStack.Offset = context.Rsp;
    while (StackWalk64(
        0x8664u,
        GetCurrentProcess(),
        CurrentThread,
        &frame,
        &context,
        ReadProcessMemoryProc64,
        (PFUNCTION_TABLE_ACCESS_ROUTINE64)SymFunctionTableAccess64,
        (PGET_MODULE_BASE_ROUTINE64)SymGetModuleBase64,
        0)
        && frame.AddrFrame.Offset)
    {
        if (v9)
            (*t->_addr)[v9 - 1] = LODWORD(frame.AddrPC.Offset);
        if (++v9 >= 0xD)
            return;
    }
    do
    {
        v10 = v9++ - 1;
        (*t->_addr)[v10] = 0;
    } while (v9 < 0xD);
}

stack_tracer::stack_tracer(u64 (*addr)[12])
{
    for (int i = 0; i < 12; i++)
    {
        *_addr[i] = *addr[i];
    }
}

mem_debugger::mem_debugger()
{
    threading::mutex::mutex("mem_debugger");

    queue.readptr = 0;
    queue.counter = 0;
    enabled = 1;

    lock();

    records = (mem_debugger::record*)_aligned_malloc(0x1000000u, 0x10u);
    R_ASSERT2(records, "not enough memory for records");

    infos = (mem_debugger::info*)_aligned_malloc(0x1B000000u, 0x10u);
    R_ASSERT2(infos, "not enough memory for infos");

    count = 0;
    mt = 1;
    _alloc(this, 0, "the mem-debugger itself");
    if (mt)
        _beginthread((_beginthread_proc_type)helper, 0, 0);
    unlock();
}

void mem_debugger::_alloc(void* ptr, u32 size, const char* desc)
{
    R_ASSERT(ptr);
    R_ASSERT(u32(-1) != size);

    if (enabled)
    {
        R_ASSERT(ptr);

        mem_debugger::info I;
        I.ptr = ptr;
        I.desc = desc;
        I.size = size;
        stack_trace(&I.addr, 0);

        if (mt)
            _append(&I);
        else
            _execute(&I);
    }
}

void mem_debugger::_append(mem_debugger::info* I)
{
    while (TRUE)
    {
        mem_debugger::info op; // [rsp+20h] [rbp-108h] BYREF
        op.ptr = 0;
        while (_InterlockedCompareExchange(&qlock._lock, -1, 0))
            ;
        if (circular_buffer<mem_debugger::info, 9, thread_unsafe_incdec>::write_tail(&this->queue, I))
            break;
        circular_buffer<mem_debugger::info, 9, thread_unsafe_incdec>::read(&this->queue, &op);
        lock();
        this->qlock._lock = 0;
        R_ASSERT(op.ptr);
        _execute(&op);
        unlock();
    }
    qlock.unlock();
}

void mem_debugger::_free(void* ptr)
{
    if (ptr && enabled)
    {
        mem_debugger::info I;
        I.ptr = ptr;
        I.size = -1;

        if (!mt)
            _execute(&I);
        else
            _append(&I);
    }
}

void mem_debugger::dump()
{
    dump_memory(0xFFFFFFFF, "u:\\memdump%d.csv");
}

void mem_debugger::dump_prepare()
{
    lock();
    cleanup();
    for (u32 i = 0; i < count; ++i)
    {
        mem_debugger::info* v5 = &infos[i];
        if (IsBadReadPtr(v5->desc, 4u))
            v5->desc = "unknown";
        else
            v5->desc = _strdup(v5->desc);

        if (sym_ok())
        {
            string4096 v6;
            dump_callstack(v5->funcs, v5->addr, v6, sizeof(v6), GetCurrentProcess(), 10);
        }
    }
    unlock();
}

void mem_debugger::state_diff(u32 state_id)
{
    dump_memory(state_id, "u:\\memdiff%d.csv");
}