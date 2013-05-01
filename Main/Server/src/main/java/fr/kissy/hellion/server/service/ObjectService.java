package fr.kissy.hellion.server.service;

import fr.kissy.hellion.proto.common.ObjectDto;
import fr.kissy.hellion.proto.common.SystemDto;
import fr.kissy.hellion.server.domain.Player;

/**
 * @author Guillaume LE BILLER
 */
public class ObjectService {

    private static final String MOVABLE = "Movable";
    private static final String PLAYER = "Player";
    private static final String IMAGE = "Image";
    private static final String UPDATABLE = "Updatable";
    private static final String NETWORK = "Network";

    /**
     * Get the CREATE player Object.
     *
     * @param player The player to convert.
     * @param isControllable Is the player controllable ?
     * @return The player Object created.
     */
    public ObjectDto.ObjectProto getCreateObject(Player player, boolean isControllable) {
        ObjectDto.ObjectProto.Builder builder = ObjectDto.ObjectProto.newBuilder();
        builder.setName(player.getId().toString());

        ObjectDto.ObjectProto.SystemObjectProto.Builder graphicSystemObject = builder.addSystemObjectsBuilder();
        graphicSystemObject.setSystemType(SystemDto.SystemProto.Type.Graphic);
        graphicSystemObject.setType(IMAGE);
        graphicSystemObject.addProperties(player.getImageProperty().build());

        if (isControllable) {
            ObjectDto.ObjectProto.SystemObjectProto.Builder inputSystemObject = builder.addSystemObjectsBuilder();
            inputSystemObject.setSystemType(SystemDto.SystemProto.Type.Input);
            inputSystemObject.setType(PLAYER);
        }

        ObjectDto.ObjectProto.SystemObjectProto.Builder networkSystemObject = builder.addSystemObjectsBuilder();
        networkSystemObject.setSystemType(SystemDto.SystemProto.Type.Network);
        networkSystemObject.setType(isControllable ? PLAYER : UPDATABLE);

        ObjectDto.ObjectProto.SystemObjectProto.Builder physicSystemObject = builder.addSystemObjectsBuilder();
        physicSystemObject.setSystemType(SystemDto.SystemProto.Type.Physic);
        physicSystemObject.setType(MOVABLE);
        physicSystemObject.addProperties(player.getPositionProperty().build());
        physicSystemObject.addProperties(player.getOrientationProperty().build());
        physicSystemObject.addProperties(player.getVelocityProperty().build());

        return builder.build();
    }

    /**
     * Get the UPDATE player Object.
     *
     * @param player The player to convert.
     * @return The player Object created.
     */
    public ObjectDto.ObjectProto getUpdateObject(Player player) {
        ObjectDto.ObjectProto.Builder builder = ObjectDto.ObjectProto.newBuilder();
        builder.setName(player.getId().toString());
        // Only network system update objects
        ObjectDto.ObjectProto.SystemObjectProto.Builder networkSystemObject = builder.addSystemObjectsBuilder();
        networkSystemObject.setSystemType(SystemDto.SystemProto.Type.Network);
        networkSystemObject.setType(NETWORK);
        networkSystemObject.addProperties(player.getPositionProperty().build());
        networkSystemObject.addProperties(player.getOrientationProperty().build());
        networkSystemObject.addProperties(player.getVelocityProperty().build());
        return builder.build();
    }

    /**
     * Get the DELETE player Object.
     *
     * @param player The player to convert.
     * @return The player Object created.
     */
    public ObjectDto.ObjectProto getDeleteObject(Player player) {
        ObjectDto.ObjectProto.Builder builder = ObjectDto.ObjectProto.newBuilder();
        builder.setName(player.getId().toString());
        return builder.build();
    }
}
