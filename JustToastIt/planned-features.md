# Planned Features

This document lists out planned & upcoming features for JustToastIt. The features are listed by priority and progress on completion, for user clarity and ease of development.

## Current Limitations
- Parser Stability - At the current stage, the parser has limited testing, and may not correctly handle incorrect input. (Further testing is in progress to fix the issue).

- Force killing the program via Task Manager or electricity loss may cause loss of data, if the autosave does not trigger before the event. (Autosave timing will be desynced from scheduler wake up to make this issue less likely).

- The program only works on Windows devices. (Linux and MacOS support will be added at a later release).

- Task information is stored in a plain text file without any encryption. While there is no cloud storage, if someone were to access your PC's files, this data may be compromised. Sensitive data, such as passwords and banking information should not be entered (Task name encryption will be added at a later release)

- Task collection can handle 1 million tasks in 


## Completed 
- CLI Task Creation 
- Basic Command Parser
- Primitive Scheduler loop 
- Persistant storage 
- Autosave at scheduler wake up
- Persistance on program termination



## In Testing
- Program Stability



## In Development



## High Priority
- Sleep & Busy time settings

## Medium Priority
- Rework parsing to use enum switch

## Low Priority
- 

## Stretch
- Multiple Task's Due Date Interaction 

