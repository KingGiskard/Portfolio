package assignment13;

import static org.junit.Assert.*;

import java.io.FileNotFoundException;

import org.junit.Before;
import org.junit.Test;

public class TestNetworkGraph {

	private NetworkGraph sTestOne;
	private NetworkGraph sTestTwo;
	private  NetworkGraph airports;
	
	@Before
	public void setup () {
		try {
			sTestOne = new NetworkGraph("FlightTestCases/sTestOne.csv");
		} catch (FileNotFoundException e) {
			System.out.println("o_O");
		}
		try {
			sTestTwo = new NetworkGraph("FlightTestCases/sTestTwo.csv");
		} catch (FileNotFoundException e) {
			System.out.println("O_o");
		}
		try {
			airports = new NetworkGraph("FlightTestCases/testfile.csv");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	

	
	@Test
	public void airportsTestDelay() {
		BestPath shortestDelayPath1 = airports.getBestPath("KAR", "KPL", FlightCriteria.DELAY);
		assertEquals(219,(int)shortestDelayPath1.getPathLength());
		
		BestPath shortestDelayPath2 = airports.getBestPath("KAR", "RVQ", FlightCriteria.DELAY);
		assertEquals(198,(int)shortestDelayPath2.getPathLength());
	}
	
	@Test
	public void airportsTestTime() {
		BestPath shortestTestPath1 = airports.getBestPath("KAR", "KPL", FlightCriteria.TIME);
		assertEquals(193,(int)shortestTestPath1.getPathLength());
		
		BestPath shortestTimePath2 = airports.getBestPath("KAR", "RVQ", FlightCriteria.TIME);
		assertEquals(187,(int)shortestTimePath2.getPathLength());
	}
	
	@Test
	public void airportsTestDistance() {
		BestPath shortestDistancePath1 = airports.getBestPath("KAR", "KPL", FlightCriteria.DISTANCE);
		assertEquals(1336,(int)shortestDistancePath1.getPathLength());
		
		BestPath shortestDistancePath2 = airports.getBestPath("KAR", "RVQ", FlightCriteria.DISTANCE);
		assertEquals(1291,(int)shortestDistancePath2.getPathLength());
	}
	
	@Test
	public void airportsTestCost() {
		BestPath shortestDistancePath1 = airports.getBestPath("KAR", "KPL", FlightCriteria.COST);
		assertEquals(33813,(int)(100*shortestDistancePath1.getPathLength()));
		
		BestPath shortestDistancePath2 = airports.getBestPath("KAR", "RVQ", FlightCriteria.COST);
		assertEquals(73238,(int)(100*shortestDistancePath2.getPathLength()));
	}
	
	@Test
	public void airportsTestCanceled() {
		BestPath shortestDistancePath1 = airports.getBestPath("KAR", "KPL", FlightCriteria.CANCELED);
		assertEquals(100,(int)(100*shortestDistancePath1.getPathLength()));
		
		BestPath shortestDistancePath2 = airports.getBestPath("KAR", "RVQ", FlightCriteria.CANCELED);
		assertEquals(0,(int)(shortestDistancePath2.getPathLength()));
	}
	
	
	@Test
	public void sTestOneUOBYOXDelay() {
		BestPath path = sTestOne.getBestPath("UOB", "YOX", FlightCriteria.DELAY);
		assertEquals("Path Length: 157.5\nPath: [UOB, KOJ, YOX]", path.toString());
	}
	
	@Test
	public void sTestOneUOBYOXCost() {
		BestPath path = sTestOne.getBestPath("UOB", "YOX", FlightCriteria.COST);
		assertEquals("Path Length: 469.345\nPath: [UOB, YOX]", path.toString());
	}
	
	@Test
	public void sTestOneNonExistantDestination() {
		BestPath path = sTestOne.getBestPath("UOB", ">:P", FlightCriteria.COST);
		assertEquals("Path Length: 0.0\nPath: []", path.toString());
	}
	
	@Test
	public void sTestOneNoPathExists() {
		BestPath path = sTestOne.getBestPath("KOJ", "UOB", FlightCriteria.CANCELED);
		assertEquals("Path Length: 0.0\nPath: []", path.toString());
	}
	
	@Test
	public void sTestTwoZKVETXTime() {
		BestPath path = sTestTwo.getBestPath("ZKV", "ETX", FlightCriteria.TIME);
		assertEquals("Path Length: 771.0\nPath: [ZKV, UWC, ETX]", path.toString());
	}
	
	@Test
	public void sTestTwoETXZKVCanceled() {
		BestPath path = sTestTwo.getBestPath("ETX", "ZKV", FlightCriteria.CANCELED);
		assertEquals("Path Length: 1.0\nPath: [ETX, UWC, ZKV]", path.toString());
	}

}
