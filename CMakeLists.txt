cmake_minimum_required(VERSION 3.9)
project(300assignment3)

set(CMAKE_CXX_STANDARD 11)

add_executable(300assignment3
        InputHandler.c
        InputHandler.h
        LinkedList.h
        list.c
        list.h
        ListNode.h
        main.c
        OSCommands.c
        OSCommands.h
        Report.h
        Semaphore.h Process.h)
