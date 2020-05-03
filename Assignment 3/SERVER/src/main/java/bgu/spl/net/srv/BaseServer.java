package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.bidi.BidiMessagingProtocolImpl;
import bgu.spl.net.api.bidi.ConnectionsImpl;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {

    private final int port;
    private final Supplier<BidiMessagingProtocolImpl<T>> protocolFactory;
    private final Supplier<MessageEncoderDecoder<T>> encdecFactory;
    private ServerSocket sock;
    //additions
    private ConnectionsImpl connections;

    public BaseServer(
            int port,
            Supplier<BidiMessagingProtocolImpl<T>> protocolFactory,
            Supplier<MessageEncoderDecoder<T>> encdecFactory) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
        this.sock = null;
        // additions;
        this.connections = new ConnectionsImpl();
    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {
            System.out.println("BaseServer started");

            this.sock = serverSock; //just to be able to close

            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept(); /* here we take the (client) socket that wants to register at the server,
                                                            and save it in the Socket clientSocket.*/

                BlockingConnectionHandler<T> handler = new BlockingConnectionHandler<>(
                        //additions
                        connections,
                        clientSock,
                        encdecFactory.get(),
                         protocolFactory.get());
                /* we then build a handler with that clientSocket, a copy of the protocol, and a copy of the encoder-decoder */
                // now we need to add the connection handler to connected users:
                connections.addToConnectedUsers( handler);

                execute(handler); /* then, we execute the handler (in a different thread */
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
        if (sock != null)
            sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler<T>  handler);

}