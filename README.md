<h1 align="center">CS225 Final Project: Open Flights</h1>

<h1 align="center">CS225 Final Project: Open Flights</h1>

## Presentation Video
Please watch on YouTube: https://youtu.be/bvAs_9HUSPo

## Data
We are using the Airport database and Route database from [OpenFlights](https://openflights.org/data.html)' dataset collection:
* [Airport Data](https://raw.githubusercontent.com/jpatokal/openflights/master/data/airports.dat)
* [Routes Data](https://raw.githubusercontent.com/jpatokal/openflights/master/data/routes.dat)

## Repo Structure
### Deliverables

- entry of our program is in `/entry/main.cpp`
- Source codes and our own header files are stored in `/src` directory. 
- Datasets are stored as txt file in `/data`
- Test cases are stored in `/tests`.
- Proposal, development logs, written report, etc. are stored in `/documents`. 
- Included libraries are stored in `/lib`. 

## Running Instructions

We are using CMake to complie. If you want to use our test case, please make sure you have Catch2 installed. Docker container is not required for compiling.

### Important functions

**shortestPath**: This function returns the results of a specific shortest path calculated by calcPrevious. This function takes a source airport IATA and a destination airport IATA, and returns the shortest path between them as a vector of strings if such a path exists. The result of this function will be an empty vector if either source or destination is not included in the graph. This function will return a vector with only the destination IATA if there is not a path from source to destination to distinguish this situation from the former ones.

**calcFrequency**: "Frequency" here refers to the centrality score in Betweenness centrality Algorithm. It the centrality score for all airports.

**RoundTrip**: 

## Team Members
* Ellen Yang (`yaweny2`)
* Encheng Xie (`encheng2`) 
* Ruijia Lyu (`ruijial3`)
* Yue Chen (`yuec12`)
