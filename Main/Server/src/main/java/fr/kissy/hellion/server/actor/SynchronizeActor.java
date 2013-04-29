package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import com.google.common.collect.Sets;
import fr.kissy.hellion.proto.common.ObjectDto;
import fr.kissy.hellion.proto.common.SystemDto;
import fr.kissy.hellion.proto.message.ObjectUpdated;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import fr.kissy.hellion.server.service.WorldService;
import org.bson.types.ObjectId;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.List;
import java.util.Set;


/**
 * Send the list of near players to be created & deleted.
 *
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class SynchronizeActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(SynchronizeActor.class);

    @Autowired
    private WorldService worldService;

    @Override
    public void onReceive(Object o) throws Exception {
        AuthenticatedMessageEvent messageEvent = (AuthenticatedMessageEvent) o;
        LOGGER.debug("Received event {} for user {}", messageEvent.getMessage().getType(), messageEvent.getSubject().getPrincipal());

        Player player = (Player) messageEvent.getSubject().getSession().getAttribute(Player.class.getSimpleName());

        ObjectUpdated.ObjectUpdatedProto.Builder createdObjects = ObjectUpdated.ObjectUpdatedProto.newBuilder();
        ObjectUpdated.ObjectUpdatedProto.Builder deletedObjects = ObjectUpdated.ObjectUpdatedProto.newBuilder();
        ObjectUpdated.ObjectUpdatedProto.Builder updateObject = ObjectUpdated.ObjectUpdatedProto.newBuilder();
        updateObject.addObjects(player.getBuilder());

        // Create the update message
        // TODO make a function or use the update info from the PlayerMoveActor
        ObjectDto.ObjectProto.Builder playerBuilder = ObjectDto.ObjectProto.newBuilder();
        playerBuilder.setName(player.getId().toString());
        ObjectDto.ObjectProto.SystemObjectProto.Builder networkSystemObject = playerBuilder.addSystemObjectsBuilder();
        networkSystemObject.setSystemType(SystemDto.SystemProto.Type.Network);
        networkSystemObject.setType("Network");
        networkSystemObject.addProperties(player.getPositionProperty().build());
        networkSystemObject.addProperties(player.getVelocityProperty().build());

        ObjectUpdated.ObjectUpdatedProto.Builder objectUpdated = ObjectUpdated.ObjectUpdatedProto.newBuilder();
        objectUpdated.addObjects(playerBuilder.build());

        UpstreamMessageDto.UpstreamMessageProto.Builder updatePlayerBuilder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        updatePlayerBuilder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_UPDATED);
        updatePlayerBuilder.setData(objectUpdated.build().toByteString());

        // Create the create message
        // TODO make a function
        ObjectUpdated.ObjectUpdatedProto.Builder nearPlayerCreated = ObjectUpdated.ObjectUpdatedProto.newBuilder();
        nearPlayerCreated.addObjects(player.getBuilder());

        UpstreamMessageDto.UpstreamMessageProto.Builder nearPlayerObjectCreated = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        nearPlayerObjectCreated.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_CREATED);
        nearPlayerObjectCreated.setData(nearPlayerCreated.build().toByteString());

        Set<Player> oldNearPlayers = Sets.newHashSet(player.getNearPlayers());
        List<Player> nearPlayers = worldService.getNearPlayers(player);

        LOGGER.debug("Number of near players for user {} is {}", player.getId(), nearPlayers.size());
        for (Player nearPlayer : nearPlayers) {
            // Send the update message to all players
            if (nearPlayer.getNearPlayers().add(player)) {
                nearPlayer.getChannel().write(nearPlayerObjectCreated.build());
            } else {
                nearPlayer.getChannel().write(updatePlayerBuilder.build());
            }

            // Add the object to creation
            if (!oldNearPlayers.remove(nearPlayer)) {
                player.getNearPlayers().add(nearPlayer);
                createdObjects.addObjects(nearPlayer.getBuilder());
            }
        }

        LOGGER.debug("Number of player to delete for user {} is {}", player.getId(), oldNearPlayers.size());
        for (Player playerToRemove : oldNearPlayers) {
            // TODO use a simple list of ID ?
            player.getNearPlayers().remove(playerToRemove);
            ObjectDto.ObjectProto.Builder playerToDelete = ObjectDto.ObjectProto.newBuilder();
            playerToDelete.setName(playerToRemove.getId().toString());
            deletedObjects.addObjects(playerToDelete.build());

            // Update deleted player
            playerToDelete.setName(player.getId().toString());
            UpstreamMessageDto.UpstreamMessageProto.Builder objectDeleted = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
            objectDeleted.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_DELETED);
            objectDeleted.setData(deletedObjects.build().toByteString());
            player.getChannel().write(objectDeleted.build());
            playerToRemove.getChannel().write(playerToDelete.build());
        }

        if (createdObjects.getObjectsCount() > 0) {
            UpstreamMessageDto.UpstreamMessageProto.Builder objectCreated = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
            objectCreated.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_CREATED);
            objectCreated.setData(createdObjects.build().toByteString());
            player.getChannel().write(objectCreated.build());
        }
        if (deletedObjects.getObjectsCount() > 0) {
            UpstreamMessageDto.UpstreamMessageProto.Builder objectDeleted = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
            objectDeleted.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_DELETED);
            objectDeleted.setData(deletedObjects.build().toByteString());
            player.getChannel().write(objectDeleted.build());
        }
    }
}
