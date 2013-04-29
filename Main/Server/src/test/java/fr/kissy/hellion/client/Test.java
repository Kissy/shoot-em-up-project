package fr.kissy.hellion.client;

import com.google.common.collect.Lists;
import org.bson.types.ObjectId;

import java.util.List;
import java.util.Random;
import java.util.UUID;

/**
 * @author Guillaume LE BILLER
 */
public class Test {

    private Integer ELEMENTS = 10000;
    private List<String> stringList = Lists.newArrayList();
    private List<ObjectId> objectIdList = Lists.newArrayList();

    @org.testng.annotations.Test
    public void test() {
        for (int i = 0; i < ELEMENTS; i++) {
            stringList.add(UUID.randomUUID().toString());
            objectIdList.add(new ObjectId());
        }

        Random random = new Random();
        String id = stringList.get(random.nextInt(ELEMENTS));
        ObjectId oid = objectIdList.get(random.nextInt(ELEMENTS));

        long reference=System.nanoTime();
        for (int i = 0; i < 1000; i++) {
            boolean a = stringList.contains(id);
            boolean b = stringList.contains(UUID.randomUUID().toString());
        }
        long finishm=System.nanoTime();
        System.out.println( ( (double)(finishm-reference) )/1000000.0);

        reference=System.nanoTime();
        for (int i = 0; i < 1000; i++) {
            boolean a = objectIdList.contains(oid);
            boolean b = objectIdList.contains(new ObjectId());
        }
        finishm=System.nanoTime();
        System.out.println( ( (double)(finishm-reference) )/1000000.0);
    }
}
