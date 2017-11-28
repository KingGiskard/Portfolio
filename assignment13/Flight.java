package assignment13;

import java.util.HashSet;

/**
 * Represents a flight, or a one-way connection between airports. Contains the
 * flight's origin; destination; a list of carriers; and the average value for
 * each cost, distance, time, cancellation chance, and delay.
 * 
 * @author Samuel Thayer, Robert King
 *
 */
public class Flight implements Comparable<Flight> {
	/** Origin of the flight (airport acronym) */
	private String origin;

	/** Destination of the flight (airport acronym) */
	private String destination;

	/** List of all carriers that make this flight */
	private HashSet<String> carriers;

	/** Average minutes the flight is delayed */
	private double delay;

	/** Chance of the flight being canceled */
	private double canceled;

	/** Number of minutes for the flight to reach its destination */
	private double time;

	/** Length of flight in miles */
	private double distance;

	/** Cost of flight in USD */
	private double cost;

	/**
	 * Stores the number of flights that made this same trip, used for computing
	 * rolling averages
	 */
	private int count;

	/**
	 * Constructs a Flight.
	 * 
	 * @param origin
	 *            Origin of the flight (airport acronym)
	 * @param destination
	 *            Destination of the flight (airport acronym)
	 * @param carrier
	 *            A carrier that makes this flight
	 * @param delay
	 *            Minutes the flight was delayed
	 * @param canceled
	 *            Whether the flight was canceled
	 * @param time
	 *            Number of minutes for the flight to reach its destination
	 * @param distance
	 *            Length of flight in miles
	 * @param cost
	 *            Cost of flight in USD
	 */
	public Flight(String origin, String destination, String carrier, double delay, double canceled, double time,
			double distance, double cost) {
		this.origin = origin;
		this.destination = destination;
		carriers = new HashSet<>();
		carriers.add(carrier);
		this.cost = cost;
		this.distance = distance;
		this.time = time;
		this.delay = delay;
		this.canceled = canceled;	
		count = 1;

	}

	/**
	 * Using another flight with the same origin and destination as this one,
	 * update this one's values to reflect the average of all flights ever
	 * passed as a parameter to this method, and including the one this flight
	 * was constructed as.
	 * 
	 * @param flight
	 *            The flight whose values should be averaged into this one's.
	 */
	public void updateAverage(Flight flight) {
		// Verify that we were given an appropriately similar same flight
		if (!this.origin.equals(flight.getOrigin())) {
			throw new IllegalArgumentException("Orgin of flights must be the same");
		}
		if (!this.destination.equals(flight.getDestination())) {
			throw new IllegalArgumentException("Destination of flights must be the same");
		}

		// adjust variables
		carriers.addAll(flight.getCarriers());
		this.cost = (this.cost * count + flight.getCost()) / (count + 1);
		this.distance = (this.distance * count + flight.getDistance()) / (count + 1);
		this.time = (this.time * count + flight.getTime()) / (count + 1);
		this.canceled = (this.canceled * count + flight.getCanceled()) / (count + 1);
		this.delay = (this.delay * count + flight.getDelay()) / (count + 1);

		// increase count
		count++;

	}

	/**
	 * Returns the origin of this flight.
	 * 
	 * @return The origin of this flight
	 */
	public String getOrigin() {
		return origin;
	}

	/**
	 * Returns the destination of this flight.
	 * 
	 * @return The destination of this flight
	 */
	public String getDestination() {
		return destination;
	}

	/**
	 * Returns a set of the carriers of this flight.
	 * 
	 * @return A set of the carriers of this flight
	 */
	public HashSet<String> getCarriers() {
		return carriers;
	}

	/**
	 * Returns the cost of this flight.
	 * 
	 * @return The cost of this flight
	 */
	public double getCost() {
		return cost;
	}

	/**
	 * Returns the distance of this flight.
	 * 
	 * @return Distance of this flight
	 */
	public double getDistance() {
		return distance;
	}

	/**
	 * Returns the time of this flight.
	 * 
	 * @return The time of this flight
	 */
	public double getTime() {
		return time;
	}

	/**
	 * Returns the cancellation chance of this flight.
	 * 
	 * @return The cancellation chance of this flight
	 */
	public double getCanceled() {
		return canceled;
	}

	/**
	 * Returns the average delay time of this flight.
	 * 
	 * @return The average delay time of this flight
	 */
	public double getDelay() {
		return delay;
	}

	@Override
	public boolean equals(Object o) {
		if (o instanceof Flight) {
			Flight other = (Flight) o;
			return this.origin.equals(other.getOrigin()) && this.destination.equals(other.getDestination());
		}
		return false;
	}

	@Override
	public int compareTo(Flight o) {
		return this.destination.compareTo(o.getDestination());
	}
	
	/**
	 * Returns the value associated to this flight that is specified by the given FlightCriteria.
	 * @param type Specifies the value that should be returned
	 * @return The value associated to this flight that is specified by the given FlightCriteria.
	 */
	public double getValue(FlightCriteria type) {
		switch (type) {
		case COST:
			return cost;
		case DELAY:
			return delay;
		case DISTANCE:
			return distance;
		case CANCELED:
			return canceled;
		default:
			return time;
		}
	}

}
