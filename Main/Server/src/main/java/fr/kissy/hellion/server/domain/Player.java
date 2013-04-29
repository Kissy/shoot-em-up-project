package fr.kissy.hellion.server.domain;

import com.google.common.collect.Sets;
import com.google.protobuf.ByteString;
import fr.kissy.hellion.proto.common.ObjectDto;
import fr.kissy.hellion.proto.common.PropertyDto;
import fr.kissy.hellion.proto.common.SystemDto;
import fr.kissy.hellion.server.core.rtree.model.Box;
import fr.kissy.hellion.server.core.rtree.model.BoxObject;
import org.bson.types.ObjectId;
import org.jboss.netty.channel.Channel;
import org.springframework.data.annotation.Id;

import java.beans.Transient;
import java.util.Set;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class Player implements BoxObject {

    @Id
    private ObjectId id;
    private PropertyDto.PropertyProto.Builder positionProperty;
    private PropertyDto.PropertyProto.Builder velocityProperty;

    // Local Instance
    private Channel channel;
    private Set<Player> nearPlayers = Sets.newHashSet();

    public Player() {
        positionProperty = PropertyDto.PropertyProto.newBuilder();
        positionProperty.setName("Position");
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));

        velocityProperty = PropertyDto.PropertyProto.newBuilder();
        velocityProperty.setName("Velocity");
        velocityProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        velocityProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        velocityProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
    }

    public ObjectId getId() {
        return id;
    }

    public void setId(ObjectId id) {
        this.id = id;
    }

    public PropertyDto.PropertyProto.Builder getPositionProperty() {
        return positionProperty;
    }

    public PropertyDto.PropertyProto.Builder getVelocityProperty() {
        return velocityProperty;
    }

    public void setPosition(int x, int y, int z) {
        positionProperty.setValue(0, ByteString.copyFromUtf8(String.valueOf(x)));
        positionProperty.setValue(1, ByteString.copyFromUtf8(String.valueOf(y)));
        positionProperty.setValue(2, ByteString.copyFromUtf8(String.valueOf(z)));
    }

    public void setVelocity(float x, float y, float z) {
        velocityProperty.setValue(0, ByteString.copyFromUtf8(String.valueOf(x)));
        velocityProperty.setValue(1, ByteString.copyFromUtf8(String.valueOf(y)));
        velocityProperty.setValue(2, ByteString.copyFromUtf8(String.valueOf(z)));
    }

    @Transient
    public Channel getChannel() {
        return channel;
    }

    public void setChannel(Channel channel) {
        this.channel = channel;
    }

    @Transient
    public Set<Player> getNearPlayers() {
        return nearPlayers;
    }

    @Override
    public Box getBox() {
        return new Box(
                Integer.valueOf(new String(positionProperty.getValue(0).toByteArray())),
                Integer.valueOf(new String(positionProperty.getValue(1).toByteArray())),
                Integer.valueOf(new String(positionProperty.getValue(2).toByteArray()))
        );
    }

    @Transient
    public Box getNearestBounds() {
        Box nearest = getBox();
        nearest.expand(400, 300, 200);
        return nearest;
    }

    @Transient
    public ObjectDto.ObjectProto.Builder getBuilder() {
        return getBuilder(false);
    }

    @Transient
    public ObjectDto.ObjectProto.Builder getBuilder(boolean isControllable) {
        ObjectDto.ObjectProto.Builder player = ObjectDto.ObjectProto.newBuilder();

        ObjectDto.ObjectProto.SystemObjectProto.Builder graphicSystemObject = player.addSystemObjectsBuilder();
        graphicSystemObject.setSystemType(SystemDto.SystemProto.Type.Graphic);
        graphicSystemObject.setType("Image");

        if (isControllable) {
            ObjectDto.ObjectProto.SystemObjectProto.Builder inputSystemObject = player.addSystemObjectsBuilder();
            inputSystemObject.setSystemType(SystemDto.SystemProto.Type.Input);
            inputSystemObject.setType("Player");
            inputSystemObject.addProperties(velocityProperty.build());
        }

        ObjectDto.ObjectProto.SystemObjectProto.Builder networkSystemObject = player.addSystemObjectsBuilder();
        networkSystemObject.setSystemType(SystemDto.SystemProto.Type.Network);
        networkSystemObject.setType(isControllable ? "Player" : "Updatable");

        ObjectDto.ObjectProto.SystemObjectProto.Builder physicSystemObject = player.addSystemObjectsBuilder();
        physicSystemObject.setSystemType(SystemDto.SystemProto.Type.Physic);
        physicSystemObject.setType("Movable");
        physicSystemObject.addProperties(positionProperty.build());

        return player;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Player player = (Player) o;
        return id.equals(player.id);
    }

    @Override
    public int hashCode() {
        return id.hashCode();
    }

}
