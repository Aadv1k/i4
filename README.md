# i4

`i4` (or inline4) is a byte-code compiler and virtual machine for a simple JavaScript-like language. It is an exercise in learning C, as well as writing quality (hopefully!) software by hand without the use of LLMs (see [LLM disclosure](#llm-disclosure)) below.

Current goal: get the following compiled into bytecode, and get an interpreter to run it.

```javascript
var x = 30;
var y = 37;

var z = x + y;
```

## Quickstart

```sh
chdmox +x build.sh
./build
./dist/i4_test
```



### LLM disclosure

One of my major motivators to build this project was to take a step back from agentic programming and the spec-driven culture around software engineering, that discourages being meticulous about every single line of code, or writing code by hand, instead focusing on the overall HLD, LLD and supervising the agent. 

With this project, every single line of code (including documentation, tests, scripts) has been written 100% by hand. LLMs have only been used for learning, qualitative code review & and as a search engine. 

I don't hold strong opinions around agentic programming, I believe it has it's place, but for this project I wanted to take a step back, and write code purely recreationally.
