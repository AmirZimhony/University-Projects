package bgu.spl.mics.application.passiveObjects;
import java.io.Serializable;
/**
 * Passive data-object representing a receipt that should 
 * be sent to a customer after the completion of a BookOrderEvent.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You may add fields and methods to this class as you see fit (including public methods).
 */
public class OrderReceipt implements Serializable{

	private int orderId;
	private String Seller;// will hold the name of the service that handled the order.
	private int customerId;
	private String title;
	private int price;
	private int orderTick; // tick in which customer ordered the book
	private int proccessTick;// tick in which the selling service started processing the order
	private int issuedTick;//tick in which this receipt was issued (when corresponding event had been completed).

	public OrderReceipt(int orderId, String Seller, int customerId, String title,
						int price, int orderTick, int proccessTick, int issuedTick) {
		this.orderId = orderId;
		this.Seller = Seller;
		this.customerId = customerId;
		this.title = title;
		this.price = price;
		this.orderTick = orderTick;
		this.proccessTick = proccessTick;
		this.issuedTick = issuedTick;
	}
	
	/**
     * Retrieves the orderId of this receipt.
     */
	public int getOrderId() {
		return orderId;
	}
	
	/**
     * Retrieves the name of the selling service which handled the order.
     */
	public String getSeller() {
		return Seller;
	}
	
	/**
     * Retrieves the ID of the customer to which this receipt is issued to.
     * <p>
     * @return the ID of the customer
     */
	public int getCustomerId() {
		return customerId;
	}
	
	/**
     * Retrieves the name of the book which was bought.
     */
	public String getBookTitle() {
		return title;
	}
	
	/**
     * Retrieves the price the customer paid for the book.
     */
	public int getPrice() {
		return price;
	}
	
	/**
     * Retrieves the tick in which this receipt was issued.
     */
	public int getIssuedTick() {
		return issuedTick;
	}
	
	/**
     * Retrieves the tick in which the customer sent the purchase request.
     */
	public int getOrderTick() {
		return orderTick;
	}
	
	/**
     * Retrieves the tick in which the treating selling service started 
     * processing the order.
     */
	public int getProcessTick() {
		return proccessTick;
	}


	/* the following methods shouldn't be thread-safe because there's no way 2 threads will try to access
        one of the following functions simultaneously:
        1. Because only one thread work on a certain OrderBookEvent
        2. The program flow regarding these functions is consequential.
        (3. It's one reciept per book; if the customer wants to buy 2 books - each book gets its own thread.)
         */
	public void setOrderTick(int tick) throws Exception {
		if (tick < 0 || this.orderTick != -1)
			throw new Exception("tick<0 or orderTick has already been assigned");
		this.orderTick = tick;
	}

	public void setProccessTick(int tick) throws Exception {
		if (tick < 0 || this.proccessTick != -1 || this.orderTick > tick)
			throw new Exception("tick<0 or proccessTick has already been assigned or orderTick > (to be process)tick");
		this.proccessTick = tick;
	}

	public void setIssuedTick(int tick) throws Exception {
		if (tick < 0 || this.issuedTick != -1 || this.proccessTick > issuedTick)
			throw new Exception("tick<0 or issuedTick has already been assigned or processTick > (to be issued)tick");
		this.issuedTick = tick;
	}
}
