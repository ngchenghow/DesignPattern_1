
A compact C++-project that implements multiple classical object-oriented design patterns, organized by pattern category for study and reference.

## Overview

This repository provides example implementations of various GoF-style design patterns in C++. Each pattern is contained in a separate folder, including but not limited to:

- Abstract Factory
- Adapter
- Bridge
- Builder
- Chain of Responsibility
- Composite
- Decorator
- Factory
- Flyweight
- Prototype
- Proxy

The project is built using Visual Studio (solution: `DesignPattern_1.sln`), with C++ source files and project definitions present.

## Purpose

- Serve as a learning resource for developers who want to **understand how design patterns map into C++ code**.
- Provide runnable code examples to explore how patterns structure class relationships, behaviour delegation, object creation and composition.
- Offer a reference for integrating design-pattern ideas into your own C++ projects (for example your framework work).

## Structure

- Each pattern folder contains one or more `.cpp`/.h files demonstrating that pattern in action.
- The root contains `DesignPattern_1.cpp`, which appears to drive or demonstrate the collection.
- Visual Studio project files (`.vcxproj`, `.vcxproj.filters`) and solution file (`.sln`) are included, making it easy to open in Windows + Visual Studio.

## How to Use

1. Clone the repository:
	```
	git clone https://github.com/ngchenghow/DesignPattern_1.git
	```
2. Open `DesignPattern_1.sln` in Visual Studio (Windows).
3. Build and run. Review each folder’s code to see how the pattern is implemented and used.
4. As you study each pattern, compare with theoretical descriptions (e.g., from “Design Patterns: Elements of Reusable Object-Oriented Software”) and reflect how the C++-implementation maps to roles (Client, ConcreteFactory, AbstractProduct, etc).

## Contributions & Extensions

Feel free to fork and extend the repository:

- Add additional patterns (e.g., Singleton, Observer, State, Strategy) if missing.
- Provide comments or documentation in each folder explaining the pattern’s intent, usage, pros & cons.
- Integrate small unit tests to demonstrate correctness or typical usage scenarios.
- Update the code to use modern C++ (C++11/14/17) features where appropriate (smart pointers, `std::unique_ptr`, lambdas, etc).

## Ideal For

- Developers learning design patterns in C++.
- Engineers building frameworks or libraries in C++ (such as your own “mind”-based platform) who want to embed pattern-aware architecture.
- Reviewers or instructors preparing courses or workshops on design patterns, OO architecture and C++.
