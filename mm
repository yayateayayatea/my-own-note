Absolutely. If you said:

“Hey ChatGPT, I want to learn the Linux mm/ subsystem.”

I would not immediately start explaining struct page, buddy allocator, SLUB, page tables, vm_area_struct, reclaim, etc. That is exactly how you get lost.

I'd first build a map and then give you a project that forces you to learn the important pieces gradually.

Demo: Learning Linux mm/
0. First, understand what mm/ is
At a very high level, Linux mm/ is responsible for managing memory.

A simplified picture:

                    User process
                         │
                         │ malloc()
                         │ mmap()
                         ▼
              ┌────────────────────┐
              │   Virtual Memory   │
              └─────────┬──────────┘
                        │
                page tables / VMAs
                        │
                        ▼
              ┌────────────────────┐
              │ Physical Memory    │
              └─────────┬──────────┘
                        │
              ┌─────────┴─────────┐
              │                   │
         page allocator       slab allocator
              │                   │
              ▼                   ▼
           physical            kernel
            pages              objects

But this is only our map, not something you need to memorize yet.

1. Our project
I'd give you a project like:

Build a kernel memory-allocation tracer.

The final result might look conceptually like:

$ sudo ./memtrace

PID     COMM       SIZE       TYPE
1234    bash       4096       page
1234    bash       8192       slab
5678    firefox    16384      page

And eventually:

Memory statistics
────────────────────────────
Allocations:       12483
Frees:              11921
Currently allocated: 562

Pages allocated:   12 MB
Slab allocated:     4 MB

This is much better than saying:

"Let's implement our own memory allocator."

Because implementing Linux's memory-management subsystem is enormous.

Our project instead makes us observe and interact with the real mm/ subsystem.

2. Before coding: our Level 1 / 2 / 3 map
This is where your three-level idea becomes useful.

Level 1 — Must understand now
For our first version:

Level 1
├── physical vs virtual memory
├── page
├── virtual address
├── kernel address
├── kmalloc()
├── kfree()
├── alloc_pages()
├── __free_pages()
└── basic allocation lifecycle

These are things I would expect you to understand reasonably well before moving on.

For example:

void *p;

p = kmalloc(4096, GFP_KERNEL);

if (!p)
    return -ENOMEM;

/* use memory */

kfree(p);

You should eventually be able to explain:

kmalloc()
   │
   ├── What does it allocate?
   ├── Where does the memory come from?
   ├── What does GFP_KERNEL mean?
   ├── Who owns the returned memory?
   └── What happens when kfree() is called?

3. Level 2 — Recognize, but don't dive into yet
While implementing the project, you'll probably encounter:

Level 2
├── struct page
├── buddy allocator
├── SLAB
├── SLUB
├── vm_area_struct
├── page tables
├── GFP flags
├── per-CPU caches
├── NUMA
├── highmem
└── memory zones

I'd tell you:

You need to know what these things are roughly, but don't study their implementation yet.

For example, when you see:

struct page *page;

you should know:

"struct page represents metadata associated with a physical page."

Good enough for now.

Don't immediately jump into:

include/linux/mm_types.h
        ↓
struct page
        ↓
folios
        ↓
compound pages
        ↓
memory sections
        ↓
SPARSEMEM
        ↓
NUMA

That's the rabbit hole we're deliberately avoiding.

4. Level 3 — Completely defer
If you encounter things like:

Level 3
├── THP
├── KSM
├── NUMA balancing
├── memory compaction
├── CMA
├── MGLRU
├── memory hotplug
├── HMM
├── ZONE_DEVICE
└── architecture-specific MMU details

I'd explicitly say:

Not yet. Put these in the parking lot.

You aren't avoiding them forever.

You're establishing a dependency order.

5. First mini-project: understand kmalloc()
Before writing our actual tracer, I'd give you a tiny experiment.

Something like:

static int __init mem_demo_init(void)
{
    void *p;

    p = kmalloc(4096, GFP_KERNEL);

    if (!p)
        return -ENOMEM;

    pr_info("allocated: %px\n", p);

    kfree(p);

    return 0;
}

But here's the important part:

I wouldn't just give you that code and move on.

I'd ask you to investigate:

1. Where is kmalloc() defined?

2. What does kmalloc() eventually call?

3. What does GFP_KERNEL mean?

4. What happens when kmalloc() fails?

5. What is the relationship between kmalloc()
   and physical pages?

6. What happens when kfree() executes?

Then we'd trace the code together.

6. And here is where AI becomes your guide
Suppose you ask me:

"I found kmalloc() but the implementation leads me into SLUB. I'm lost."

I'd not dump SLUB's entire implementation on you.

I'd say:

Your current position
your module
    │
    ▼
 kmalloc()
    │
    ▼
 SLUB allocator
    │
    ▼
 slab
    │
    ▼
 physical pages

Then:

For now, you only need to understand that SLUB is one of Linux's mechanisms for allocating smaller kernel objects efficiently. Don't study its internal per-CPU structures yet.

Then we continue.

That's the difference between navigation and information dumping.

7. Then introduce alloc_pages()
Once kmalloc() makes sense, we deliberately use another API:

struct page *page;

page = alloc_pages(GFP_KERNEL, 0);

if (!page)
    return -ENOMEM;

/* experiment */

__free_pages(page, 0);

Now we have:

              Kernel memory allocation
                       │
              ┌────────┴─────────┐
              │                  │
           kmalloc()        alloc_pages()
              │                  │
              ▼                  ▼
        kernel objects       physical pages

This distinction is very important.

Now you have a reason to learn:

struct page

instead of learning it because someone told you it's important.

