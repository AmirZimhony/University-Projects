package bgu.spl.mics.application.passiveObjects;

import bgu.spl.mics.Future;


import java.util.concurrent.*;


/**
 * Passive object representing the resource manager.
 * You must not alter any of the given public methods of this class.
 * <p>
 * This class must be implemented safely as a thread-safe singleton.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You can add ONLY private methods and fields to this class.
 */


public class ResourcesHolder {

    private final BlockingQueue<DeliveryVehicle> listOfCars;
    private Semaphore semaphore;
    private int numOfCars;// no one changes this value. this is for debugging purposes.


    private static class HolderOfResourceHolder {
        private static ResourcesHolder List = new ResourcesHolder();
    }
    private ResourcesHolder() {
        listOfCars = new LinkedBlockingQueue<DeliveryVehicle>();
    }
    /**
     * Retrieves the single instance of this class.
     */
    public static ResourcesHolder getInstance() {
        return HolderOfResourceHolder.List;
    }

    /**
     * Tries to acquire a vehicle and gives a future object which will
     * resolve to a vehicle.
     * <p>
     * @return 	{@link Future<DeliveryVehicle>} object which will resolve to a
     * 			{@link DeliveryVehicle} when completed.
     */

    public Future<DeliveryVehicle> acquireVehicle() {
        try {
            semaphore.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Future<DeliveryVehicle> ans = new Future<>();

        try {
            ans.resolve(listOfCars.take());
        } catch (InterruptedException e){
            System.out.println("Interrupted while acquiring a vehicle");
        }
            return ans;
    }

    /**
     * Releases a specified vehicle, opening it again for the possibility of
     * acquisition.
     * <p>
     * @param vehicle	{@link DeliveryVehicle} to be released.
     */
    public void releaseVehicle(DeliveryVehicle vehicle) {
        listOfCars.add(vehicle);
        System.out.println("a vehicle has been released");
        semaphore.release();
    }

    /**
     * Receives a collection of vehicles and stores them.
     * <p>
     * @param vehicles	Array of {@link DeliveryVehicle} instances to store.
     */
    public void load(DeliveryVehicle[] vehicles) {
        for (DeliveryVehicle v:vehicles){
            listOfCars.add(v);
        }
        numOfCars = vehicles.length;
        this.semaphore = new Semaphore(listOfCars.size());
    }

}
