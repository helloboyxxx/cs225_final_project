## Leading Question 

As a group of people who love to travel all over, we are interested in learning how flights and airports location perform and how those performances should affect the decisions of the travelers. Given a dataset, our code will be able to operate: The shortest path to travel from one airport to another: To calculate the shortest path between two airports considering the distance of the path, we will be using Dijkstra's Algorithm on a directed weighted graph. We will take airports as nodes, routes between airports as edges, and distance of the routes as weights of the corresponding edges. The busiest airports in the world: As long as we have the ability to calculate the shortest path between any two nodes, we can implement the Betweenness Centrality algorithm to calculate the importance of each airport. Multi-cities round trip: If time allows, the next update would be to generate the most optimal path traveling as many cities as possible without revisiting any one of them, and eventually return to the home city (the input city). The process which would be performed by Eulerian Path.

## Dataset Acquisition

The dataset we are using is the Airport database and Route database from a dataset collection (OpenFlights: https://openflights.org/data.html).

## Data Format

The input dataset contains two parts, both from OpenFlights.org. The first part is a file containing information of airports around the world. Each line contains the following information separated by comma: Airport ID, Airport Name, Airport Located City, Airport Located Country, IATA, ICAO, Latitude, Longitude, Altitude, Timezone, DST, Tz database time zone, Type, Source. This file is about 1.1MB. The second part is a file containing information of the routes between airports in the world. Each line contains the following information separated by comma: Airline, Airline ID, Source airport, Source airport ID, Destination airport, Destination airport ID, Codeshare, Stops, Equipment. This file is about 2MB.

Since no specific information of the stops are given from the Route database file, we will be only considering flights with zero stops. Since our purpose is to provide a globalwide air transportation network, we will use the whole dataset of the airports. We will use the airport dataset to get information about latitude and longitude, so to calculate the cost of each path. The route dataset will be used as our edge in the graph. Additionally, we will use the information of source airport and destination airport in the route dataset to decide the direction of our edges.

## Data Correction

Since the two data files we are using are not updated simultaneously, hence a potential error is very likely to occur: airport information does not match. More precisely, one route from the route file may indicate an IATA does not exist in the airport file; probably due to IATA-renaming, failure of collecting that airport to airport file, etc. Since there are so many airports, each has not just one route, the impact of lack of one route is small enough to be omitted. Hence, we choose to simply ignore that information and move to the next one.

## Data Storage

In this project, we are going to calculate the shortest path many times while we won’t modify the data structure a lot. So we choose to use an adjacent list as our main data structure to store this graph. According to what we have learnt, this structure will store: All the vertices as a STL vector. Space complexity will be O(n). Every airport has a STL list of adjacent edges. Add up to O(2m) according to the handshake theory. (This is the worst case since the graph we are implementing is a connected directed graph, so complexity can be less than O(2m)) Edge list as a STL list. This is equal to the number of edges, so O(m) To sum up, we have an overall space complexity of O(n + m) This is a very general estimation of our space complexity, but we still need to explore which specific auxiliary structure (whether using STL vector / STL list / STL map etc).

## Algorithm

Function Inputs: Our group will use Dijkstra’s Algorithm to calculate the shortest flight routes between two airports. We will first use the airports dataset to get the nodes for our graph. Then we will use the routes dataset to get the edges for the graph. For the second part of our project we will be using betweenness centrality algorithm with the graph we get in the first part to find the result. If time allows, we will use the Eulerian Path when implementing the function. Since we are generating a path for a global trip depends on the starting city, where the person is currently at. Therefore, expected input is the IATA of the airport where the traveler decides to start the trip.

Function Outputs: For the first part of our project, the expected output for the Dijkstra’s Algorithm is the sequence of strings of the airports’ IATA that are included in the shortest distance between the given departure airport and arrival airport. For instance, the sequence “ORD-LAX” means the shortest distance from Chicago O’Hare Airport to Los Angeles Airport is to fly directly without taking stops during the trip. For finding the busiest airports in the world, the expected output will be a vector storing the ranking of all the airports from the airports with the most number of flights to the airports with the least number of flights available. For the Eulerian Algorithm, the expected output will be a sequence of strings, representing airports that should fly to, in order to travel the most area around the world. For instance, the sequence “LAX-ORD-LAX” means flying from Los Angeles to Chicago and then to Los Angeles. 

Function Efficiency: The function efficiency is significantly based on our shortest path algorithm. Since we wish to use Dijkstra’s algorithm, the expected complexity of this algorithm is O(n^2) if it is based on arrays, where n is the number of vertices in our representation. The Betweenness Centrality algorithm could take a huge amount of time to calculate. We broadly researched this algorithm and found that an efficient way of implementing this algorithm has complexity O(nm + (n^2)logn). So, based on our implementation, we might have to take up to O(n^4). But since we will only use a small subset for this part, we could still run the algorithm in a reasonable amount of time.

## Timeline

###### Week 1 (Nov 3 - Nov 4):

- Decide our project goals 
- Finalize and polish the project proposal 

###### Week 2 (Nov 5 - Nov 11):

- Set up the dataset 
- Implement BFS traversal for the graph 
- Start implement the shortest path algorithm with Dijkstra’s Algorithm 
- Write test cases for shortest path (Dijstra’s)

###### Week 3 (Nov 12 - Nov 18):

Mid-Project Check In (Nov 14-18):

- Finish implement the shortest path algorithm with Dijkstra’s Algorithm
-  Start implement the busiest airports with the betweenness centrality

######  Week 4 (Nov 19 - Nov 25):

- Finish finding the busiest airports in the world ranking with betweenness centrality 
- Write test cases for betweenness centrality algorithms

######  Week 5 (Nov 26- Dec 2):

- Finish any unfinished parts 
- If time allows, implement the multi-cities round trip using the Eulerian path 

###### Week 6 (Dec 3 - Dec 8):

*Final Project Deliverables (Dec 8) 

- Code and documentation clean-up 
- Complete a written report and a final presentation