8. Then the project gets interesting
Now we might build:

memlab

with operations like:

$ sudo ./memlab alloc-kmalloc 4096
$ sudo ./memlab alloc-pages 10
$ sudo ./memlab stats
$ sudo ./memlab free-all

Kernel side:

              userspace
                  │
             ioctl/write
                  │
                  ▼
            memlab driver
                  │
        ┌─────────┴─────────┐
        ▼                   ▼
    kmalloc()          alloc_pages()
        │                   │
        ▼                   ▼
       SLUB              page allocator

Now you're learning several mm/ concepts through an actual system.

9. Then we deliberately break it
This is where I'd push your understanding.

For example:

What happens if you allocate 1 GB?

What happens if allocation fails?

What happens if you free twice?

What happens if you access memory after freeing it?

What happens if multiple processes use memlab?

What happens if the module is unloaded while allocations remain?

Now suddenly concepts such as:

ownership
lifetime
reference counting
locking
GFP flags
allocation failure
use-after-free

become meaningful.

Instead of memorizing definitions, you've experienced why they exist.

10. Eventually move into virtual memory
After physical/kernel allocation makes sense, I'd move the project toward userspace memory.

For example:

userspace program
       │
       │ mmap()
       ▼
┌─────────────────┐
│ Virtual address │
└────────┬────────┘
         │
         ▼
       VMA
         │
         ▼
    page tables
         │
         ▼
 physical page

Now Level 1 expands:

Level 1
├── virtual address
├── physical address
├── page
├── VMA
├── page table
├── page fault
├── mmap()
└── allocation

And again, the project gives us a reason to learn each one.

11. Eventually you get to the real mm/ map
After several projects, your mental map might become:

                         Linux MM
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
   Physical memory      Virtual memory      Kernel objects
        │                   │                   │
        ▼                   ▼                   ▼
   page allocator        mmap()              kmalloc()
        │                   │                   │
        ▼                   ▼                   ▼
      pages                VMA                SLUB
        │                   │                   │
        └─────────────┬─────┴───────────────────┘
                      │
                 page tables
                      │
                 page faults
                      │
                memory reclaim
                      │
                 writeback
                      │
                   swap

At that point, things like:

buddy allocator
SLUB
page faults
reclaim
LRU
swap
THP
NUMA
compaction

stop looking like completely unrelated vocabulary.

They have a place in your map.

The key difference from a normal tutorial
A normal tutorial might do:

Chapter 1: kmalloc
Chapter 2: struct page
Chapter 3: buddy allocator
Chapter 4: SLUB
Chapter 5: VMA
Chapter 6: page tables
...

And after chapter 6 you're thinking:

"I understand the words, but how does all this fit together?"

Our approach does the opposite:

             PROJECT
                │
                ▼
          encounter concept
                │
                ▼
       understand minimum
                │
                ▼
          inspect kernel
                │
                ▼
          experiment
                │
                ▼
          update mental map
                │
                ▼
         encounter next thing

So the project determines what you learn, rather than trying to learn mm/ encyclopedically before doing anything.

And I'd keep your 3 levels dynamic
This is particularly important.

Something can move between levels.

For example:

Week 1:

struct page
    ↓
Level 2
"Know roughly what it represents."

Later:

Week 5:

struct page
    ↓
Level 1
"I need to understand how pages are represented
because we're working with the page allocator."

Later:

Month 3:

folio
    ↓
Level 2
"I know why it exists and where it fits."

And eventually:

folio internals
    ↓
Level 1
"I specifically need this for the project I'm doing."

That's how you prevent premature depth.

So if you actually started mm/ with me...
I would propose something like:

MM ROADMAP

Project 1 ─ Kernel allocation lab
             │
             ├── kmalloc/kfree
             ├── GFP flags
             └── allocation lifecycle

Project 2 ─ Page allocation lab
             │
             ├── struct page
             ├── alloc_pages
             ├── page order
             └── buddy allocator

Project 3 ─ Memory mapping lab
             │
             ├── virtual addresses
             ├── mmap
             ├── VMA
             └── page faults

Project 4 ─ Page-table exploration
             │
             ├── PGD/P4D/PUD/PMD/PTE
             ├── address translation
             └── TLB

Project 5 ─ Memory reclaim experiment
             │
             ├── LRU
             ├── reclaim
             ├── swap
             └── page cache

Project 6 ─ Kernel allocator exploration
             │
             ├── SLUB
             ├── slabs
             ├── caches
             └── per-CPU allocation

Project 7 ─ MM observability tool
             │
             ├── tracepoints
             ├── ftrace
             ├── perf
             └── eBPF

Project 8 ─ Advanced MM
             │
             ├── THP
             ├── NUMA
             ├── compaction
             ├── reclaim internals
             └── memory cgroups

And at every project we'd maintain your three buckets:

┌──────────────────────────────────────────────┐
│ LEVEL 1 — Understand deeply                  │
├──────────────────────────────────────────────┤
│ Concepts required for current project        │
└──────────────────────────────────────────────┘

┌──────────────────────────────────────────────┐
│ LEVEL 2 — Recognize                          │
├──────────────────────────────────────────────┤
│ Know what it is, don't dive into internals   │
└──────────────────────────────────────────────┘

┌──────────────────────────────────────────────┐
│ LEVEL 3 — Park for later                    │
├──────────────────────────────────────────────┤
│ Interesting, but currently irrelevant        │
└──────────────────────────────────────────────┘

That is the learning system I'd recommend for you. It preserves the thing you already like—building real projects with AI guidance—while preventing the "I have read 30 kernel APIs and now I have no idea how Linux works" problem.
