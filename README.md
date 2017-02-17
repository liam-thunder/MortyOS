## MortyOS Kernel
An operating system kernel called MortyOS, based on JOS from [MIT 6.828](https://pdos.csail.mit.edu/6.828/2016/index.html), [JamesM's kernel development tutorials](http://www.jamesmolloy.co.uk/tutorial_html/) and [hurlex OS kernel tutorial](http://wiki.0xffffff.org/). 

MortyOS is developed for learning purporses, some features may not be perfect.

## Done
* `printf()` function (can't print float/double)
* few functions in `string.h`
* protected mode flat memory model(gdt)
* interrupt handler (idt isr irq)
* simple physical memory management
* paging for kernel
* kernel heap (kmalloc and kfree)

## TODO
* Buddy Algorithm
* better kmalloc algorithm
* keyboard driver
* mouse driver

### Fun
Morty is the name of a charactor from *Rick and Morty* (an adult animated science fiction sitcom called).