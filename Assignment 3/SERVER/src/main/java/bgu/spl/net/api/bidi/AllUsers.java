package bgu.spl.net.api.bidi;


import bgu.spl.net.api.bidi.messagesToClient.Notification;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;

public  class  AllUsers {

     private static class HolderOfAllUsers{
         private static AllUsers allUsers= new AllUsers();
     }

     public static AllUsers getInstance() {
         return HolderOfAllUsers.allUsers;
     }
    private int numOfRegistered=0;
    private ArrayList<User> ListOfRegisteredUsers=new ArrayList<>();
    private ConcurrentHashMap<String, User> registeredUsers=new ConcurrentHashMap<>();//Users that have registered
    private ConcurrentHashMap<String, User> loggedInUsers=new ConcurrentHashMap<>();//Users that have logged in
    private ConcurrentHashMap<Integer, String> IDsToNames=new  ConcurrentHashMap<>();//Map between  a user's ID to his name
    private ConcurrentHashMap<String, List<Notification>> allPosts = new ConcurrentHashMap<>(); // map between users and their posts
    private ConcurrentHashMap<String, List<Notification>> allPMs = new ConcurrentHashMap<>(); // map between users and their PMs.


public void registerUser(String name, User user){
        ListOfRegisteredUsers.add(user);
        registeredUsers.put(name,user);
        user.setRegistrationTime(numOfRegistered);
        numOfRegistered++;
    }

public void logInAUser(String name, User user){
         loggedInUsers.put(name,user);
     }

public void logOutAUser(int connectionId){
String userName=IDsToNames.get(connectionId);
IDsToNames.remove(connectionId);
loggedInUsers.remove(userName);
}

public User getUserByName(String name) {
    return registeredUsers.get(name);
}//

public User getUserByConnectionId(int connectionId) {
    String name = IDsToNames.get(connectionId);
    return getUserByName(name);
}

public void MapConnection(Integer connectionNum, String userName){
         IDsToNames.put(connectionNum,userName);
     }

public boolean checkIfLoggedIn(int connectionId) {
    return IDsToNames.get(connectionId) != null;
}

public boolean checkIfLoggedIn(String name) {
    return loggedInUsers.get(name) != null;
}

public String getName(int connectionId) {
    return IDsToNames.get(connectionId);
}

public int getConnectionId(String name) {
    return loggedInUsers.get(name).getConnectionId().intValue();
}

public boolean checkIfRegistered(String name){
    return registeredUsers.containsKey(name);
}



public ConcurrentHashMap getRegisteredUsers(){
    return registeredUsers;
}

public List<User> getListOfRegisteredUsers(){
    return ListOfRegisteredUsers;
}

public void savePost(String name, Notification notification) {
    if (allPosts.get(name) == null) {
        allPosts.put(name, new CopyOnWriteArrayList<>());
    }
    allPosts.get(name).add(notification);
}

public void savePM(String name, Notification notification) {
    if (allPMs.get(name) == null) {
        allPMs.put(name, new CopyOnWriteArrayList<>());
    }
    allPMs.get(name).add(notification);
}

}
