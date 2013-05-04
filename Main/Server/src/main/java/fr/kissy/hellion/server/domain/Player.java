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
    private PropertyDto.PropertyProto.Builder imageProperty;
    private PropertyDto.PropertyProto.Builder positionProperty;
    private PropertyDto.PropertyProto.Builder velocityProperty;
    private PropertyDto.PropertyProto.Builder orientationProperty;

    // Local Instance
    private Channel channel;
    private Set<Player> nearPlayers = Sets.newHashSet();

    public Player(String image) {
        imageProperty = PropertyDto.PropertyProto.newBuilder();
        imageProperty.setName("Image");
        imageProperty.addValue(ByteString.copyFrom(image.getBytes()));

        positionProperty = PropertyDto.PropertyProto.newBuilder();
        positionProperty.setName("Position");
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));

        orientationProperty = PropertyDto.PropertyProto.newBuilder();
        orientationProperty.setName("Orientation");
        orientationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        orientationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        orientationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        orientationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(1)));

        velocityProperty = PropertyDto.PropertyProto.newBuilder();
        velocityProperty.setName("Velocity");
        velocityProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
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

    public PropertyDto.PropertyProto.Builder getImageProperty() {
        return imageProperty;
    }

    public void setPosition(ByteString x, ByteString y, ByteString z) {
        positionProperty.setValue(0, x);
        positionProperty.setValue(1, y);
        positionProperty.setValue(2, z);
    }

    public PropertyDto.PropertyProto.Builder getVelocityProperty() {
        return velocityProperty;
    }

    public void setVelocity(ByteString x, ByteString y, ByteString z, ByteString w) {
        velocityProperty.setValue(0, x);
        velocityProperty.setValue(1, y);
        velocityProperty.setValue(2, z);
        velocityProperty.setValue(3, w);
    }

    public PropertyDto.PropertyProto.Builder getOrientationProperty() {
        return orientationProperty;
    }

    public void setOrientation(ByteString x, ByteString y, ByteString z, ByteString w) {
        orientationProperty.setValue(0, x);
        orientationProperty.setValue(1, y);
        orientationProperty.setValue(2, z);
        orientationProperty.setValue(3, w);
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
            Float.valueOf(new String(positionProperty.getValue(0).toByteArray())).intValue(),
            Float.valueOf(new String(positionProperty.getValue(1).toByteArray())).intValue(),
            Float.valueOf(new String(positionProperty.getValue(2).toByteArray())).intValue()
        );
    }

    @Transient
    public Box getNearestBounds() {
        Box nearest = getBox();
        nearest.expand(400, 300, 200);
        return nearest;
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
