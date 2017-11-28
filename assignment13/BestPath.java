/**
 * 
 */
package assignment13;

import java.util.ArrayList;
import java.util.LinkedList;

/**
 * This class is what we will test your code on. If your BestPath
 * objects equal ours (using the .equals method given) then you will
 * pass the tests. Do no modify anything that is given (use it of
 * course but don't change names etc.)
 * 
 * @author CS2420 Teaching Staff - Spring 2016, Samuel Thayer, Robert King
 */
public class BestPath {
	
	/**
	 * This should contain the nodes between the origin and destination
	 * inclusive. For example if I want the path between SLC and MEM it
	 * should have SLC (index 0), MEM (index 1). If there are lay overs
	 * it should include them in between (turns out you can fly to Memphis
	 * from here directly).
	 */
	private ArrayList<String> path;
	
	/**
	 * Since some path costs are going to be doubles sometimes use a double
	 * when costs are integers cast to a double.
	 */
	private double pathLength;

	/**
	 * Works backwards from the given node to create the best path.
	 * If destination is null, no path exists, and this object will be created accordingly.
	 * @param destination The DNode to work backwards from
	 */
	public BestPath(DNode destination) {
		
		if (destination == null) {
			path = new ArrayList<>();
			return;
		}
		pathLength = destination.getValue();
		
		LinkedList<String> path = new LinkedList<>();
		DNode pathNode = destination;
		while (pathNode!=null)
		{
			path.addFirst(pathNode.getAirport().getName());
			pathNode = pathNode.getPrevious();
		}
		
		this.path = new ArrayList<>(path);
	}

	@Override
	public boolean equals(Object o) {
		if (o instanceof BestPath) {
			BestPath other = (BestPath) o;
			return this.pathLength == other.pathLength && this.path.equals(other.path);
		}
		return false;
	}
	
	@Override
	public String toString() {
		return "Path Length: " + pathLength + "\nPath: "+ this.path;
	}
	
	/**
	 * returns the length of the path as a double.
	 * @return the length of the best path
	 */
	public double getPathLength()
	{
		return pathLength;
	}
}
