package bgu.spl.net.srv;

import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.WeakHashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class ActorThreadPool {
    /* actorThreadPool helps us managing all the tasks in the server's queue, s.t. there's always ONE task per client at
    most in the main queue. when we finished one task of a client c in the queue, we check if there are other tasks needed
    to be done for c. if so, we add them to to the main queue. */

    private final Map<Object, Queue<Runnable>> acts;
    private final ReadWriteLock actsRWLock;
    private final Set<Object> playingNow;
    private final ExecutorService threads;

    public ActorThreadPool(int threads) {
        this.threads = Executors.newFixedThreadPool(threads);
        acts = new WeakHashMap<>();
        playingNow = ConcurrentHashMap.newKeySet();
        actsRWLock = new ReentrantReadWriteLock();
    }

    public void submit(Object act, Runnable r) {
        synchronized (act) { // so _act_ acts as the keeper for the thread - if we don't have anything with that thread in playingNow- we add r.
            if (!playingNow.contains(act)) {
                playingNow.add(act);
                execute(r, act);
            } else {
                pendingRunnablesOf(act).add(r);
            }
        }
    }

    public void shutdown() {
        threads.shutdownNow();
    }

    private Queue<Runnable> pendingRunnablesOf(Object act) { /* we return the pedgdingRunnables of this thread,
    if exists. if not, we return an empty linked list.*/

        actsRWLock.readLock().lock(); // we lock so that no one else can touch this queue while we get the relevant runnable queue.
        Queue<Runnable> pendingRunnables = acts.get(act);
        actsRWLock.readLock().unlock();

        if (pendingRunnables == null) {
            actsRWLock.writeLock().lock();
            acts.put(act, pendingRunnables = new LinkedList<>());
            actsRWLock.writeLock().unlock();
        }
        return pendingRunnables;
    }

    private void execute(Runnable r, Object act) {
        threads.execute(() -> {
            try {
                r.run();
            } finally {
                complete(act);
            }
        });
    }

    private void complete(Object act) {
        /* if we took the runnable from playingNow, we remove it. else, we took it from pending queue and already
           removed it, and now we remove the next one from the pending queue.*/
        synchronized (act) {
            Queue<Runnable> pending = pendingRunnablesOf(act);
            if (pending.isEmpty()) {
                playingNow.remove(act);
            } else {
                execute(pending.poll(), act); // poll - returns and removes the element from queue.
            }
        }
    }

}
