/**
 * 
 */
package assignment13;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.PriorityQueue;
import java.util.Scanner;

/**
 * <p>
 * This class represents a graph of flights and airports along with specific
 * data about those flights. It is recommended to create an airport class and a
 * flight class to represent nodes and edges respectively. There are other ways
 * to accomplish this and you are free to explore those.
 * </p>
 * 
 * <p>
 * Testing will be done with different criteria for the "best" path so be sure
 * to keep all information from the given file. Also, before implementing this
 * class (or any other) draw a diagram of how each class will work in relation
 * to the others. Creating such a diagram will help avoid headache and confusion
 * later.
 * </p>
 * 
 * <p>
 * Be aware also that you are free to add any member variables or methods needed
 * to completed the task at hand
 * </p>
 * 
 * @author CS2420 Teaching Staff - Spring 2016, Samuel Thayer, Robert King
 */
public class NetworkGraph {

	private HashMap<String, Airport> airports;

	/**
	 * <p>
	 * Constructs a NetworkGraph object and populates it with the information
	 * contained in the given file. See the sample files or a randomly generated
	 * one for the proper file format.
	 * </p>
	 * 
	 * <p>
	 * You will notice that in the given files there are duplicate flights with
	 * some differing information. That information should be averaged and
	 * stored properly. For example some times flights are canceled and that is
	 * represented with a 1 if it is and a 0 if it is not. When several of the
	 * same flights are reported totals should be added up and then reported as
	 * an average or a probability (value between 0-1 inclusive).
	 * </p>
	 * 
	 * @param flightInfoPath
	 *            - The path to the file containing flight data. This should be
	 *            a *.csv(comma separated value) file
	 * 
	 * @throws FileNotFoundException
	 *             The only exception that can be thrown in this assignment and
	 *             only if a file is not found.
	 */
	public NetworkGraph(String flightInfoPath) throws FileNotFoundException {
		airports = new HashMap<>();

		File inputFile = new File("" + flightInfoPath);
		// We throw a FileNotFoundException, so no need for catch statement
		// The try is only for managing the scanner
		try (Scanner scan = new Scanner(new BufferedReader(new FileReader(inputFile)))) {

			// Skip the title line
			scan.nextLine();

			while (scan.hasNextLine()) {

				// Use the line to make a flight
				String[] rawData = scan.nextLine().split(",");
				Flight inputFlight = initializeFlight(rawData);

				// Create the origin airport if necessary, just add the flight
				// otherwise
				String key = inputFlight.getOrigin();
				if (airports.containsKey(key)) {
					airports.get(key).addFlight(inputFlight);
				} else {
					Airport newAirport = new Airport(inputFlight);
					airports.put(key, newAirport);
				}

				// Create destination airport (w/ no flights) if necessary
				key = inputFlight.getDestination();
				if (!airports.containsKey(key)) {
					airports.put(key, new Airport(key));
				}

			}
		}

	}

	/**
	 * Makes a Flight out of a string array containing the appropriate data.
	 * Behavior is undefined for invalid input.
	 * 
	 * @param rawData
	 *            The data to use when constructing the Flight
	 * @return A Flight constructed out of the given data
	 */
	private Flight initializeFlight(String[] rawData) {
		Flight returnFlight;
		double delay = Double.parseDouble(rawData[3]);
		double canceled = Double.parseDouble(rawData[4]);
		double time = Double.parseDouble(rawData[5]);
		double distance = Double.parseDouble(rawData[6]);
		double cost = Double.parseDouble(rawData[7]);
		returnFlight = new Flight(rawData[0], rawData[1], rawData[2], delay, canceled, time, distance, cost);
		return returnFlight;
	}

