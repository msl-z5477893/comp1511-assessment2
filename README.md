COMP1511 Assessment 2 - Bookshelf
=================================


Status
------

Stage 1 :heavy_check_mark:

______________

- [x] Stage 1.1
- [x] Stage 1.2
- [x] Stage 1.3
- [x] Stage 1.4
- [x] Stage 1.5

Stage 2

______________

- [x] Stage 2.1
- [x] Stage 2.2
- [x] Stage 2.3
- [x] Stage 2.4

Stage 3

______________

- [x] Stage 3.1 
- [x] Stage 3.2 
- [x] Stage 3.3 
- [x] Stage 3.4
- [ ] Stage 3.5

Stage 4

______________

- [ ] Stage 4.1
- [ ] Stage 4.2

TODO
---
- Fix `access to memory that has already been freed` error in stage 3.5.

Notes
-----
- Be careful when using `malloc`: memory must never be allocated in a function
unless said so in the assignment specs.
- To insert into a linked list assign  `next` pointer of the node rather than
reassigning the structure itself.
- Stage 3.5 code quality is poor: currently it works but there are potential
bugs that could come from anywhere.

```c
new_node->next = anchor_node->next;
anchor_node->next = new_node;
```
