package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.transform.PlayerToCreateObjectDto;
import fr.kissy.hellion.server.transform.PlayerToDeleteObjectDto;
import org.springframework.context.annotation.Bean;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class TransformConfig {

    @Bean
    public PlayerToCreateObjectDto playerToObjectDto() {
        return new PlayerToCreateObjectDto();
    }

    @Bean
    public PlayerToDeleteObjectDto playerToDeleteObjectDto() {
        return new PlayerToDeleteObjectDto();
    }

}
