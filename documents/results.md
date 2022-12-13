# Final Report

## Algorithm Overview

### Dijkstra’s

Out implementation of Dijkstra’s algorithm mainly included three functions:

- calcPrevious: This function holds the calculation part of Dijkstra’s algorithm. This function takes two inputs. The first one is the ID of a source airport which is the starting vertex in Dijkstra’s algorithm calculation. The second one is a reference to a map “previous” which is updated to the correct previous map after the calculation of Dijkstra’s algorithm. This function also serves as a helper function for the two functions below.
- shortestPath: This function returns the results of a specific shortest path calculated by calcPrevious. This function takes a source airport IATA and a destination airport IATA, and returns the shortest path between them as a vector of strings if such a path exists. The result of this function will be an empty vector if either source or destination is not included in the graph. This function will return a vector with only the destination IATA if there is not a path from source to destination to distinguish this situation from the former ones.
- allShortestPath: This function returns the results of the shortest paths calculated by calcPrevious. This function takes a source airport IATA and returns a vector of paths where each path takes the same form as the output of shortestPath.

Dijkstra’s algorithm runs at a time complexity $O((m+n) * log(n))$ in our program where $m$ represents the number of edges in our graph and $n$ represents the number of vertices in our graph. Our implementation of this algorithm changed the std::priority_queue to std::set as both structures perform the same as they both input (with sorting) and remove with time complexity $log(n)$. The reason for this modification is that we realized std::set gave a better runtime in our docker container, although they have the same performance locally.

To ensure the accuracy of our implementation of Dijkstra’s algorithm, we wrote test cases taken from the pictured example given in the CS 225 lecture.

### Betweenness Centrality

We use the function “calcPrevious”, which implements Dijkstra’s algorithm, for calculating the shortest paths starting from every airport. It then accumulates the centrality scores along all shortest paths. What we have done is relatively naive, so the time complexity is higher than optimal algorithms. Specifically, for each airport, we run:

- Dijkstra’s algorithm. This algorithm takes $O((m+n) *log(n))$
- and then a function that accumulates scores. It has the worst case complexity $O(n^2)$ assuming each shortest path has length = $n$. But the length of every shortest path should be a small value in a normal graph, meaning that this $n^2$ should have a small constant. 

We conclude that this function has time complexity $$O(n^3 + mnlog(n) + n^2log(n))$$

We tested this function by manually creating small graphs and counting the number of centrality scores for each vertex. 

### Hierholzer’s

Our Hierholzer’s algorithm(roundTrip) takes in an IATA of the starting airport and outputs a file that contains the airports’ IATA which represents the cycle route from the given airport. The algorithm contains two main parts, the first part is to generate a cycle graph, which is based on the original graph created using the airport.txt. The cycle graph we generate is strongly connected and is undirected between each node. The second part is Hierholzer’s algorithm implementation and it has two main helper functions:

- cycleDFS: This function takes two inputs. The first input is the cycle graph we generated. The second input is the ID of the given airport in roundTrip. This function will return a vector of airports’ ID that represent the sequence of airports visited throughout the round trip. It has a helper, also named cycleDFS, which updates the vector representing the route.
- hasOtherPath: This function serves as a helper function of the previous function. The purpose of this function is to serve as backtracking from the current vertex to the previous one.

This algorithm allows us to implement the multi-cities round trip functionality in our program.

Hierholzer's algorithm runs at a time complexity of $O(m^2+n^2)$ in our program.

We checked our results of this algorithm by writing a small graph and check all vertex have been walk through.

## Leading Questions

By successfully implementing the algorithms above, we are able to answer our questions: how flights and airports' locations perform and how these results affect travelers’ decisions.

One thing we found interesting is that the most important airport we find using the centrality algorithm is Keflavik International Airport located in Iceland. 

We can now use our implementation of Dijkstra’s Algorithm to calculate the shortest path between any two airports in the world. We now have a better understanding of how airlines' graphs or any other graphs may determine the shortest paths using this algorithm. 

The Betweenness Centrality Algorithm also proves to us that some well-known airports are indeed very busy. Even when we only use the direct distance between airports as the weight of each route, it gives us a relatively good estimation of “the busiest airports in the world”.
In the end, Hierholzer’s algorithm provides us with a special route-planning method that we have never thought of before. 
