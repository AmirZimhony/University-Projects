package bgu.spl.mics.application.services;

import bgu.spl.mics.Future;
import bgu.spl.mics.MicroService;
import bgu.spl.mics.application.messages.OrderBookEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.Customer;
import bgu.spl.mics.application.passiveObjects.OrderReceipt;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.ArrayList;
import java.util.concurrent.TimeUnit;
/**
 * APIService is in charge of the connection between a client and the store.
 * It informs the store about desired purchases using {@link //BookOrderEvent}.
 * This class may not hold references for objects which it is not responsible for:
 * {@link// ResourcesHolder}, {@link //MoneyRegister}, {@link //Inventory}.
 * 
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class APIService extends MicroService{
private List<OrderReceipt> orderReceipts;
private List<OrderReceipt> additionalReceipts;
private CountDownLatch countDownLatch;
volatile int CurrentTime=0;
private Customer customer;
int receiptId = 1;

	public APIService(String name,List<OrderReceipt> orders,Customer customer,CountDownLatch countDownLatch) {
		super(name);
		this.customer=customer;
		this.orderReceipts=orders;
		this.countDownLatch=countDownLatch;
		additionalReceipts=new ArrayList<>();
	}

	@Override
	protected void initialize() {
		subscribeBroadcast(TickBroadcast.class, TickBroadcastCallback -> {
			CurrentTime=TickBroadcastCallback.getCurrentTime();
			if (TickBroadcastCallback.getCurrentTime()==TickBroadcastCallback.getDuration()) {
				System.out.println(getName()+" is being terminated");
				customer.addReceipts(additionalReceipts);
				terminate();
			}
			else
			{
				Future<OrderReceipt> future=new Future<>();
				for(OrderReceipt o:orderReceipts){
					if (o.getOrderTick()==CurrentTime) {
						future = sendEvent(new OrderBookEvent(o.getBookTitle(), CurrentTime, customer, customer.getId()*receiptId));
						additionalReceipts.add(future.get(3,TimeUnit.SECONDS));
						receiptId++;
					}
				}
			}
		});
		countDownLatch.countDown();
	}

}
