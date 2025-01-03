# c_oop

<!-- TOC -->

* [Overview](#Overview)
* [Features Control](#Features Control)
* [Build](#Build)

<!-- TOC -->

# Overview

The project aimed to provide a `Object Oriented Programming` Template like CPP style like. Using macro implement some
keyword like:

* `Class` you can use this keyword to declare a OOP class base with C struct, and will add a custom virtual table if you
  wanna declare a `virtual function` to experience the `Polymorphism`
* `Class` implement `public` and `private` datamembers, but more like CPP `struct` default member's scope is public.

# Features Control

> You can via define lots of macros to control the generated code.

| Macro        | Usage                                                                    |
|--------------|--------------------------------------------------------------------------|
| GEN_LOGFILE  | Generate debug log file under executable's directory.                    |
| VTABLE_SIZE  | Usage like: `#define VTABLE_SIZE 256`, then the vtable array size is 256 |
| NEED_PRIVATE | IF class need private data members, default none private data members    |
| ---          | ---                                                                      |

# Build

* Make

* CMake

> Default Will compile the project as a static library, you can use the `target_link_libraries()` to link that. more
> details see examples  



