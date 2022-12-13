# Development Logs

---

## **Week 1: (Date: 11/03/22 -- 11/04/22)**

### Weekly Goals
Decide our project goals and finish our project proposal. If possible, start working on the project.

### Specific Tasks Completed (and by who)
This week, we as a group discussed details about project goals and completed project proposals.
Ruijia and Ellen worked together on writing the team contract.
Yue opened the project git repository and added required members to the project.
Encheng and Yue have significantly contributed to the initial design of the project’s data structure for storing the graph while other members have added comments. 
All members contributed for writing the project proposal.

### Problems Encountered 
This week went smoothly as we didn’t encounter any unsolvable problems inside our group.

### Plans for next week
Our plan for next week is to finish writing the data structure and storing the dataset into a functional graph. We also plan to start implementing a function of finding the shortest path between two airports (given departure and arrival) using the Dijkstra’s Algorithm, and start writing the test case for this function.

### Optional: Questions for mentor
None

---
## **Week 2: (Date: 11/05/22 -- 11/11/22)**

### Weekly Goals
Set up the dataset and work on the graph. Implement BFS traversal for the graph. Start implementing the shortest path algorithm with Dijkstra’s Algorithm. If possible, write test cases for shortest path (Dijstra’s)

### Specific Tasks Completed (and by who)
Monday(11/07/22)

All members set up github and local dock containers. We also tested and got familiar with git branch.
Yue wrote naive_strucutres.h which contains the initial ideas of our data structure of the graph.

Tuesday (11/08/22)

Encheng:
Developed structure files that contain all structs needed for the project.
Developed class receiving and converting data file to operable code structure (graph).

Yue Chen:
Mimicked the CMakeLists in our MP and lab in our project so that we can use cmake to compile our file easily in the future. 

Friday (11/11/22)

Ellen Yang, Ruijia Lyu, Yue Chen:
Worked on Graph files that contain all the functions we need for Dijkstra’s algorithm.
Start working on implementing BFS and Dijkstra’s files.

### Problems Encountered 
This week we didn’t encounter any unsolvable problems inside our group.

### Plans for next week
Our plan for next week is to finish implementing the shortest path algorithm with Dijkstra's algorithm. And prepare for the mid-project checkpoint. We also plan to start implementing the busiest airports with the betweenness centrality if we have the time. 

### Optional: Questions for mentor
We are wondering if we need to write operator++ for BFS.

---
## **Week 3: (Date: 11/12/22 -- 11/18/22)**

## Weekly Goals

Complete Dijkstra's Algorithm for finding the shortest path and BFS of the graph. Write the test cases for Dijkstra’s algorithm. Test for the functions of finding the shortest path between airports.

## Specific Tasks Completed (and by who)

Ruijia Lyu: Wrote the function shortestPath and allShortestPath that returns the shortest paths from one airport to another airport/the rest of the airports in the graph based on Dijkstra’s algorithm. Fixed the way to update the priority queue during Dijkstra’s algorithm’s calculation of the shortest path.

Ellen Yang: Wrote calculateDistance in graph_generation. Wrote test cases for shortestPath and allShortestPath.

Yue Chen:

(1) Build the framework of our shortest path functions and finished implementing Dijkstra algorithm in our functions. A bug has been fixed by Ruijia Lyu. Thanks!

(2) Finished the BFS search. So that we can use it to check whether our graph is connected or not. 

(3) Created a graph using basic graph functions. It is a very small test case with only 8 nodes and 12 edges.

(4) Fixed the constructor for both structs in our structures.h


Encheng Xie:

1. Data conversion completed

2. Eulerian Path code structure starts

## Problems Encountered 

We have already implemented all the functions required for calculating shortes paths using Dijkstra's algorithm. Though we have tested it with a few simple and small test cases, we still need to test our functions with real data in the data set. 

While writing the graph generation file of generating a graph from the airport and route files from https://openflights.org/data.html, we realized that some of the airport names in the route file include commas (“,”) which affect the file reading process. We plan to discuss and fix this problem on Sunday (11/20) this week.

## Plans for next week

We will be finishing our testing for shortest path finding functions using data from airports.txt and begin to implement the betweenness centrality algorithm. We will also be fixing the issue with file reading. 

## Optional: Questions for mentor

If we have the ability to get the shortest path starting from any point, we can use that to try to calculate the betweenness centrality of our graph. But do we need to use Floyd-Warshall algorithm for calculating all the shortest path for this goal? We are currently using adjacent list for storing, so will this make our algorithm slower?

---
## **Week 4: (Date: 11/19/22 -- 12/02/22)**

## Weekly Goals

Complete testing for shortest path finding functions using data from airports.txt and route.txt
Complete betweenness centrality algorithm. 

## Specific Tasks Completed (and by who)

Yue Chen:

(1) In order to increase the running speed of our program, I made some significant changes on everywhere. Instead of using string IATA as the unique specifier of each airport, I used airport ID, which is represented using unsigned integer, as our specifier. This increases about 50% of the running speed. To make these work, I added some functions and make it work.
(2) The work above would obviously bring lots of bug. So Ruijia and I debugged most part of the program, so that we can now pass all test cases.

(3) Again, together with Ruijia, we finished the basic centrality function. 

(4) Partially done the function that can write the betweenness centrality to a file so that in the future, we can write functions to read from it.

(5) Minor contribution to data correction.

(6) Published releases of our history versions on GitHub so that we can trace back our work at anytime. 

Ruijia Lyu: 

Completed betweenness centrality algorithm function calcFrequency. Helped fix some bugs on certain branches that switched the airport map storing from IATA to ID.

Ellen Yang: 

Wrote test cases for Dijkstra's Algorithm and finished testing shortest path functions.

Encheng Xie:

Eulerian algorithm started

## Problems Encountered 

Small problem: Betweenness centrality takes about 60 seconds to run locally, 90 seconds in the docker container. We don't know if this is a normal speed for these dataset. Currently our time complexity of betweenness centrality is:
$$O(mn*log(n) + n^2 *log(n) + n^2), n = |V|, m = |E|, m > n.$$ So leading term is $$O(mn*log(n))$$


## Plans for next week

We will finish writing test cases for betweenness centrality algorithms and complete implementing the multi-cities round trip using Eulerian path. We will also be starting working on the final project deliverables.

 
## Optional: Questions for mentor

Is the complexity in the "problems encountered" section acceptable?