	/**
	 * This method returns a BestPath object containing information about the
	 * best way to fly to the destination from the origin. "Best" is defined by
	 * the FlightCriteria parameter <code>enum</code>. This method should throw
	 * no exceptions and simply return a BestPath object with information
	 * dictating the result. For example, if a destination or origin is not
	 * contained in this instance of NetworkGraph simply return a BestPath with
	 * no path (not a <code>null</code> path). If origin or destination are
	 * <code>null</code> return a BestPath object with null as origin or
	 * destination (which ever is <code>null</code>.
	 * 
	 * @param origin
	 *            - The starting location to find a path from. This should be a
	 *            3 character long string denoting an airport.
	 * 
	 * @param destination
	 *            - The destination location from the starting airport. Again,
	 *            this should be a 3 character long string denoting an airport.
	 * 
	 * @param criteria
	 *            - This enum dictates the definition of "best". Based on this
	 *            value a path should be generated and return.
	 * 
	 * @return - An object containing path information including origin,
	 *         destination, and everything in between.
	 */
	public BestPath getBestPath(String origin, String destination, FlightCriteria criteria) {
		// Call the other getBestPath() with a null airliner requirement
		return getBestPath(origin, destination, criteria, null);
	}

	/**
	 * <p>
	 * This overloaded method should do the same as the one above only when
	 * looking for paths skip the ones that don't match the given airliner.
	 * </p>
	 * 
	 * @param origin
	 *            - The starting location to find a path from. This should be a
	 *            3 character long string denoting an airport.
	 * 
	 * @param destination
	 *            - The destination location from the starting airport. Again,
	 *            this should be a 3 character long string denoting an airport.
	 * 
	 * @param criteria
	 *            - This enum dictates the definition of "best". Based on this
	 *            value a path should be generated and return.
	 * 
	 * @param airliner
	 *            - a string dictating the airliner the user wants to use
	 *            exclusively. Meaning no flights from other airliners will be
	 *            considered.
	 * 
	 * @return - An object containing path information including origin,
	 *         destination, and everything in between.
	 */
	public BestPath getBestPath(String origin, String destination, FlightCriteria criteria, String airliner) {

		// If either the destination or the origin doesn't exist, no path
		// exists. Return appropriately.
		if (!airports.containsKey(destination) || !airports.containsKey(origin)) {
			return new BestPath(null);
		}

		// Reset the airports for a new search
		setVisistedToFalse();

		// Set up a PriorityQueue, and the first DNode
		PriorityQueue<DNode> queue = new PriorityQueue<>(new DComparator());
		DNode currentNode = new DNode(airports.get(origin), 0.0, null);

		// Dijkstra!
		// Exit loop if either we run out of DNodes (meaning no path exists)
		// Or if we found the destination
		while (currentNode != null && !currentNode.getAirport().getName().equals(destination)) {

			// Mark this node and add any new destinations from this flight to
			// the queue
			currentNode.getAirport().setVisited(true);
			Flight[] flights = currentNode.getAirport().getFlights();
			for (Flight flight : flights) {
				// If airliner is null, we have no flight requirement. Consider
				// all flights from this airport.
				// Otherwise, only consider flights that are run by the given
				// airliner.
				if (airliner == null || flight.getCarriers().contains(airliner)) {
					// Either way, flights must also be unvisited to be added to
					// the queue.
					if (!airports.get(flight.getDestination()).isVisited()) {
						// All tests were passed, so create a DNode out of the
						// airport and add it
						DNode newNode = new DNode(airports.get(flight.getDestination()),
								currentNode.getValue() + flight.getValue(criteria), currentNode);
						queue.add(newNode);
					}
				}
			}

			// Get the next DNode and repeat
			currentNode = queue.poll();
		}

		// Exiting the loop means that currenNode has all the information
		// BestPath's constructor needs, even if no path exists
		return new BestPath(currentNode);
	}

	/**
	 * Guarantees that the isVisited() method of all airports will return false,
	 * until their setVisited() method is next called.
	 */
	private void setVisistedToFalse() {
		Iterator<Airport> it = airports.values().iterator();
		while (it.hasNext()) {
			it.next().setVisited(false);
		}

	}

	/**
	 * A comparator to use when ordering DNodes for running Dijkstra's
	 * Algorithm.
	 * 
	 * @author Samuel Thayer, Robert King
	 */
	private class DComparator implements Comparator<DNode> {

		@Override
		public int compare(DNode lhs, DNode rhs) {
			double comparison = lhs.getValue() - rhs.getValue();

			// Use a tolerance value to compare if doubles are the same
			if (Math.abs(comparison) < 0.00000001) {
				return 0;
			}
			// If not equal, return an int that is appropriately positive or
			// negative
			else if (comparison < 0) {
				return -1;
			} else {
				return 1;
			}

		}

	}
}
