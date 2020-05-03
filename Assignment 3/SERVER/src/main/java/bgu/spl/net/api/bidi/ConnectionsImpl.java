package bgu.spl.net.api.bidi;




import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import bgu.spl.net.srv.bidi.ConnectionHandler;
//************* start with this *****************
/* Implement Connections<T> to hold a list of the new ConnectionHandler interface
for each active client. Use it to implement the interface functions. Notice that
given a connections implementation, any protocol should run. This means that you
keep your implementation of Connections on T.

This interface should map a unique ID for each active client
connected to the server

 */
public class ConnectionsImpl<T> implements Connections<T> {

    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connectedUsers;
    private Integer connectionId;

    public ConnectionsImpl() {
        this.connectedUsers = new ConcurrentHashMap<>();
        this.connectionId = new Integer(0);
    }

    public void addToConnectedUsers(ConnectionHandler<T> connectionHandler) {

        connectedUsers.putIfAbsent(connectionId, connectionHandler);


    }

    /* sends a message T to client represented by the given connId
     */
    public boolean send(int connectionId, T msg) {

        ConnectionHandler connectionHandler = connectedUsers.get(connectionId);
        synchronized (connectionHandler) {
            if (connectionHandler != null) {
                connectionHandler.send(msg);
                return true;
            } else { // assuming that if someone uses send, then the user is logged in

            }
            return false;
        }
    }


    public int getConnectionId() {
        return connectionId.intValue();
    }

    /* sends a message T to all active clients. This
includes clients that has not yet completed log-in by the BGS protocol.
Remember, Connections<T> belongs to the server pattern
implemenration, not the protocol!.
     */
    public void broadcast(T msg) {
        connectedUsers.forEach((connId, connHandler) -> connHandler.send(msg));
    }

    /* removes active client connId from map.
     */
    public synchronized void disconnect(int connectionId) {
        connectedUsers.remove(new Integer(connectionId));
    }//maybe add synchronized- so we don't get the problem we had in assignment 2 (with unregister and send)?


    public synchronized void increaseConnectionId(){
        connectionId++;
    }
}