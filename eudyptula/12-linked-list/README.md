# Task 12

## This is Task 12 of the Eudyptula Challenge

Nice job with the driver patch.  That took some work in finding the
proper place to modify, and demonstrates a great skill in tracking down
issues when you can't get a specific piece of hardware working.

Now let's step back from drivers (they are boring things), and focus on
the kernel core.  To do that, we need to go way back to the basics --
stuff you would learn in your "intro to data structures" class, if you
happened to take one.

Yes, I'm talking about linked lists.

The kernel has a unique way of creating and handling linked lists, that
is quite different than the "textbook" way of doing so.  But, it turns
out to be faster, and simpler, than a "textbook" would describe, so
that's a good thing.

For this task, write a kernel module, based on your cleaned up one from
task 04, that does the following:
  - You have a structure that has 3 fields:
        char  name[20];
        int   id;
        bool  busy;
    name this structure "identity".
  - Your module has a static variable that points to a list of these
    "identity" structures.
  - Write a function that looks like:
        int identity_create(char *name, int id)
    that creates the structure "identity", copies in the 'name' and 'id'
    fields and sets 'busy' to false.  Proper error checking for out of
    memory issues is required.  Return 0 if everything went ok; an error
    value if something went wrong.
  - Write a function that looks like:
        struct identity *identity_find(int id);
    that takes a given id, iterates over the list of all ids, and
    returns the proper 'struct identity' associated with it.  If the
    identity is not found, return NULL.
  - Write a function that looks like:
        void identity_destroy(int id);
    that given an id, finds the proper 'struct identity' and removes it
    from the system.
  - Your module_init() function will look much like the following:

        struct identity *temp;

        identity_create("Alice", 1);
        identity_create("Bob", 2);
        identity_create("Dave", 3);
        identity_create("Gena", 10);

        temp = identity_find(3);
        pr_debug("id 3 = %s\n", temp->name);

        temp = identity_find(42);
        if (temp == NULL)
                pr_debug("id 42 not found\n");

        identity_destroy(2);
        identity_destroy(1);
        identity_destroy(10);
        identity_destroy(42);
        identity_destroy(3);

   Bonus points for properly checking return values of the above
   functions.

As always, please send the full module (following the proper kernel
coding style rules), and "proof" of it working properly.  And remember
to use your id of "7c1caf2f50d1" in the Subject line properly.

## Proof

Tested in QEMU

```
[root@nixos:~]# insmod /mnt/linked_list.ko
[   12.937090] linked_list: loading out-of-tree module taints kernel.
[   12.937601] Loading linked_list
[   12.937724] id 3 = David
[   12.937817] id 42 not found
[   12.937917] id = 3 is gone now
[   12.938027] list_empty() == 1

[root@nixos:~]# rmmod linked_list
[   39.764792] Unloading linked_list
```