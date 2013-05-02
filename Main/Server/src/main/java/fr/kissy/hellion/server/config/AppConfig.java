package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.core.rtree.RTree;
import fr.kissy.hellion.server.core.rtree.splitter.SplitterType;
import fr.kissy.hellion.server.domain.World;
import fr.kissy.hellion.server.service.WorldService;
import org.springframework.context.annotation.*;
import org.springframework.context.support.PropertySourcesPlaceholderConfigurer;

@Configuration
@PropertySource("/fr/kissy/hellion/server/common.properties")
@Import({DatabaseConfig.class, SecurityConfig.class, ServerConfig.class, HandlerConfig.class, ActorConfig.class, ServiceConfig.class, TransformConfig.class})
public class AppConfig {

    @Bean
    public World world() {
        return new World();
    }

    @Bean
    public RTree rTree() {
        return new RTree(15, 30, SplitterType.QUADRATIC);
    }

    @Bean
    public static PropertySourcesPlaceholderConfigurer getPropertySourcesPlaceholderConfigurer() {
        return new PropertySourcesPlaceholderConfigurer();
    }

}