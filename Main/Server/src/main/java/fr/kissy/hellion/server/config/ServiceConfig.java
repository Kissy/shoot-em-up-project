package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.service.ObjectService;
import fr.kissy.hellion.server.service.UpstreamMessageService;
import fr.kissy.hellion.server.service.WorldService;
import org.springframework.context.annotation.Bean;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class ServiceConfig {

    @Bean
    public WorldService worldService() {
        return new WorldService();
    }

    @Bean
    public ObjectService objectService() {
        return new ObjectService();
    }

    @Bean
    public UpstreamMessageService upstreamMessageService() {
        return new UpstreamMessageService();
    }

}
