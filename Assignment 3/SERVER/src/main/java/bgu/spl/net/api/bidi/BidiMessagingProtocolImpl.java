package bgu.spl.net.api.bidi;

import bgu.spl.net.api.bidi.messagesToServer.BasicMessageToServer;

/* – This interface replaces the MessagingProtocol interface.
It exists to support peer 2 peer messaging via the Connections interface.
 */
public class BidiMessagingProtocolImpl<T> implements myBidiMessagingProtocol<T> {

    /* fields */
    private ConnectionsImpl<T> connections;
    private int connectionId;
    private boolean shouldTerminate;



    /* initiate the protocol with the active connections structure of the server and saves the
owner client’s connection id.
     */
    public void start(int connectionId, ConnectionsImpl<T> connections){
        this.connectionId = connectionId;
        this.connections = connections;
        this.shouldTerminate = false;
    }


    /* As in MessagingProtocol, processes a given
message. Unlike MessagingProtocol, responses are sent via the
connections object send function.
     */
    public void process(T message){
        ((BasicMessageToServer)message).process(connectionId, connections, this);
    }

    /**
     * @return true if the connection should be terminated
     */
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    public void changeShouldTerminateTo(boolean shouldTerminate){
        this.shouldTerminate = shouldTerminate;
    }

}
