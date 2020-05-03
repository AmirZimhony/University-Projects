package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoderImpl;
import bgu.spl.net.api.bidi.BidiMessagingProtocolImpl;
import bgu.spl.net.api.bidi.ConnectionsImpl;


import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.ClosedSelectorException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.function.Supplier;

public class Reactor<T> implements Server<T> {

    private final int port;
    private final Supplier<BidiMessagingProtocolImpl<T>> protocolFactory;
    private final Supplier<MessageEncoderDecoderImpl> readerFactory;
    private final ActorThreadPool pool;
    private Selector selector;
    private Thread selectorThread;
    private final ConcurrentLinkedQueue<Runnable> selectorTasks = new ConcurrentLinkedQueue<>();
    // additions:
    private int connectionId;
    private ConnectionsImpl connections;

    public Reactor(
            int numThreads,
            int port,
            Supplier<BidiMessagingProtocolImpl<T>> protocolFactory,
            Supplier<MessageEncoderDecoderImpl> readerFactory) {

        this.pool = new ActorThreadPool(numThreads);
        this.port = port;
        this.protocolFactory = protocolFactory;
        this.readerFactory = readerFactory;

        // additions:
        connectionId = 0;
    }

    @Override
    public void serve() {
        selectorThread = Thread.currentThread();

        // additions:
        this.connections = new ConnectionsImpl();
        //

        try (Selector selector = Selector.open(); // we initiate the selector
             ServerSocketChannel serverSock = ServerSocketChannel.open()) { // and also initiate the server socket.

            this.selector = selector; //just to be able to close

            serverSock.bind(new InetSocketAddress(port)); // Binds the channel's socket to a local address and configures the socket to listen for connections.
            serverSock.configureBlocking(false);
            serverSock.register(selector, SelectionKey.OP_ACCEPT);
            System.out.println("REACTOR Server started");

            while (!Thread.currentThread().isInterrupted()) { // the main loop of the reactor

                selector.select(); // created the table of the threads that have pending tasks.
                runSelectionThreadTasks(); // adds the tasks of changing the selection keys to somewhere.

                for (SelectionKey key : selector.selectedKeys()) {

                    if (!key.isValid()) {
                        continue;
                    } else if (key.isAcceptable()) {
                        handleAccept(serverSock, selector);
                    } else {
                        handleReadWrite(key);
                    }
                }

                selector.selectedKeys().clear(); //clear the selected keys set so that we can know about new events

            }

        } catch (ClosedSelectorException ex) {
            //do nothing - server was requested to be closed
        } catch (IOException ex) {
            //this is an error
            ex.printStackTrace();
        }

        System.out.println("server closed!!!");
        pool.shutdown();
    }

    /*package*/ void updateInterestedOps(SocketChannel chan, int ops) {
        final SelectionKey key = chan.keyFor(selector); // Retrieves the key representing the channel's registration with the given selector.
        if (Thread.currentThread() == selectorThread) { // if we are in the selector thread, then we do something
            key.interestOps(ops); // Sets this key's interest set to the given value.
        } else { // else, we add the task to the relevant queue.
            selectorTasks.add(() -> {
                key.interestOps(ops);
            });
            selector.wakeup();
        }
    }


    private void handleAccept(ServerSocketChannel serverChan, Selector selector) throws IOException {
        SocketChannel clientChan = serverChan.accept();
        clientChan.configureBlocking(false);
        BidiMessagingProtocolImpl bidiMessagingProtocol=protocolFactory.get();
        final NonBlockingConnectionHandler<T> handler = new NonBlockingConnectionHandler<T>(
                connections,
                connectionId,
                readerFactory.get(),
                 protocolFactory.get(),
                clientChan,
                this);
        clientChan.register(selector, SelectionKey.OP_READ, handler);

        //additions:
        connections.addToConnectedUsers(handler);
        increaseConnectionId();
        //
    }

    private void handleReadWrite(SelectionKey key) {
        @SuppressWarnings("unchecked")
        NonBlockingConnectionHandler<T> handler = (NonBlockingConnectionHandler<T>) key.attachment(); // Retrieves the current attachment.

        if (key.isReadable()) {
            Runnable task = handler.continueRead();
            if (task != null) {
                pool.submit(handler, task);
            }
        }

        if (key.isValid() && key.isWritable()) {
            handler.continueWrite();
        }
    }

    private void runSelectionThreadTasks() {
        while (!selectorTasks.isEmpty()) {
            selectorTasks.remove().run();
        }
    }

    @Override
    public void close() throws IOException {
        selector.close();
    }
    private void increaseConnectionId(){
        connectionId++;
        connections.increaseConnectionId();
    }
}