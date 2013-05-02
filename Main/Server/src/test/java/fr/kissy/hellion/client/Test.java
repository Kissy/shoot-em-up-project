package fr.kissy.hellion.client;

import com.google.common.collect.Lists;
import com.google.common.collect.Sets;
import fr.kissy.hellion.server.domain.Player;
import org.bson.types.ObjectId;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.lang.management.GarbageCollectorMXBean;
import java.lang.management.ManagementFactory;
import java.lang.management.MemoryPoolMXBean;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.UUID;

/**
 * @author Guillaume LE BILLER
 */
public class Test {

    private Logger LOGGER = LoggerFactory.getLogger(Test.class);

    private Integer ELEMENTS = 10000;
    private List<Player> players = Lists.newArrayList();
    private Set<Player> playerSet = Sets.newHashSet();
    private Set<ObjectId> playerIdSet = Sets.newHashSet();

    private void dumpMemory() {
        int mb = 1024*1024;
        Runtime runtime = Runtime.getRuntime();
        System.out.println("Used Memory:" + (runtime.totalMemory() - runtime.freeMemory()) / mb);
    }

    @org.testng.annotations.Test
    public void test() {
        for (int i = 0; i < ELEMENTS; i++) {
            Player player = new Player("SpaceShip.png");
            player.setId(new ObjectId());
            players.add(player);
        }

        dumpMemory();

        for (Player player : players) {
            playerSet.add(player);
            playerIdSet.add(player.getId());
        }

        dumpMemory();

        Random random = new Random();

        long reference=System.nanoTime();
        for (int i = 0; i < 1000; i++) {
            playerSet.contains(players.get(random.nextInt(ELEMENTS)));
        }
        long finishm=System.nanoTime();
        System.out.println( ( (double)(finishm-reference) )/1000000.0);

        reference=System.nanoTime();
        for (int i = 0; i < 1000; i++) {
            playerIdSet.contains(players.get(random.nextInt(ELEMENTS)).getId());
        }
        finishm=System.nanoTime();
        System.out.println( ( (double)(finishm-reference) )/1000000.0);
    }
}
