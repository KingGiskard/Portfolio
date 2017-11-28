package assignment13;

import java.util.HashMap;

/**
 * Representation of an airport, featuring a set of flights
 * 
 * @author Samuel Thayer, Robert King
 */
public class Airport {
	/** Set of all the flights coming out of this airport */
	private HashMap<String, Flight> flights;

	/** This airport's name */
	private String name;

	/** Used in for path finding */
	private boolean visited;

	/**
	 * Constructs an airport with of the given name
	 * 
	 * @param name
	 *            The name of the airport
	 */
	public Airport(String name) {
		flights = new HashMap<String, Flight>();
		this.name = name;
		visited = false;
	}

	/**
	 * Constructs an airport with the same name as the origin of the given
	 * flight. The given flight will also be listed in the airport's flight
	 * list.
	 * 
	 * @param f
	 *            A flight originating from this airport
	 */
	public Airport(Flight f) {
		this(f.getOrigin());
		flights.put(f.getDestination(), f);
	}

	/**
	 * Returns the name of this airport
	 * 
	 * @return The name of this airport
	 */
	public String getName() {
		return name;
	}

	/**
	 * Adds a flight to this airport's list if no flight with the given
	 * destination exists; otherwise, averages the given flight's values into
	 * the pre-existing flight.
	 * 
	 * @param flight
	 *            A flight that should originate from this airport. The origin
	 *            is not checked though; it is assumed to originate from this
	 *            airport.
	 */
	public void addFlight(Flight flight) {
		if (flights.containsKey(flight.getDestination())) {
			flights.get(flight.getDestination()).updateAverage(flight);
			return;
		}
		flights.put(flight.getDestination(), flight);

	}

	/**
	 * Returns an array of all flights originating from this airport.
	 * 
	 * @return An array of flights
	 */
	public Flight[] getFlights() {
		return flights.values().toArray(new Flight[0]);
	}

	/**
	 * Returns true if this airport has been visited by the algorithm, and false
	 * otherwise
	 * 
	 * @return True if this airport has been visited by the algorithm, and false
	 *         otherwise
	 */
	public boolean isVisited() {
		return visited;
	}

	/**
	 * Sets whether this airport has been visited or not.
	 * 
	 * @param visited
	 *            Whether this airport has now been visited or not.
	 */
	public void setVisited(boolean visited) {
		this.visited = visited;
	}
}
