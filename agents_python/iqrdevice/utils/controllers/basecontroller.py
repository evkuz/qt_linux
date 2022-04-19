from iqrdevice.eventbus import EventBus
from typing import Any, List, Optional


class BaseController(object):
    def __init__(self, name:str):
        self.__event_bus:Optional[EventBus] = None
        self._name:str = name

    @property
    def name(self)->str:
        return self._name

    def set_event_bus(self, bus:EventBus):
        self.__event_bus = bus

    @property
    def channels(self)->List[str]:
        raise NotImplementedError()
    
    def fire_event(self, channel:str, data:Any):
        if self.__event_bus is not None:
            if channel not in self.channels:
                raise ValueError(f"Unregistered channel name: {channel}")
            self.__event_bus.set_event(f"{self._name}.{channel}", data)

