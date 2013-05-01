/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package fr.kissy.hellion.server.handler;

import akka.actor.ActorSystem;
import fr.kissy.hellion.server.config.bus.MessageEventBus;
import fr.kissy.hellion.server.config.bus.StateEventBus;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * Handler implementation for client message.
 */
public class MessageServerHandler extends SimpleChannelUpstreamHandler {

    private static final Logger LOGGER = LoggerFactory.getLogger(MessageServerHandler.class.getName());

    @Autowired
    private MessageEventBus messageEventBus;
    @Autowired
    private StateEventBus stateEventBus;

    /**
     * @inheritDoc
     */
    @Override
    public void messageReceived(ChannelHandlerContext context, MessageEvent event) {
        messageEventBus.publish(event);
    }

    /**
     * @inheritDoc
     */
    @Override
    public void channelDisconnected(ChannelHandlerContext context, ChannelStateEvent event) throws Exception {
        stateEventBus.publish(event);
    }
}
