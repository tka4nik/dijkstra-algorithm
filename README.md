
# **Dijkstra algorythm**
by: Tkachenko Nikita

---

### Describtion

Implementation of [Dijstra](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) algorythm in C++ language. Used to find the shortest route between two nodes of a graph.
Uses my own implementation of [Graph](https://github.com/tka4nik/Graph) class.
	

### Installation

You should have `git` and a compiler for C++/17 installed

Depends on [Graph](https://github.com/tka4nik/Graph) class repository.

```
git clone https://github.com/tka4nik/Dijkstra.git
git submodules update --init
```

### Usage
There is an example graph in `/workdir` folder.

```
mkdir build && cd build
cmake ..
cmake --build .
./dijstra --file ../workdir/graph.txt --from 0 --to 2
```
