package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import com.google.protobuf.ByteString;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import fr.kissy.hellion.server.service.UpstreamMessageService;
import fr.kissy.hellion.server.service.WorldService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.List;


/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class PlayerShotActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(PlayerShotActor.class);

    @Autowired
    private WorldService worldService;
    @Autowired
    private UpstreamMessageService upstreamMessageService;

    @Override
    public void onReceive(Object o) throws Exception {
        AuthenticatedMessageEvent messageEvent = (AuthenticatedMessageEvent) o;
        LOGGER.debug("Received event {} for user {}", messageEvent.getMessage().getType(), messageEvent.getSubject().getPrincipal());

        ByteString data = messageEvent.getMessage().getData();
        Player player = (Player) messageEvent.getSubject().getSession().getAttribute(Player.class.getSimpleName());
        UpstreamMessageDto.UpstreamMessageProto upstreamMessage = upstreamMessageService.getObjectCreatedMessage(data);

        List<Player> nearPlayers = worldService.getNearPlayers(player);
        for (Player nearPlayer : nearPlayers) {
            nearPlayer.getChannel().write(upstreamMessage);
        }
    }

}
