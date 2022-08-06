import json
import logging
# from Crypto.Cipher import AES
import base64

from ..discovery import DiscoveryService
from ..domain.agent import Agent
# from ..domain.event import I1820Event
from ..domain.log import I1820Log
from ..exceptions.format import (InvalidAgentFormatException,
                                 InvalidLogFormatException)
from ..exceptions.thing import ThingNotFoundException
from ..things.base import Things


class Handler():
    def __init__(self, discovery_service: DiscoveryService, tenant: str):
        self.logger = logging.getLogger("mqtt.handler")
        self.discovery_service = discovery_service
        self.tenant = tenant
        self.valid_agent_with_things = [
            {"id": "test1", "things": ["test1:1", "test1:2", "test1:3", "test1:4", "test1:5", "test1:6"]},
            {"id": "test2", "things": ["test2:1", "test2:2", "test2:3", "test2:4", "test2:5", "test2:6"]}]

    def on_log(self, _client, _userdata, message):
        '''
        Handles log messages that come from I1820 Agents.
        These messages are used for report and status collecting.

        :param client: the client instance for this callback
        :param userdata: the private user data as set in
        Client() or userdata_set()
        :param message: recived message that contains topic, payload,
        qos, retain.
        :type message: MQTTMessage
        '''
        try:
            log = I1820Log.from_json(message.payload.decode('ascii'))
            if self.authenticate_things(log.agent, log.device) is False:
                self.logger.warning("[%s]: Invalid thingId!", message.topic)
                return
            print(f'log message = log.device={log.device}, log.type={log.type}, agent={log.agent}, state={log.states}')
        except InvalidLogFormatException as e:
            self.logger.warning("[%s]: %s", message.topic, str(e))
            return

        # Sending raw data without any futher processing
        # data = {
        #     'agent_id': log.agent,
        #    'device_id': log.device,
        #     'type': log.type,
        #     'states': {state['name']: state['value'] for state in log.states}
        # }
        # EventController().event(I1820Event('raw', data))

        try:
            thing = Things.get(log.type).get_thing(log.agent, log.device)
        except ThingNotFoundException as e:
            self.logger.warning("[%s]: %s", message.topic, str(e))
            return

        for state in log.states:
            setattr(thing, state['name'], {'value': state['value'],
                                           'time': log.timestamp})

        self.logger.info("[%s]", message.topic)

    def on_discovery(self, client, userdata, message):
        '''
        Handles discovery messages that come from I1820 Agents.
        These messages are used as Raspberry PI heart beat.

        : client: the client instance for this callback
        :param userdata: the private user data as set in
        Client() or userdata_set()
        :param age: recived message that contains topic, payload, qos, retain.
        :type message: MQTTMessage
        '''
        try:
            agent = Agent.from_json(message.payload.decode('ascii'))
            print(f'ping message = agent.ident={agent.ident}, agent.things={type(agent.things)}')
            if self.authenticate_agent(agent) is False:
                self.logger.warning("[%s]: Invalid agentId!", message.topic)
                return
        except InvalidAgentFormatException as e:
            self.logger.warning("[%s]: %s", message.topic, str(e))
            return

        self.discovery_service.ping(agent)

        self.logger.info("discovery on [%s]", agent.ident)

    def authenticate_agent(self, agent: Agent):
        for a in self.valid_agent_with_things:
            if a['id'] == agent.ident:
                return True
        return False

    def authenticate_things(self, agent_id, device_id):
        # cipher = AES.new("mohammad", AES.MODE_ECB)
        # decoded = cipher.decrypt(base64.b64decode(agent_id))
        for a in self.valid_agent_with_things:
            if a['id'] == agent_id:
                for thing in a['things']:
                    if thing == device_id:
                        return True
        return False

    def publish(self, client, body):
        '''
        publish the given body into the tenant.
        '''
        client.publish(f'I1820/{self.tenant}/configuration/request', body)

    def on_connect(self, client, userdata, flags, rc):
        '''
        Called when the broker responds to our connection request.
        It subscribes to thing side channels based on things API token

        :param client: the client instance for this callback
        :param userdata: the private user data as set in
        Client() or userdata_set()
        :param flags: response flags sent by the broker
        :param rc: the connection result
        '''
        # Discovery
        client.subscribe(f'I1820/{self.tenant}/agent/ping')
        client.message_callback_add(f'I1820/{self.tenant}/agent/ping',
                                    self.on_discovery)
        # Log
        client.subscribe(f'I1820/{self.tenant}/log/send')
        client.message_callback_add(f'I1820/{self.tenant}/log/send',
                                    self.on_log)
