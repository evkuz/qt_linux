from typing import Any
import threading
from .nodes import BaseNode
from typing import Dict, List
import logging


class EventBus(object):
    def __init__(self, name):
        self._name = name
        self.__channels:Dict[str, List[BaseNode]] = {}

    @property
    def name(self)->str:
        return self._name

    def fire_event(self, channel:str, data:Any)->None:
        #TODO: processing in other thread
        if channel in self.__channels.keys():
            for s in self.__channels[channel]:
                try:
                    s.process_event(channel, data)
                except Exception as e:
                    logging.error(f"in {self._name}: {channel} : {str(e)}")

    def add_subscriber(self, channel:str, subscriber:BaseNode)->None:
        ch_subscribers = self.__channels.setdefault(channel, [])
        ch_subscribers.append(subscriber)
