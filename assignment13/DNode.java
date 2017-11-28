package assignment13;

/**
 * Node Class for using Dijkstra's Algorithm. Each instance contains:
 * 
 * 1) A reference to an airport
 * 
 * 2) A value representing the cost of reaching this airport (where cost refers
 * to whatever FlightCriteria was given)
 * 
 * 3) A reference to the DNode that came before this one in the cheapest path to
 * this DNode.
 * 
 * @author Samuel Thayer, Robert King
 *
 */
public class DNode {
	/** The airport this DNode represents */
	private Airport airport;

	/**
	 * The total cost (measured by the appropriate FlightCriteria) required to
	 * reach this Airport
	 */
	private double value;

	/** The DNode came before this one in the cheapest path to this airport */
	private DNode previous;

	/**
	 * Constructs a DNode
	 * 
	 * @param airport
	 *            The airport this DNode represents
	 * @param cost
	 *            The cost required to reach this DNode (measured by the
	 *            appropriate FlightCriteria)
	 * @param previous
	 *            The DNode came before this one in the cheapest path to this
	 *            airport
	 */
	public DNode(Airport airport, Double cost, DNode previous) {
		this.airport = airport;
		this.value = cost;
		this.previous = previous;
	}

	/**
	 * Returns the airport this DNode represents
	 * 
	 * @return The airport this DNode represents
	 */
	public Airport getAirport() {
		return airport;
	}

	/**
	 * Returns the cost required to reach this DNode (measured by the
	 * appropriate FlightCriteria)
	 * 
	 * @return The cost required to reach this DNode (measured by the
	 *         appropriate FlightCriteria)
	 */
	public double getValue() {
		return value;
	}

	/**
	 * Returns the DNode came before this one in the cheapest path to this
	 * airport
	 * 
	 * @return The DNode came before this one in the cheapest path to this
	 *         airport
	 */
	public DNode getPrevious() {
		return previous;
	}

}